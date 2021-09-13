#include <iostream>
#include <fstream>
#include <map>
#include <string>//continue 159
#include <bitset>
struct Node {
    char a = 0;
    int weight = 0;
    Node *left = nullptr, *right = nullptr, *next = nullptr, *prev = nullptr;
    std::string code;
};
struct Tree {
    Node *root;
};
struct List {
    Node *head, *tail;
};
List* add_node_list(List *freq, char symb){
    Node *check = freq->head;
    if(check->a == symb){
        check->weight++;
        return freq;
    }
    while(check->next != nullptr){
        if(check->a != symb){
            check = check->next;
            if(check->a == symb){
                check->weight++;
                return freq;
            }
        } else {
            check->weight++;
            return freq;
        }
    }
    Node *new_node = new Node;
    new_node->a = symb;
    new_node->weight = 1;
    new_node->next = nullptr;
    new_node->prev = freq->tail;
    freq->tail->next = new_node;
    freq->tail = freq->tail->next;
    return freq;
}
void swap(Node* left, Node* right){
    Node *tmp;
    tmp->a = left->a;
    tmp->weight = left->weight;
    left->a = right->a;
    left->weight = right->weight;
    right->a = tmp->a;
    right->weight = tmp->weight;
}
List * sort_list_by_weight(List *freq){
    Node *left = freq->head;
    Node *right = left->next;
    while(left->next){
        while(right){
            if(left->weight > right->weight){
                swap(left, right);
            }
            right = right->next;
        }
        left = left->next;
        right = left->next;
    }
    return freq;
}
void print_tree(Node *root , const int level){
    if(root->left != nullptr){
        print_tree(root->left, level + 1);
    }
    for (int i = 0; i < level; i++)
        std::cout << "   ";
    std::cout << root->weight << " " << root->code << std::endl;
    if(root->right != nullptr){
        print_tree(root->right, level + 1);
    }

}
Node * build_tree(List *freq){
    while(freq->head->next->next != nullptr) {
        //add children
        Node *root = new Node;
        root->weight = freq->head->weight + freq->head->next->weight;
        root->left = freq->head;
        root->right = freq->head->next;
        //insert new node
        Node *tmp;
        tmp = freq->head->next->next;
        while (true) {
            if (root->weight <= tmp->weight) {
                root->next = tmp;
                root->prev = tmp->prev;
                tmp->prev->next = root;
                tmp->prev = root;
                break;
            } else {
                if (tmp->next == nullptr) {
                    tmp->next = root;
                    root->prev = tmp;
                    root->next = nullptr;
                    freq->tail = root;
                    break;
                }
            }
            tmp = tmp->next;
        }
        //cut out used nodes
        freq->head = freq->head->next->next;
        freq->head->prev->prev->next = nullptr;
        freq->head->prev->prev = nullptr;
        freq->head->prev->next = nullptr;
        freq->head->prev = nullptr;
        //add more children
    }
    Node *root = new Node;
    root->weight = freq->head->weight + freq->head->next->weight;
    root->left = freq->head;
    root->right = freq->head->next;
    freq->head->next->prev = nullptr;
    freq->head->next = nullptr;
    return root;
}
void LNR(Node *root, const std::string& code, std::map <char, std::string> *codes){
    if(root->left == nullptr && root->right == nullptr){
        //root->code = code;
        (*codes)[root->a] = code;
        return;
    }
    if(root->left != nullptr){
        LNR(root->left,  code + "0", codes);
    }
    if(root->right != nullptr){
        LNR(root->right, code + "1", codes);
    } else {
        return;
    }
}

void encode(std::map <char, std::string> codes) {
    std::ofstream out;
    out.open(R"(C:\Users\SLAVA\CLionProjects\Huffman\output.txt)");
    if(!out.is_open()){
        std::cout << "OOps something went wrong" << '\n';
    }
    out << "0 ";
    for(auto& it : codes){
        out << it.first << ' ' << it.second << ' ';
    }
    out << '\n';
    std::ifstream fin(R"(C:\Users\SLAVA\CLionProjects\Huffman\input.txt)", std::ios::in);
    char symb = 0, code;
    symb = fin.get();
    uint8_t i = 0;
    while(true){
        int8_t k = 7;
        int8_t counter = 0;
        while(k >= 0){
            while(i < codes[symb].length() && k >= 0) {
                //bool check = ((codes[symb][i] == '0') ? 0 : 1);
                code |= (((codes[symb][i] == '0') ? 0 : 1) << k);
                i++;
                counter++;
                k--;
            }
            if(k>=0) {
                symb = fin.get();
                counter %= 8;
                counter += '0';
                if(symb == EOF){
                    out << code;
                    out.seekp(0);
                    out << char(counter) << ' '; //i - is the number of bits we have in the beginning of last byte
                    std::cout << std::bitset<8>(int(code)) << ' ';
                    fin.close();
                    out.close();
                    return;
                }
                i = 0;
            }
        }
        k = 7;
        std::cout << std::bitset<8>(int(code)) << ' ';
        out << code;
        code = 0;
    }

}
void decode(){
    std::ifstream fin(R"(C:\Users\SLAVA\CLionProjects\Huffman\output.txt)");
    if(fin.is_open()){
        std::string line;
        std::getline(fin, line);
        std::cout << line;
        std::map <char, std::string> codes;
        int8_t residue = line[0];
        int8_t counter = 0;
        int8_t start;
        char symb;
        for(int i = 2; i < line.size(); i++){
            symb = line[i];
            i+=2;
            start = i;
            while(line[i] != ' '){
                i++;
                counter++;
            }
            codes[symb] = std::string(line, start, counter);
            counter = 0;
        }
        std::cout << '\n';
        for(auto& it : codes){
            std::cout << it.first << it.second;
        }
        return;
    }
    std::cout << "Something went wrong" << '\n';
}
int main() {
    std::ifstream fin(R"(C:\Users\SLAVA\CLionProjects\Huffman\input.txt)", std::ios::in);
    char symb;
    List freq{};
    Node *thirst = new Node;
    symb = char(fin.get());
    thirst->a = symb;
    thirst->weight = 1;
    thirst->next = nullptr;
    thirst->prev = nullptr;
    freq.head = thirst;
    freq.tail = thirst;
    while((symb = fin.get()) != EOF){
        add_node_list(&freq, symb);
    }
    fin.close();
    sort_list_by_weight(&freq);
    Node* tmp;
    tmp = freq.head;
    while(tmp != nullptr){
        std::cout << tmp->a << " " << tmp->weight << " ";
        tmp = tmp->next;
    }
    std::cout << '\n';
    Tree huftree{};
    huftree.root = build_tree(&freq);
    std::map <char, std::string> codes;
    auto ptr_codes = &codes;
    LNR(huftree.root, "", ptr_codes);
    print_tree(huftree.root, 0);
    for(auto& it : codes){
        std::cout << it.first << it.second;
    }
    std::cout << '\n';

    encode(codes);
    decode();




    return 0;
}
