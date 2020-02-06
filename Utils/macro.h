#ifndef MACRO_DEFINITION_H
#define MACRO_DEFINITION_H

// #include "types.h"

namespace {
    #define     ZEROS   arma::fill::zeros
    #define     ONES    arma::fill::ones
    #define     NORMAL  arma::fill::randn
    #define     UNIFORM arma::fill::randu
    #define     EYE     arma::fill::eye
    #define     NONE    arma::fill::none
}

#define     base_standatr_value(REAL_TYPE) (static_cast<REAL_TYPE>(::rand()) / static_cast<REAL_TYPE>(RAND_MAX))
#define     standart_rand_valuef base_standatr_value(float)
#define     standart_rand_valued base_standatr_value(double)

#define     BASE_VECTOR_GENERATOR(VEC_TYPE, ELEM_NUM, INIT_VAL) VEC_TYPE(ELEM_NUM, INIT_VAL) 

#define     ZerosColumn(ELEM_NUM) BASE_VECTOR_GENERATOR(Column, ELEM_NUM, ZEROS)
#define     OnesColumn(ELEM_NUM) BASE_VECTOR_GENERATOR(Column, ELEM_NUM, ONES)
#define     NoiseColumn(ELEM_NUM) BASE_VECTOR_GENERATOR(Column, ELEM_NUM, NORMAL)
#define     RandColumn(ELEM_NUM) BASE_VECTOR_GENERATOR(Column, ELEM_NUM, UNIFORM)

#define     ZerosRow(ELEM_NUM) BASE_VECTOR_GENERATOR(Row, ELEM_NUM, ZEROS)
#define     OnesRow(ELEM_NUM) BASE_VECTOR_GENERATOR(Row, ELEM_NUM, ONES)
#define     NoiseRow(ELEM_NUM) BASE_VECTOR_GENERATOR(Row, ELEM_NUM, NORMAL)
#define     RandRow(ELEM_NUM) BASE_VECTOR_GENERATOR(Row, ELEM_NUM, UNIFORM)


#define     BASE_MATRIX_GENERATOR(ROWS_NUM, COLS_NUM, INIT_VAL) Matrix(ROWS_NUM, COLS_NUM, INIT_VAL)

#define     ZerosMatrix(ROWS_NUM, COLS_NUM) BASE_MATRIX_GENERATOR(ROWS_NUM, COLS_NUM, ZEROS)
#define     OnesMatrix(ROWS_NUM, COLS_NUM) BASE_MATRIX_GENERATOR(ROWS_NUM, COLS_NUM, ONES)
#define     RandnMatrix(ROWS_NUM, COLS_NUM) BASE_MATRIX_GENERATOR(ROWS_NUM, COLS_NUM, NORMAL)
#define     RanduMatrix(ROWS_NUM, COLS_NUM) BASE_MATRIX_GENERATOR(ROWS_NUM, COLS_NUM, UNIFORM)
#define     EyeMatrix(ELEM_NUM) BASE_MATRIX_GENERATOR(ELEM_NUM, ELEM_NUM, EYE)

#define     SubDiagMatrix(TENSOR, DIAG_NUM) arma::diagmat(TENSOR, DIAG_NUM)
#define     DiagMatrix(TENSOR) SubDiagMatrix(TENSOR, 0)


#define     RandnSpMatrix(ROWS_NUM, COLS_NUM, DENSITY) arma::sprandn(ROWS_NUM, COLS_NUM, DENSITY)
#define     RanduSpMatrix(ROWS_NUM, COLS_NUM, DENSITY) arma::sprandu(ROWS_NUM, COLS_NUM, DENSITY)
#define     EyeSpMatrix(ELEM_NUM) arma::speye(ELEM_NUM, ELEM_NUM)


#define     NORM2(TENSOR) arma::norm(TENSOR)
#define     ABS(TENSOR) arma::abs(TENSOR)

#define     EIGEN_VAL(MATRIX) arma::eig_gen(MATRIX)
#define     EIGEN_DECOMPOSE(VAL, VEC, MATRIX) arma::eig_gen(VAL, VEC, MATRIX)
#define     EIGEN_SPARSE_VAL(MATRIX, AMOUNT_VAL) arma::eigs_gen(MATRIX, AMOUNT_VAL)
#define     EIGEN_SPARSE_DECOMPOSE(VAL, VEC, MATRIX, AMOUNT_VAL) arma::eigs_gen(VAL, VEC, MATRIX, AMOUNT_VAL)
#define     EIGEN_SYM_VAL(MATRIX) arma::eig_sym(MATRIX)
#define     EIGEN_SYM_DECOMPOSE(VAL, VEC, MATRIX) arma::eig_sym(VAL, VEC, MATRIX)

#define     INV(MATRIX) arma::inv(MATRIX)
#define     PINV(MATRIX) arma::pinv(MATRIX)
#define     INV_SYMPD(MATRIX) arma::inv_sympd(MATRIX)
#define     COV(TENSOR) arma::cov(TENSOR)    

#define     CONCATINATE_PAIR(MODE, LEFT_UP, RIGHT_DOWN) arma::join_##MODE(LEFT_UP, RIGHT_DOWN)
#define     CONCATINATE_PAIR_HORIZ(LEFT, RIGHT) CONCATINATE_PAIR(horiz, LEFT, RIGHT)
#define     CONCATINATE_PAIR_VERT(UP, DOWN) CONCATINATE_PAIR(vert, UP, DOWN)



#define     BASE_LOGGING(LOG_OUT, msg) LOG_OUT << msg << '\n'
#define     CONSOLE_LOG(msg) BASE_LOGGING(std::cout, msg)
#define     ERROR_LOG(msg)  BASE_LOGGING(std::cerr, msg)
#define     FILE_LOG(FILE_NAME, msg) \
		{\
         std::stringstream stream;\
         std::string message("");\
         stream << msg;\
         const int lineLen = 128;\
         while(!stream.eof()){\
           char line[lineLen];\
           stream.getline(line, lineLen);\
           message += std::string(line) + std::string("\n");\
         }\
         fn::logMessageToFile(FILE_NAME, message);\
		}
#define     DEBUG_LOG(msg) \
                CONSOLE_LOG("\n==>\n" << msg << "\n<==\n");\
                ERROR_LOG("\n==>\n" << msg << "\n<==")


#endif
