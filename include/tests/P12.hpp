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
void P12(double alpha = 1.0, double T = 1.0){

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

        file << "N,h,final_error,max_error\n";    
        file << std::setprecision(
            std::numeric_limits<double>::max_digits10 // 17 digits
        );

        std::size_t steps = 9;
        const double exp_start = -1.0;
        const double exp_end = -4.0;
        const double exp_diff = (exp_end - exp_start) / steps;

        for (std::size_t k = 0; k <= steps; k++) {
            const double exponent = exp_start + k*exp_diff;

            const double h = std::pow(10, exponent);

            const std::size_t N = static_cast<std::size_t>(std::ceil(T / h));

            P12Errors errors = P12_single(
                method.solver,
                alpha,
                1.0,
                1.0,
                N
            );

            file << N << ","
                 << h << ","
                 << errors.final_error << ","
                 << errors.max_error << "\n";
        }
    }
}