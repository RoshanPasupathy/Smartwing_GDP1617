#ifndef _MATRIX_H_
#define _MATRIX_H_


namespace MatrixFunc {
    double det( double** in_matrix, int size);
    double** mat_mult(double** matrix_A, double** matrix_B, int rowsA,int inner_dimension, int columnsB );
    double** inv( double** in_matrix, int size);
}

namespace MatrixUtil {
    void teardown(double** in_matrix, int rows);
}

#endif