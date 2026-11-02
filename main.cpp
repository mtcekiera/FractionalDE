#include <iostream>
#include <fde/extra/special_functions.hpp>
#include <fde/core/square_matrix.hpp>
#include <fde/core/types.hpp>

#include <fde/solvers/abm_solver.hpp>
#include <fde/solvers/gl_solver.hpp>
#include <fde/solvers/l1_solver.hpp>
#include <fde/solvers/pt_solver.hpp>

#include <tests/test_ml.hpp>
#include <tests/test_solver.hpp>
#include <vector>



int main(){
    
    fde::GLSolver gl;
    fde::L1Solver l1;
    fde::ABMSolver abm;
    fde::PTSolver pt;

    test_solver_mittag_leffler(
        "data/gl_test_a05.csv", gl, 0.5
    );
    test_solver_mittag_leffler(
        "data/gl_test_a1.csv", gl, 1.0
    );
    test_solver_mittag_leffler(
        "data/l1_test_a05.csv", l1, 0.5
    );
    test_solver_mittag_leffler(
        "data/l1_test_a1.csv", l1, 1.0
    );
    test_solver_mittag_leffler(
        "data/abm_test_a05.csv", abm, 0.5
    );
    test_solver_mittag_leffler(
        "data/abm_test_a1.csv", abm, 1.0
    );
    test_solver_mittag_leffler(
        "data/pt_test_a05.csv", pt, 0.5
    );
    test_solver_mittag_leffler(
        "data/pt_test_a1.csv", pt, 1.0
    );


    return 0;
}