#include <algorithm>
//#include <chrono>
// #include <pybind11/pybind11.h>
#include <iostream>
#include <exception>
#include "linear_regression.h"

using namespace std;
// namespace py=pybind11;

LinearRegression::LinearRegression()
{
}

void LinearRegression::fit(Matrix X, Matrix y)
{
    // Por el momento, la implementacion utiliza regresiones lineales simples
    Matrix A = Matrix(X.rows(), 2);
    A.col(0) = MatrixXd::Constant(X.rows(), 1, 1);
    A.col(1) = X;

    x = (A.transpose() * A).ldlt().solve(A.transpose() * y);
}


Matrix LinearRegression::predict(Matrix X)
{
    Vector y(X.rows());
    for (int i = 0; i < X.rows(); ++i) {
        y(i) = x(0) + x(1) * X(i);
        // cout << y(i) << " = " << x(0) << " + " << x(1) << "*" << X(i) << endl;
    }

    return y;
}
