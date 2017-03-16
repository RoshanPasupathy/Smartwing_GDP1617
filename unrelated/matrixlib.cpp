// Example program
#include <iostream>
#include <string>
#include <math.h>
double det( double** in_matrix, int size);
double** inv( double** in_matrix, int size);
double** mat_mult(double** matrix_A, double** matrix_B, int sizeA, int columnsB );

int main()
{
    double** test_mat_A;
    test_mat_A = new double*[3];
    for (int ia = 0; ia < 3;ia++){
        test_mat_A[ia] = new double[3];
    }

    double ref_mat_A[3][3] = {
        {1.0, 3.0 , 3.0},
        {4.0, 9.0 , 6.0},
        {7.0, 8.0 , 9.0}
    };
    for (int xa = 0;  xa < 3;xa++){
        for (int ya = 0; ya < 3; ya++){
            test_mat_A[xa][ya] = ref_mat_A[xa][ya];
        }
    }

    double** test_mat_B;
    test_mat_B = new double*[3];
    for (int ib = 0; ib < 3;ib++){
        test_mat_B[ib] = new double[1];
    }
    double ref_mat_B[3][1] = {
        {5.0},
        {4.0},
        {7.0}
    };
    for (int xb  = 0;  xb < 3;xb++){
        for (int yb = 0; yb < 1; yb++){
            test_mat_B[xb][yb] = ref_mat_B[xb][yb];
        }
    }


    //create output array
    double** out_array;
    // out_array = new double*[3];
    // for (int i = 0; i < 3;i++){
    //    out_array[i] = new double[1];
    // }
    //out_array = inv(test_mat,3);
    out_array = mat_mult(test_mat_A, test_mat_B, 3, 1);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 1; ++j)
        {
            std::cout << out_array[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    //DEBUG
    double det_test = det(test_mat_A, 3);
    std::cout << det_test << ' '<< std::endl;
}

double det( double** in_mat, int size){
    int i, j, x, y;
    int mx = 0;
    int my = 0;
    double detA = 0;
    double** minor;
    if (size == 1){
        return in_mat[0][0];
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
        return detA;
    }
}

double** inv(double** in_mat, int size){

    //for (int di = 0; di < size; ++di)
    //{
    //    for (int dj = 0; dj < size; ++dj)
    //    {
    //        std::cout << in_mat[di][dj] << ' ';
    //    }
    //    std::cout << std::endl;
    //}

    double det_in = det(in_mat, size);
    std::cout << det_in << ' '<< std::endl;
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
        return inv_mat;
    }
    else {
        return in_mat;
    }
}

double** mat_mult(double** mat_a, double** mat_b, int sizeA, int columnsB ){
    int rowsA = sizeA;
    int columnsA = sizeA;
    double** res_mat;
    res_mat = new double*[rowsA];
    for (int ri = 0; ri < rowsA; ri++){
        res_mat[ri] = new double[columnsB];
    }
    for (int r = 0; r < rowsA;  r++){
        for (int c = 0; c < columnsB; c++){
            res_mat[r][c] = 0;
            for (int i = 0; i < columnsA; i++){
                res_mat[r][c] += mat_a[r][i] * mat_b[i][c];
            }
        }
    }
    return res_mat;
}