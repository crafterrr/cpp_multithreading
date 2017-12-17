#include <iostream>
#include <fstream>
#include <string>
#include "PrimeFactorizer.h"
#include "InputParallelizer.h"


using namespace std;

string dt(){
    return "FOO";
}

#include <cmath>
int main(int argc, const char * argv[]) {
    PrimeFactorizer pf = PrimeFactorizer();
    ifstream in;
    ofstream out;
//    in.open("input.txt");
//    out.open("output.txt");
//    ifstream tempin("tin.txt");
//    int a = 21;
//    const double &foo = dt(a);
//    cout << foo << endl;
//    getchar();
    InputParallelizer par("tin.txt", "output.txt");
    par.parallelize(&PrimeFactorizer::DescribeFactorization, pf, 4);
//    par.parallelize(dt, 4);
    return 0;
}
