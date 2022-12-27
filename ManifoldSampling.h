#ifndef MANIFOLDSAMPLING_H
#define MANIFOLDSAMPLING_H
#include<random>
#include<chrono>
#include <math.h>

template<typename RealType>
class Sampler{
    int emb_dim;
    public:
    Sampler(int d) : emb_dim{d} {}
    virtual RealType* UniformSample(RealType* arr, int num)=0; //samples num points from the manifold, if arr is nullptr will allocate an array
                                                          //Consecutive elements are components of the coordiantes
    int embedded_dim() const {return emb_dim;}
};

template<int Dim, typename RealType>
class CircleSampler : public Sampler<RealType>{
    RealType r;
    public:
    CircleSampler() : Sampler<RealType>{Dim+1}, r{1} {}
    CircleSampler(RealType r_) : Sampler<RealType>{Dim+1}, r{r_} {}
    RealType* UniformSample(RealType* arr, int num) override{
        std::uniform_real_distribution<RealType> dist1(0, M_PI);
        std::uniform_real_distribution<RealType> dist2(0, 2*M_PI);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);
        RealType* ret = nullptr;
        if (arr == nullptr){
            arr = new RealType[this->embedded_dim() * num];
            ret = arr;
        }
        int j;
        for (int i = 0; i < num; ++i){
            for (j = 0; j < Dim; ++j){
                arr[i*this->embedded_dim() + j] = dist1(generator);
            }
            arr[i*this->embedded_dim() + j-1] = dist2(generator);
            RealType acc = 1.0;
            for (j = 0; j < Dim; ++j){
                RealType temp = acc * cos(arr[i*this->embedded_dim() + j]);
                acc *= sin(arr[i*this->embedded_dim() + j]);
                arr[i*this->embedded_dim() + j] = temp;
            }
            arr[i*this->embedded_dim() + j] = acc;
        }
        if (ret != nullptr) return ret;
        else return nullptr;
    }
    RealType* NormalSample(RealType* arr, int num, RealType* mu = nullptr, RealType sd = 1.0){
        std::default_random_engine generator;
        std::normal_distribution<RealType> distribution(mu,sd);
        

    }
};

#endif