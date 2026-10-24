#pragma once
#include <fde/solvers/gl_solver.hpp>
#include <fde/core/driver.hpp>

void test_gl(std::string filename){
    int i = 1;
    fde::System system;
    system.alpha = 1.0;
    system.y0 = {1.0};
    system.func = [](
        double t, 
        const fde::State& y,
        fde::State& result
    ){
        result = {-y[0]};
    };
    system.T = 1.0;
    system.N = 100;

    fde::GLSolver gl;
    gl.initialize(system);
    fde::Driver driver;

    fde::Solution solution = driver.solve(system, gl);
    solution.write_csv(filename);
}
