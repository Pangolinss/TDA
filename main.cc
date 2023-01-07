#include<iostream>
#include"ManifoldSampling.h"
#include"Complexes.h"
#define DIM 4

int main(){
    // Simplex simp(100);
    // int word[] = {1,2,3,4,5};
    // int word2[] = {1,2,3,7,8, 9};
    // int word3[] = {10,11,12};
    // int word4[] = {1,2,3, 4};
    // simp.insert(word, 5);
    // simp.insert(word2, 6);
    // simp.insert(word3, 3);
    // simp.insert(word4, 4);
    // simp.print();
    CircleSampler<DIM, double> cs{};
    int num_samples = 100;
    CircleSampler<DIM, double>::EmbMatrix<-1> mat(DIM+1, num_samples);
    cs.UniformSample(mat, num_samples);
    VRComplex(mat, 3);
    std::cout<<"Finished\n"<<std::flush;

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