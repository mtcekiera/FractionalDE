#pragma once
#include <fde/extra/special_functions.hpp>


#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>

double MU = 2.0;
double H = 0.1;
int N = 120;


void test_ml(std::string filename, std::vector<double> alphas,
    double z0 = -1.0, double zEnd = 0.0, double dz = 0.01
){
    std::ofstream file(filename);
    const int steps = static_cast<int>( (zEnd-z0)/dz );

    // normally c++ saves only 6 digits to csv => error at O(1e-6)
    file << std::setprecision(
        std::numeric_limits<double>::max_digits10 // 17 digits
    );
    file << "z";
    for (std::size_t i=0; i<alphas.size(); i++)
        file << ",ML" << i+1;
    file << "\n";

    for(int i=0; i<=steps; i++){
        const double z = z0 + static_cast<double>(i) * dz;
        file << z;
        for (auto alpha : alphas){
            const double val = 
                fde::mittag_leffler_gerrappa(z, alpha, MU, H, N);
            file << "," << val;
        }
            
        file << "\n";
    }   

    file.close();

}