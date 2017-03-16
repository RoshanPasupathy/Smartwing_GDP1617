#include "matrix.h"
#include <math.h>

/**
    * Returns determinant of input matrix
    * Non recursive for sizes 1, 2 and 3
*/
double MatrixFunc::det( double** in_mat, int size){
    int i, j, x, y;
    int mx = 0;
    int my = 0;
    double detA = 0;
    double** minor;
    /**
        Not very elegant for size 1, 2, 3
        but computationally efficient
    */
    if (size == 1){
        return in_mat[0][0];
    }
    else if (size == 2){
        return (in_mat[0][0] * in_mat[1][1]) - (in_mat[0][1] * in_mat[1][0]);
    }
    else if (size == 3){
        return (in_mat[0][0]*((in_mat[1][1] * in_mat[2][2]) - (in_mat[1][2] * in_mat[2][1])))
                - (in_mat[0][1]*((in_mat[1][0] * in_mat[2][2]) - (in_mat[1][2] * in_mat[2][0])))
                + (in_mat[0][2]*((in_mat[1][0] * in_mat[2][1]) - (in_mat[1][1] * in_mat[2][0])));
    }
    else {
        minor = new double*[size -1];
        for (i = 0; i < size - 1; i++){
            minor[i] = new double[size - 1];
        }
        for ( j = 0; j < size; j++){
            // for each element deta = summation(a_ij * cofact(a_ij))
            // populate cofact matrix
            // row = 1 , column = j
            // x =>> cofact rows , y =>> cofact columns
            for (x = 1; x < size; x++){ //iterate over only rows 2 and 3
                for (y = 0; y < size; y++){
                    if (y != j){
                        minor[mx][my] = in_mat[x][y];
                        ++my; //next column
                    }
                    else {
                        //next y
                    }
                }
                ++mx; //once iterated though all columns
                            // next row in cofact
                my = 0;
            }
            mx = 0;
            my = 0;
            detA += in_mat[0][j] * pow((double) -1,((double) (2 + j))) * det(minor, size - 1);
        }

        //teardown
        MatrixUtil::teardown(minor, size - 1);

        //return
        return detA;
    }
}

/**
    * IMPORTANT: Teardown returned array to free memory allocated in function
    * Matrix A * B
    * A has to be square matrix for this function
    * sizeA => number of rows and columns in A
    * columndsB => no of columns in B
    * Consider expanding function to work for non square matrix A
*/
double** MatrixFunc::mat_mult(double** mat_a, double** mat_b, int rowsA,int in_d, int columnsB ){
    double** res_mat;
    res_mat = new double*[rowsA]; //make sure to delete
    for (int ri = 0; ri < rowsA; ri++){
        res_mat[ri] = new double[columnsB];
    }
    for (int r = 0; r < rowsA;  r++){
        for (int c = 0; c < columnsB; c++){
            res_mat[r][c] = 0;
            for (int i = 0; i < in_d; i++){
                res_mat[r][c] += mat_a[r][i] * mat_b[i][c];
            }
        }
    }
    return res_mat;
}

/**
    * IMPORTANT: Teardown returned array to free memory allocated in function
    * Returns inverse of input matrix
    * size => number of rows and columns in input matrix
    * Calls MaruxFunc::det
*/
double** MatrixFunc::inv(double** in_mat, int size){

    //for (int di = 0; di < size; ++di)
    //{
    //    for (int dj = 0; dj < size; ++dj)
    //    {
    //        std::cout << in_mat[di][dj] << ' ';
    //    }
    //    std::cout << std::endl;
    //}

    double det_in = det(in_mat, size);
    int mx = 0;
    int my = 0;
    double** inv_mat;
    //create an output matrix
    inv_mat = new double*[size];
    for (int i = 0; i < size; i++){
        inv_mat[i] = new double[size];
    }
    //create a cofactor matrix
    double** minor;
    minor = new double*[size - 1];
    for (int i = 0; i < size - 1; i++){
        minor[i] = new double[size - 1];
    }
    if (det_in != 0){ //non singular
        for (int x = 0; x < size; x++){
            for (int y = 0;y < size; y++){
                //find cofactors of xy element
                //iterate over all elements again
                for (int ix = 0; ix < size; ix++){ //rows
                    if (ix != x){
                        for (int iy = 0;iy < size; iy++){ //columns
                            if (iy != y){
                                minor[mx][my] = in_mat[ix][iy];
                                ++my;
                            }
                        }
                        ++mx; //increment only if ix != x
                    }
                    my = 0;
                }
                //reset
                mx = 0;
                my = 0;
                /*for (int di = 0; di < size - 1; ++di)
                {
                    for (int dj = 0; dj < size - 1; ++dj)
                    {
                        std::cout << cofact[di][dj] << ' ';
                    }
                    std::cout << std::endl;
                }*/
                //cofactor matrix created
                //out transpose             sign                            minor mat      determinant
                inv_mat[y][x] = pow((double) -1,((double) (x + y))) * det(minor, size - 1) / (double) det_in;
                //std::cout << det(cofact, size - 1) << ' '<< std::endl;
            }
        }
        //return inv_mat;
    }
    else {
        inv_mat = in_mat;
    }

    //teardown local
    MatrixUtil::teardown(minor, size - 1);

    //remember to teardown return value
    return inv_mat;
}

/**
    * Teardown for double pointe
    * Not designed for higher depths yet
*/
void MatrixUtil::teardown(double** in_matrix, int rows){
    for (int d = 0; d < rows; d++ ){
        delete[] in_matrix[d];
    }
    delete[] in_matrix;
}