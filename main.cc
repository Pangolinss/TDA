#include<iostream>
#include"Simplex.h"
#include"ManifoldSampling.h"
#define DIM 4

int main(){
    // Simplex simp(100);
    // int word[] = {1,2,3,4,5};
    // int word2[] = {1,2,3,7,8, 9};
    // int word3[] = {10,11,12};
    // simp.insert(word, 5);
    // simp.insert(word2, 6);
    // simp.insert(word3, 3);
    CircleSampler<DIM, double> cs{};
    int num_samples = 10;
    CircleSampler<DIM, double>::EmbMatrix<-1> mat(DIM+1, 10);
    cs.UniformSample(mat, num_samples);
    double acc = 0;
    std::cout<<mat;
    std::cout<<mat.transpose() * mat;
    // CircleSampler<DIM, double>::CoordVector vec = CircleSampler<DIM, double>::CoordVector::Zero();
    // CircleSampler<DIM, double>::CoordSquareMatrix cov;
    // int i = 0;
    // for (auto& it : cov.reshaped()){
    //     it = i/5.0;
    //     i++;
    // }
    // cov = cov*cov.transpose();
    // CircleSampler<DIM, double>::EmbMatrix<-1> mat(DIM+1, 10);
    // cs.StereographicNormalSample(mat, vec, cov);
    // std::cout<<mat;
    // std::cout<<mat.transpose()* mat;
    return 0;
}