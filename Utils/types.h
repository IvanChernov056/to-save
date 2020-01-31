#ifndef TYPES_DEFINITION_H
#define TYPES_DEFINITION_H

#include <armadillo>
#include <vector>
#include <map>

using Matrix = arma::mat;
using Column = arma::colvec;
using Row = arma::rowvec;
using SpMat = arma::sp_mat;

using Tensor = Matrix;

using ComplexMatrix = arma::cx_mat;
using ComplexColumn = arma::cx_vec;


using DataVector = Column;
using DataList = std::vector<DataVector>;   // this is to similar vectors
using VectorList = std::vector<Column>;     // this is to different vectors

#endif