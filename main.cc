#include<iostream>
#include"Simplex.h"
#include"ManifoldSampling.h"
#define DIM 1

int main(){
    Simplex simp(100);
    int word[] = {1,2,3,4,5};
    int word2[] = {1,2,3,7,8, 9};
    int word3[] = {10,11,12};
    simp.insert(word, 5);
    simp.insert(word2, 6);
    simp.insert(word3, 3);
    CircleSampler<DIM, double> cs{};
    int num_samples = 10;
    double* r = cs.UniformSample(nullptr, num_samples);
    double acc = 0;
    for (int i = 0; i < num_samples; ++i){
        acc = 0;
        for (int j = 0; j < DIM+1; ++j){
            std::cout<<r[i*(DIM+1) + j]<<" ";
            acc += r[i*(DIM+1)+j] * r[i*(DIM+1)+j];
        }
        std::cout<<" | "<<acc;
        std::cout<<std::endl;
    }
    return 0;
}