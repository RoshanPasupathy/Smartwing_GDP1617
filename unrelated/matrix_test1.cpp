#include "matrix.h"
#include <stdio.h>
#include <math.h>
#define ERR_RES 0.01
bool test_det(void);
bool test_mult(void);
bool test_inv(void);

int main(){
	printf("Testing ...\n");
	bool res_det, res_mult, res_inv;
	res_det = test_det();
	res_mult = test_mult();
	res_inv = test_inv();
	printf("Determinant function test : %s \n", res_det ? "PASS" : "FAIL");
	printf("Multiplication function test : %s \n", res_mult ? "PASS" : "FAIL");
	printf("Inverse function test : %s \n", res_inv ? "PASS" : "FAIL");
}

bool test_det(){
	/*
		* Create two test_cases
	*/
	//printf("Creating pointer test_mat_A ...\n");
	double** test_mat_A;
	//printf("Requesting memory pointer of pointers ...\n");
    test_mat_A = new double*[3];
    //printf("Requesting memory pointer of doubles per pointer ...\n");
    for (int ia = 0; ia < 3;ia++){
        test_mat_A[ia] = new double[3];
    }
    double ref_mat_A[3][3] = {
        {1.0, 3.0 , 3.0},
        {4.0, 9.0 , 6.0},
        {7.0, 8.0 , 9.0}
    };

    //printf("Assigning data to test_mat_A ...\n");
    for (int xa = 0;  xa < 3;xa++){
        for (int ya = 0; ya < 3; ya++){
            test_mat_A[xa][ya] = ref_mat_A[xa][ya];
        }
    }

    //printf("Creating pointer test_mat_B ...\n");
	double** test_mat_B;
	//printf("Requesting memory pointer of pointers ...\n");
    test_mat_B = new double*[2];
    //printf("Requesting memory pointer of doubles per pointer ...\n");
    for (int ib = 0; ib < 2;ib++){
        test_mat_B[ib] = new double[2];
    }

    double ref_mat_B[2][2] = {
        {3.0 , 3.0},
        {4.0, 9.0 }
    };
    for (int xb = 0;  xb < 2;xb++){
        for (int yb = 0; yb < 2; yb++){
            test_mat_B[xb][yb] = ref_mat_B[xb][yb];
        }
    }

    //printf("Assigning data to test_mat_B\n");
    //expected result
    double test_res_A = -42;
    double test_res_B = 15;

    //printf("Calculating determinant of A ...\n");
    double calc_res_A = MatrixFunc::det(test_mat_A, 3);

    //printf("Calculating determinant of B ...\n");
    double calc_res_B = MatrixFunc::det(test_mat_B, 2);
    //teardown
    MatrixUtil::teardown(test_mat_A,3);
    MatrixUtil::teardown(test_mat_B,2);

    //Result checking
    if (fabs(calc_res_A - test_res_A) <= ERR_RES && fabs(calc_res_B - test_res_B) <= ERR_RES){
    	return true;
    }
    else {
    	printf("Expected result A : %f , Calculated result A : %f\n", test_res_A, calc_res_A);
    	printf("Expected result B : %f , Calculated result B : %f\n", test_res_B, calc_res_B);
    	return false;
    }
}

bool test_mult() {
	bool res = true;
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
        test_mat_B[ib] = new double[2];
    }
    double ref_mat_B[3][2] = {
        {5.0, 8},
        {4.0, -4},
        {7.0, 12}
    };
    for (int xb  = 0;  xb < 3;xb++){
        for (int yb = 0; yb < 2; yb++){
            test_mat_B[xb][yb] = ref_mat_B[xb][yb];
        }
    }

    //correct answer
    double exp_out[3][2] = {
        {38, 32},
        {98, 68},
        {130, 132}
    };

    //create output array
    double** out_array;
    // out_array = new double*[3];
    // for (int i = 0; i < 3;i++){
    //    out_array[i] = new double[2];
    // }
    //Calulate output
    out_array = MatrixFunc::mat_mult(test_mat_A, test_mat_B, 3,3, 2);

    for (int ox = 0; ox < 3; ox++){
    	for (int oy = 0; oy < 2; oy ++){
    		if (fabs(out_array[ox][oy] - exp_out[ox][oy]) > ERR_RES){
    			printf("Index x = %d , y = %d\n", ox, oy);
    			printf("Expected result : %f , Calculated result: %f\n", exp_out[ox][oy], out_array[ox][oy]);
    			res = false;
    		}
    	}
    }
    //teardown
    MatrixUtil::teardown(out_array, 3);
    MatrixUtil::teardown(test_mat_A, 3);
    MatrixUtil::teardown(test_mat_B, 3);
    return res;
}

bool test_inv(){
	bool res = true;
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

    double exp_out[3][3] = {
        {-0.785714, 0.07142857, 0.214285714},
        {-0.1428571428, 0.285714, -0.14285714},
        {0.738095, -0.30952, 0.07142857}
    };

    //create output array
    double** out_array;
    out_array = MatrixFunc::inv(test_mat_A, 3);

    for (int ox = 0; ox < 3; ox++){
    	for (int oy = 0; oy < 3; oy ++){
    		if (fabs(out_array[ox][oy] - exp_out[ox][oy]) > ERR_RES){
    			printf("Index x = %d , y = %d\n", ox, oy);
    			printf("Expected result : %f , Calculated result: %f\n", exp_out[ox][oy], out_array[ox][oy]);
    			res = false;
    		}
    	}
    }
    //teardown
    MatrixUtil::teardown(out_array, 3);
    MatrixUtil::teardown(test_mat_A, 3);
    return res;
}