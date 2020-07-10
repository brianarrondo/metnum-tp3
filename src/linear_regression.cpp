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
    Matrix A = Matrix(X.rows(), X.cols() + 1);
    Matrix ones = MatrixXd::Constant(X.rows(), 1, 1);
    A << ones, X;

    x = (A.transpose() * A).ldlt().solve(A.transpose() * y);
}


Matrix LinearRegression::predict(Matrix X)
{   
    Vector y(X.rows());
    for (int i = 0; i < X.rows(); ++i) {
        y(i) += x(0);
        for (int j = 1; j < x.rows(); ++j) {
            y(i) += x(j) * X(i,j-1);
        }
    }

    return y;
}
