#pragma once
#include <fde/extra/special_functions.hpp>


#include <iomanip>
#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>



void P11_extra(
    double t0 = 0.0, double T = 5.0, double dt = 0.01,
    double lambda = 1.0,
    std::vector<double> alphas = {0.3, 0.5, 0.8, 1.0}

){
    namespace fs = std::filesystem;
    fs::path dir = std::filesystem::current_path();
    fs::path data_dir = dir / "data/P11_extra";
    fs::create_directories(data_dir);

    std::ofstream file(data_dir / "P11_extra.csv");
    const int steps = static_cast<int>( (T-t0)/dt );

    // normally c++ saves only 6 digits to csv => error at O(1e-6)
    file << std::setprecision(
        std::numeric_limits<double>::max_digits10 // 17 digits
    );
    file << "t";
    for (auto alpha : alphas)
        file << ",ML" << std::to_string(int(10*alpha));
    file << "\n";

    for(int i=0; i<=steps; i++){
        const double t = t0 + i * dt;
        file << t;

        const double z = -lambda * t;
        for (auto alpha : alphas){
            const double val = 
                fde::mittag_leffler_gerrappa(z, alpha, p11_extra::MU, p11_extra::H, p11_extra::N);
            file << "," << val;
        }
            
        file << "\n";
    }   

    file.close();

}