#include <fde/core/solution.hpp>
#include <fstream>
#include <iomanip>

fde::Solution::Solution(
    std::size_t dim, 
    std::size_t N
) : y(dim), f(dim) 
{
    t.reserve(N+1);

    for (std::size_t i = 0; i<dim; i++){
        y[i].reserve(N+1);
        f[i].reserve(N+1);
    }
}

void fde::Solution::push_back(
    double time, 
    const State& yn,
    const State& fn
){
    if (
        (yn.size() != y.size()) or (fn.size() != f.size())
    )
        throw std::invalid_argument("Dimension mismatch in y");
    t.push_back(time);
    for (std::size_t i=0; i<dimension(); i++){
        y[i].push_back(yn[i]);
        f[i].push_back(fn[i]);
    }
}

std::size_t fde::Solution::size() const{
    return t.size();
}

std::size_t fde::Solution::dimension() const{
    return y.size();
}

void fde::Solution::write_csv(
    std::string filename,
    int precision
) const {
    std::ofstream file(filename);
    // normally c++ writes only 6 digits to csv => error at O(1e-6)
    file << std::setprecision(precision);

    file<<"t";
    for (std::size_t i=0; i<dimension(); i++)
        file << ",y" << i;
    for (std::size_t i=0; i<dimension(); i++)
        file << ",f" << i;
    file << "\n";

    for (std::size_t n=0; n<size(); n++){
        file << t.at(n);
        for (std::size_t i=0; i<dimension(); i++)
            file << "," << y[i].at(n);
        for (std::size_t i=0; i<dimension(); i++)
            file << "," << f[i].at(n);
        file << "\n";
    }
    file.close();
}