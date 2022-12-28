#ifndef MANIFOLDSAMPLING_H
#define MANIFOLDSAMPLING_H
#include<random>
#include<iostream>
#include<chrono>
#include<math.h>
#include<functional>
#include<Eigen/Core>
#include<Eigen/Eigenvalues>
#include<Eigen/Cholesky>

template<typename RealType, int Emb_Dim>
class Sampler{
    public:
    static const int emb_dim = Emb_Dim;
    template<int n>
    using EmbMatrix = Eigen::Matrix<RealType, Emb_Dim, n>;
    using EmbVector = Eigen::Matrix<RealType, Emb_Dim, 1>;
    Sampler(){}
    virtual void UniformSample(EmbMatrix<-1>& samples, int num)=0; //samples num points from the manifold, if arr is nullptr will allocate an array
                                                          //Consecutive elements are components of the coordiantes
    const int embedded_dim() const {return emb_dim;}
};

template<int Dim, typename RealType>
class CircleSampler : public Sampler<RealType, Dim+1>{
    RealType r;
    public:
    template<int n>
    using CoordMatrix = Eigen::Matrix<RealType, Dim, n>;
    using CoordSquareMatrix = Eigen::Matrix<RealType, Dim, Dim>;
    using CoordVector = Eigen::Matrix<RealType, Dim, 1>;
    template<int n>
    using EmbMatrix = Eigen::Matrix<RealType, Sampler<RealType, Dim+1>::emb_dim, n>;
    using EmbVector = Eigen::Matrix<RealType, Sampler<RealType, Dim+1>::emb_dim, 1>;
    CircleSampler() : Sampler<RealType, Dim+1>{}, r{1} {}
    CircleSampler(RealType r_) : Sampler<RealType, Dim+1>{}, r{r_} {}
    void UniformSample(EmbMatrix<-1>& samples, int num) override{
        std::uniform_real_distribution<RealType> dist1(0, M_PI);
        std::uniform_real_distribution<RealType> dist2(0, 2*M_PI);
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator (seed);
        int j;
        for (int i = 0; i < num; ++i){
            for (j = 0; j < Dim; ++j){
                samples(j,i) = dist1(generator);
            }
            samples(j-1,i) = dist2(generator);
            RealType acc = 1.0;
            for (j = 0; j < Dim; ++j){
                RealType temp = acc * cos(samples(j,i));
                acc *= sin(samples(j,i));
                samples(j,i) = temp;
            }
            samples(j,i) = acc;
        }
    }
    //Must be a valid covariance matrix
    void StereographicNormalSample(EmbMatrix<-1>& samples, const CoordVector& mu, const CoordSquareMatrix& cov){
        //Using mean and covariance matrix generate normal distribution in R^n
        int num = samples.cols();
        Eigen::LLT<CoordSquareMatrix> llt(cov);
        auto L = llt.matrixL();
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine generator(seed);
        std::normal_distribution<RealType> distribution(0,1);
        RealType* ret = nullptr;
        Eigen::Matrix<RealType, -1, -1> data(Dim, num);
        for (auto& it : data.reshaped()){
            it = distribution(generator);
        }
        Eigen::Matrix<RealType, -1, -1> res = L * data;
        res.colwise() += mu;
        //Use a stereographic projection to send our points in R^n -> R^n+1 on the unit circle
        int i = 0;
        for (auto vec : samples.colwise()){
            RealType s_sq = res.col(i).squaredNorm();
            vec[vec.size()-1] = (s_sq-1)/(s_sq+1);
            for (int j = 0; j < vec.size()-1; ++j){
                vec[j] = 2 * res.col(i)[j]/(s_sq + 1);
            }
            i++;
        }
    }

};

#endif