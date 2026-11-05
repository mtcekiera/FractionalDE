#include <iostream>
#include <fde/extra/special_functions.hpp>
#include <fde/core/square_matrix.hpp>
#include <fde/core/types.hpp>

#include <fde/solvers/abm_solver.hpp>
#include <fde/solvers/gl_solver.hpp>
#include <fde/solvers/l1_solver.hpp>
#include <fde/solvers/pt_solver.hpp>

#include <tests/P11_ml.hpp>
#include <tests/P12.hpp>
#include <vector>



int main(){

    P11_ml();
    P12();
 
    return 0;
}