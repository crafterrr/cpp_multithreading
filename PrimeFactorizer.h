#pragma once

#include <vector>
#include <map>
#include <stdint.h>
#include <string>
#include <cmath>

using namespace std;

#include <iostream>
class PrimeFactorizer {
public:
    
    PrimeFactorizer();
    
    string DescribeFactorization(uint64_t& n) {
        string n_str = to_string(n);
        vector<uint64_t> factorization = CalculateFactorization(n);
        string answer = "Prime factors of " + n_str + " are:";
        for (auto &f : factorization){
            answer += " " + to_string(f);
        }
        return answer;
    }
    
    string DescribeFactorization(uint64_t n) const;
    
    vector<uint64_t> GetFactorization(uint64_t& n) const;
    
    static vector<uint64_t> CalculateFactorization(uint64_t& n) {
        vector<uint64_t> factorization;
        if (n < 1){
            factorization.push_back(0);
            return factorization;
        }
        do {
            uint64_t factor = GetMinFactor(n);
            factorization.push_back(factor);
            n = n / factor;
        } while (n != 1);
        return factorization;
    }
    
private:
//    int cache_size = 10;
    
    mutable map< uint64_t, vector<uint64_t> > cache;
    
    static uint64_t GetMinFactor(uint64_t& n) {
        for (int i=2; i<=sqrt(n); i++){
            if (n % i == 0) return i;
        }
        return n;
    }
    
};
