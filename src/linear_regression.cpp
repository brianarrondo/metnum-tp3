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
	// Por el momento esta hecho para regresiones lineales simples
	A = Matrix(X.rows(), 2);
	A.col(0) = MatrixXd::Constant(X.rows(), 1, 1);
	A.col(1) = X;

	X = A.transpose() * A;
	b = A.transpose() * y;
}


Matrix LinearRegression::predict(Matrix X)
{
    auto ret = MatrixXd::Zero(X.rows(), 1);

    return ret;
}
