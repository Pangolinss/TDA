#ifndef SIMPLEX_H
#define SIMPLEX_H
#include<memory>
#include<vector>
#include<iostream>
#include<string>
#include<algorithm>


class Simplex{
    class Node;
    Node* root;
    public:
    class NodeIterator{
        public:
        Node* node;
        NodeIterator(Node* n) : node{n} {}
        Node& operator*(){return *node;}
        NodeIterator& operator++(){node = node->parent; return *this;}
        Node* operator->(){return node;}
        friend bool operator==(const NodeIterator& a, const NodeIterator& b) {return a.node == b.node;};
        friend bool operator!=(const NodeIterator& a, const NodeIterator& b) {return a.node != b.node;};
    };
    using SimplexList = std::vector<NodeIterator>;
    private:
    class Node{
        Simplex& simplex;
        public:
        int letter;
        Node* parent;
        std::vector<Node*> children;
        Node(int k, int c, Simplex& s) :  letter{c}, children{k}, parent{nullptr}, simplex{s} {}
        Node(int k, int c, Node* p, Simplex& s) : letter{c}, children{k}, parent{p}, simplex{s} {}
        NodeIterator begin(){
            return this;
        }
        NodeIterator end(){
            return simplex.root;
        }

    };

    std::vector<SimplexList> face_list;
    Node* search_(Node* node, int* word, int len, int& pos, bool& found, int& depth){
        Node* cur = node;
        found = false;
        for (int i = 0; i < len; ++i){
            pos = i;
            for (int j = 0; j < cur->children.size(); ++j){
                if (cur->children[j]->letter == word[i]){
                    cur = cur->children[j];
                    depth++;
                    goto skip;
                }
            }
            return cur;
            skip:
            continue;
        }
        found = true;
        return cur;
    }
    void insert_(Node* node, int word[], int len, int depth){
        Node* cur = node;
        for (int i = 0; i < len; ++i){
            cur->children.push_back(new Node{0, word[i], cur, *this});
            if (depth >= face_list.size()){
                face_list.emplace_back();
            }
            face_list[depth].push_back( cur->children.back() );
            cur = cur->children.back();
            depth++;
        }
    }
    std::string getWord(NodeIterator node){
        NodeIterator cur = node;
        std::string acc = "";
        while (cur.node != nullptr){
            acc.append( std::to_string(cur->letter) );
            acc.push_back(',');
            ++cur;
        }
        return acc;
    }
    void deleteNode(Node* node){
        if (nullptr) return;
        if (node->children.size() == 0){
            delete node;
            return;
        }
        for (auto it : node->children){
            deleteNode(it);
        }
        delete node;
        return;
    }
    public:
    Simplex() : root{new Node{0, -1, *this}} {}
    ~Simplex(){
        deleteNode(root);
    }

    void insert(int* word, int len){
        int pos = 0;
        bool found;
        int depth = 0;
        Node* end = search_(root, word, len, pos, found, depth);
        if (found == true) return;
        insert_(end, word + pos, len-pos, depth);
    }
    template<typename Iterator>
    void sortInsert(Iterator begin, Iterator end){
        std::vector<int> word(end - begin);
        std::copy(begin, end, word.begin());
        std::sort(word.begin(), word.end());
        int pos = 0;
        bool found;
        int depth = 0;
        Node* node = search_(root, word.data(), word.size(), pos, found, depth);
        if (found == true) return;
        insert_(node, word.data() + pos, word.size() - pos, depth);
    }
    SimplexList& getSimplex(int i){
        return face_list[i];
    }
    int maxSimplex(){
        return face_list.size();
    }
    void print(){
        for (auto list : face_list){
            for (auto it : list){
                std::cout<< getWord(it)<<" ";
            }
            std::cout<<"\n";
        }
    }
};

#endif