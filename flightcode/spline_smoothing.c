

double** smoothing spline(double* x,double* y,double* sig, double lmda,int size){
	int n = size - 1;
	double h[n];
	/*
	u : Diagonal
	v : First Super Diagonal
	w : Second Super diagonal
	n : Size of data
	q : Q'y matrix
	*/
	double u[n];
	double v[n-1];
	double w[n-1];
	/* b has got n + 1 elements*/
	double q[n + 1];

	int i;
	double mu = 2 * (1 - lmda)/(3*lmda);
	double* b;

	/*Max indices*/
	int maxi1 = 0;
	double maxy1 = y[0];

	int maxi2 = 1;
	double maxy2 = y[1];

	int j;

	/*Calculate first two values of h*/
	h[0] = x[1] - x[0];
	h[1] = x[2] - x[1];

	/*Pad arrays*/
	u[0] = 0;
	v[0] = 0;
	w[0] = 0;

	/*Setting Boundary conditions b0 and bn = 0 */
	q[0] = 0;
	q[n] = 0;

	/* w[n-2] os calculated but never called*/
	for (i = 1; i < n-1; i++){
		h[i + 1] = x[i + 2] - x[i + 1];
		u[i] = (9 * sig[i-1]/(h[i-1] * h[i-1])) + (9 * sig[i] * (h[i] + h[i-1]) * (h[i] + h[i-1])/(h[i]*h[i-1]*h[i]*h[i-1])) + (9 * sig[i+1]/(h[i]*h[i]));
		u[i] = mu*u[i] + 2*(h[i] + h[i-1]);

		v[i] = (-9 * sig[i] * (h[i] + h[i-1])/(h[i] * h[i] * h[i-1])) + (-9 * sig[i+1] * (h[i+1] + h[i])/(h[i+1] * h[i] * h[i]));
		v[i] = mu*v[i] + h[i];

		w[i] = mu * 9 * sig[i+1]/(h[i] * h[i+1]);
		q[i] = (3 * (y[i+1] - y[i])/h[i]) - (3 * (y[i] - y[i-1])/h[i-1]);
	}
	u[n-1] = (9 * sig[n-2]/(h[n-2] * h[n-2])) + (9 * sig[n-1] * (h[n-1] + h[n-2]) * (h[n-1] + h[n-2])/(h[n-1]*h[n-2]*h[n-1]*h[n-2])) + (9 * sig[n]/(h[n-1]*h[n-1]);
	u[n-1] = mu*u[n-1] + 2*(h[n-1] + h[n-2]);
	q[n-1] = (3 * (y[n] - y[n-1])/h[n-1]) - (3 * (y[n-1] - y[n-2])/h[n-2]);

	retb(n, u, v, w, q);

	/* Estimate greatest two points and get associated x and y values*/
	for (i = 1; i < n + 1; i++){
		if (y[i] > maxy1){
			/* Update all max values and indices*/
			maxy2 = maxy1;
			maxi2 = maxi1;
			maxy1 = y[i];
			maxi1 = i;
		}
		else if (y[i] > maxy2){
			/* Update only second max*/
			maxy2 = y[i];
			maxi2 = i;
		}
	}
	if (maxi1 < maxi2){
		j = maxi1;
	}
	else {
		j = maxi2;
	}

}

void retb(int n, double* u, double* v, double* w, double* q){
	/*
	* LDL decomoposition to make lower triangle matrix
	  by updating arrays u, v, w

	* Convert q array to b array using forward and back
	  substitution
	*/

	/*double* b = (double *)malloc((n + 1) * sizeof(double));*/

	v[1] = v[1]/u[1];
	w[1] = w[1]/u[1];

	/*
	LDL decomoposition >> update u, v, w
	Possible issues
	- Negative indexing : FIXED
	- Divide by zero : UNLIKELY
	- Check boundaries of arrays
	 */
	int j;
	for (j = 2; j < n - 1; j++){
		u[j] += -(u[j-2] * w[j-2] * w[j-2]) - (u[j-1] * v[j-1] * v[j-1]);
		v[j] = (v[j] - (u[j-1] * v[j-1] * w[j-1]))/u[j];
		w[j] /= u[j];
	}
	u[n-1] += -(u[n-3] * w[n-3] * w[n-3]) - (u[n-2] * v[n-2] * v[n-2]);
	/*Forward substitution*/
	for (j = 2; j < n; j++){
		/* u, v, w indices are one behind bs*/
		q[j] = q[j] - (v[j-1] * q[j-1]) - (w[j-2] * q[j-2]);
	}
	for (j =1; j < n; j++){
		q[j] /= u[j];
	}
	/*Back substitution*/
	/*
	At j = n - 2:
	w[n-2] is called but it doesnt exist but its alright because q[n] = 0
	q[n-1] = q[n-1];
	*/
	for (j = n - 2; j > 0; j--){
		q[j] = q[j] - (v[j]*q[j+1]) - (w[j]*q[j+2]);
	}
}