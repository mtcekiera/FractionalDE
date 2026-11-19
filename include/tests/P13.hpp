#pragma once
#include <fde/fde.hpp>

void P13(){

    namespace fs = std::filesystem;

    const double lambda = 1.0;
    const double T = 1.0;

    struct AlphaCase {
        double alpha;
        std::string name;
    };

    std::vector<AlphaCase> alphas = {
        {0.3, "a03"},
        {0.5, "a05"},
        {0.8, "a08"}
    };

    fde::GLSolver gl;
    fde::L1Solver l1;
    fde::ABMSolver abm;
    fde::PTSolver pt;

    struct Method {
        std::string name;
        fde::Solver& solver;
    };

    std::vector<Method> methods = {
        {"gl", gl},
        {"l1", l1},
        {"abm", abm},
        {"pt", pt}
    };

    for (const auto& alpha_case : alphas) {

        fs::path data_dir =
            fs::current_path() / "data/P13" / alpha_case.name;

        fs::create_directories(data_dir);

        for (auto& method : methods) {

            std::ofstream file(
                data_dir / (method.name + ".csv")
            );

            file << "N,final_error,max_error,eoc_final,eoc_max\n";
            file << std::scientific << std::setprecision(15);

            P12Errors previous = {0.0, 0.0};

            for (int k = 4; k <= 10; ++k) {

                const std::size_t N = std::size_t{1} << k;

                P12Errors errors = P12_single(
                    method.solver,
                    alpha_case.alpha,
                    lambda,
                    T,
                    N
                );

                file << N << ","
                     << errors.final_error << ","
                     << errors.max_error << ",";

                // EOC for final error
                if (k > 4 &&
                    previous.final_error > 0.0 &&
                    errors.final_error > 0.0) {

                    file << std::log2(
                        previous.final_error / errors.final_error
                    );
                }

                file << ",";

                // EOC for maximum error
                if (k > 4 &&
                    previous.max_error > 0.0 &&
                    errors.max_error > 0.0) {

                    file << std::log2(
                        previous.max_error / errors.max_error
                    );
                }

                file << "\n";

                previous = errors;
            }
        }
    }
}