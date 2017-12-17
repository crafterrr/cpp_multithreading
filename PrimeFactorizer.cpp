#include "PrimeFactorizer.h"
#include <iostream>
#include <cmath>

PrimeFactorizer::PrimeFactorizer(){}

string PrimeFactorizer::DescribeFactorization(uint64_t n) const {
    vector<uint64_t> factorization = GetFactorization(n);
    string answer = "Prime factors of " + to_string(n) + " are:";
    for (auto &f : factorization){
        answer += " " + to_string(f);
    }
    return answer;
}

vector<uint64_t> PrimeFactorizer::GetFactorization(uint64_t& n) const {
//    if (cache.find(n) != cache.end()) return cache.at(n);
    vector<uint64_t> factorization = CalculateFactorization(n);
//    cache[n] = factorization;
    return factorization;
}
