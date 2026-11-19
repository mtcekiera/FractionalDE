#pragma once
#include <algorithm>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
#include <vector>
#include <fde/fde.hpp>

struct P12Errors {
    double final_error;
    double max_error;
};

P12Errors P12_single(
    fde::Solver& solver,
    double alpha,
    double lambda = 1.0,
    double T = 1.0,
    std::size_t N = 1000
){
    fde::State y0 = {1.0};

    fde::RHS func = [lambda](
        double t,
        const fde::State& y,
        fde::State& result
    ){
        result = {-lambda * y[0]};
    };

    fde::Jacobian jac = [lambda](
        double t,
        const fde::State& y,
        fde::SquareMatrix& J
    ){
        J(0, 0) = -lambda;
    };

    fde::System system(func, jac, y0, alpha, T, N);

    solver.initialize(system);

    fde::Driver driver;
    fde::Solution solution = driver.solve(system, solver);

    P12Errors errors = {0.0, 0.0};

    const auto& times = solution.get_t();
    const auto& values = solution.get_y();

    for (std::size_t n = 0; n <= N; ++n) {

        const double t = times[n];

        const double y_exact = fde::mittag_leffler_gerrappa(
            -lambda * std::pow(t, alpha),
            alpha,
            1.0,
            0.1,
            100
        );

        const double error = std::abs(values[0][n] - y_exact);

        errors.max_error = std::max(errors.max_error, error);

        if (n == N)
            errors.final_error = error;
    }

    return errors;
}
void P12(double alpha = 1.0){

    namespace fs = std::filesystem;

    fs::path data_dir = fs::current_path() / "data/P12";
    fs::create_directories(data_dir);

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

    for (auto& method : methods) {

        std::ofstream file(data_dir / (method.name + ".csv"));

        file << "N,final_error,max_error\n";
        file << std::scientific << std::setprecision(15);

        for (int k = 4; k <= 10; ++k) {

            const std::size_t N = std::size_t{1} << k;

            P12Errors errors = P12_single(
                method.solver,
                alpha,
                1.0,
                1.0,
                N
            );

            file << N << ","
                 << errors.final_error << ","
                 << errors.max_error << "\n";
        }
    }
}