#include <iostream>
#include <fstream>
#include <map>
#include <string>//continue 159
#include <bitset>
#include <iterator>
#include <algorithm>

struct Node {
    char a = 0;
    int weight = 0;
    Node *left = nullptr, *right = nullptr, *next = nullptr, *prev = nullptr;
    std::string code;
};
struct Bn_Node {
    char a = 0;
    Bn_Node *left = nullptr, *right = nullptr;
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
    out << "0 0 ";
    for(auto& it : codes){
        out << it.first << ' ' << it.second << ' ';
    }
    out << '\n';
    std::ifstream fin;
    fin.open(R"(C:\Users\SLAVA\CLionProjects\Huffman\input.txt)", std::ios::in);
    char symb = 0;
    uint8_t code = 0;
    symb = fin.get();
    uint8_t i = 0;
    bool flag = false;
    while(true){
        int8_t k = 7;
        int8_t counter = 0;
        while(k >= 0){
            while(i < codes[symb].length() && k >= 0) {
                code |= (((codes[symb][i] == '0') ? 0 : 1) << k);
                i++;
                counter++;
                k--;
            }
            if(k>=0) {
                symb = fin.get();
                if(symb == '\n'){
                    flag = true;
                }
                counter %= 8;
                if(fin.eof()){
                    counter += '0';
                    out << code;
                    out.seekp(0);
                    out.put(char(counter)); //i - is the number of bits we have in the beginning of last byte
                    if(flag){
                        out.put(' ');
                        out.put('1');
                    }
                    //std::cout << std::bitset<8>(int(code)) << ' ';
                    fin.close();
                    out.close();
                    out.clear();
                    fin.clear();


                    return;
                }
                i = 0;
            }
        }
        k = 7;
        //std::cout << std::bitset<8>(int(code)) << ' ';
        out << (char)code;
        code = 0;
    }

}
void decode(){
    std::ifstream fin(R"(C:\Users\SLAVA\CLionProjects\Huffman\output.txt)", std::ios::in);
    if(fin.is_open()){
        std::string line;
        std::getline(fin, line);

        //make new map
        std::map <std::string, char> codes;
        int8_t residue = line[0];
        int8_t counter = 0;
        int64_t start;
        char symb;
        std::map <std::string, char>::iterator it;
        if(line[2] == '1'){
            std::string addition;
            std::getline(fin, addition);
            line = line + "\n" + addition;
        }
        for(int i = 4; i < line.size(); i++){
            symb = line[i];
            i+=2;
            start = i;
            while(line[i] != ' ' && i < line.size()){
                i++;
                counter++;
            }
            if(counter != 0){
                codes[std::string(line, start, counter)] = symb;
            }

            counter = 0;
        }

        //decoding
        std::ofstream out(R"(C:\Users\SLAVA\CLionProjects\Huffman\decoded.txt)", std::ios::out);
        int c;
        bool flag;

        std::string bin_code;
        int8_t redim = 0;
        while(!fin.eof()){

            if (!fin.eof()) {
                c = fin.get();
            } else {
                std::cout << "[EoF reached]\n";
                break;
            }
            if(fin.peek() == EOF){
                redim = 8 - residue + '0';
            }
            int k = 7;
            while(k >= redim){
                ((c & (1 << k)) != 0) ? flag = true: flag = false;
                if(flag){
                    bin_code+="1";
                } else {
                    bin_code+="0";
                }
                k--;

                it = codes.find(bin_code);
                if(it != codes.end()){
                    out << it->second;
                    bin_code = "";
                }
            }
        }
        if (fin.eof())                      // check for EOF
            std::cout << "[EoF reached]\n" << fin.good() << fin.bad() << fin.eof() << fin.fail() << '\n';
        out.close();
        fin.close();
        return;
    }
    std::cout << "Something went wrong" << '\n';
}

bool compareFiles(const std::string& p1, const std::string& p2) {
    std::ifstream f1(p1, std::ifstream::binary|std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary|std::ifstream::ate);

    if (f1.fail() || f2.fail()) {
        return false; //file problem
    }

    if (f1.tellg() != f2.tellg()) {
        return false; //size mismatch
    }

    //seek back to beginning and use std::equal to compare contents
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);
    return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                      std::istreambuf_iterator<char>(),
                      std::istreambuf_iterator<char>(f2.rdbuf()));
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
    fin.clear();
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
//    for(auto& it : codes){
//        std::cout << it.first << it.second;
//    }
//    std::cout << '\n';

    encode(codes);
    decode();
    if(compareFiles(R"(C:\Users\SLAVA\CLionProjects\Huffman\input.txt)",
                 R"(C:\Users\SLAVA\CLionProjects\Huffman\decoded.txt)")){
        std::cout<< "Files are equal\n";
    } else {
        std::cout << "Files are not equal\n";
    }



    return 0;
}
