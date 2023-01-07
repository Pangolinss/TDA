#ifndef COMPLEXES_H
#define COMPLEXES_H
#include<Eigen/Core>
#include<Eigen/SparseCore>
#include<vector>
#include<utility>
#include<set>
#include"Simplex.h"
#include <bitset>

template<typename WeightType>
using AdjMatrix = Eigen::SparseMatrix<WeightType>;

template<typename WeightType>
class Graph{
    public:
    Graph<WeightType>(int num_v) : adjmatrix(num_v, num_v) {}
    using Edge = std::pair<int, int>;
    AdjMatrix<WeightType> adjmatrix; //adjacency matrix should be treated as a symmetric matrix so we only store the top triangle
    std::vector<Edge> edges;
};

template<typename WeightType>
class Complex{
    public:
    Simplex simplex;
    AdjMatrix<WeightType> skeleton;
};


//An exhaustive nearest point search
template<typename RealType, int Dim>
Graph<RealType> NearestPointGraph(RealType delta, const Eigen::Matrix<RealType, Dim, -1>& data){
    Graph<RealType> graph(data.cols());
    std::vector<Eigen::Triplet<RealType>> triplet_list;
    for (int i = 0; i < data.cols(); ++i){
        for (int j = i; j < data.cols(); ++j){
            RealType dist = (data.col(i) - data.col(j)).squaredNorm();
            if (dist < delta * delta){
                triplet_list.emplace_back(i, j, 1);
                if (i != j) graph.edges.emplace_back(i, j);
            }
        }
    }
    graph.adjmatrix.setFromTriplets(triplet_list.begin(), triplet_list.end());
    return graph;
}

//Using the Inductive VR algorithm specificed in Fast Construction of the Vietoris-Rips Complex (Zamorodian, A)
template<typename RealType, int Dim>
Complex<RealType> VRComplex(const Eigen::Matrix<RealType, Dim, -1>& data, int k){
    Complex<RealType> complex;
    Graph<RealType> graph = NearestPointGraph<RealType>(1.2, data);
    for (int i = 0 ; i < data.cols(); ++i){
        complex.simplex.insert(&i, 1);
    }
    for (auto edge : graph.edges){
        complex.simplex.insert(&(edge.first), 2);
    }

    auto outer = graph.adjmatrix.outerIndexPtr();
    auto inner = graph.adjmatrix.innerIndexPtr();

    std::set<int> set;
    std::set<int> temp_set;

    std::vector<int> word;
    for (int i = 1; i < k; ++i){
        if (i >= complex.simplex.maxSimplex()) break;
        auto& vec = complex.simplex.getSimplex(i);
        //computing intersection of neighbours
        for (auto& simplex : vec){
            set.clear();
            temp_set.clear();
            word.clear();
            for (int i = outer[simplex->letter]; i < outer[simplex->letter+1]; ++i){
                set.insert(inner[i]);
            }
            for (auto it : *simplex){
                word.push_back(it.letter);
                int vertex = it.letter;
                for (int i = outer[vertex]; i < outer[vertex+1]; ++i){
                    if (set.find(inner[i]) != set.end()){
                        temp_set.insert(inner[i]);
                    }
                }
                set.swap(temp_set);
                temp_set.clear();
            }
            word.push_back(0);
            for (auto it : set){
                word.back() = it;
                complex.simplex.sortInsert(word.begin(), word.end());
            }
        }
    }
    // std::cout<<graph.adjmatrix;
    // complex.simplex.print();
    return complex;
}



template<typename RealType>
void GraphInducedComplex(const AdjMatrix<RealType>& graph, Simplex& simplex){
    
}

#endif