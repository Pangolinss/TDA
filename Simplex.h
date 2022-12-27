#ifndef SIMPLEX_H
#define SIMPLEX_H
#include<memory>
#include<vector>
#include<iostream>

class Simplex{
    int V;
    class Node{
        public:
        int letter;
        std::vector<Node*> children;
        Node(int k, int c) : letter{c}, children{k} {}
    };
    Node* root;
    Node* search_(Node* node, int word[], int len, int& pos){
        Node* cur = node;
        for (int i = 0; i < len; ++i){
            pos = i;
            for (int j = 0; j < cur->children.size(); ++j){
                if (cur->children[j]->letter == word[i]){
                    cur = cur->children[j];
                    goto skip;
                }
            }
            return cur;
            skip:
            continue;
        }
        return cur;
    }
    void insert_(Node* node, int word[], int len){
        Node* cur = node;
        for (int i = 0; i < len; ++i){
            cur->children.push_back(new Node{0, word[i]});
            cur = cur->children.back();
        }
    }
    void print(Node* node){
        std::cout<<node->letter<<"\n";
        for (auto it : node->children){
            print(it);
        }
    }
    public:
    Simplex(int V_) : V{V}, root{new Node{0, -1}} {}
    void insert(int word[], int len){
        int pos = 0;
        Node* end = search_(root, word, len, pos);
        insert_(end, word + pos, len-pos);
    }
    void print(){
        print(root);
    }
};

#endif