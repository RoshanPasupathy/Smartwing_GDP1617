	#include <stdio.h>
	#include <math.h>
	#include <stdlib.h>

	/*################################################################################################################
	Dynamic Array Code From Matteo Furlan @ http://stackoverflow.com/questions/3536153/c-dynamically-growing-array
	################################################################################################################*/
	typedef struct{
		double* array;
		size_t used;
		size_t size;
	} Array;

	void initArray(Array *a, size_t initialSize) {
	  a->array = (double *)malloc(initialSize * sizeof(double));
	  a->used = 0;
	  a->size = initialSize;
	}

	void insertArray(Array *a, int element) {
	  /*a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
	  Therefore a->used can go up to a->size*/
	  if (a->used == a->size) {
	    a->size *= 2;
	    a->array = (double *)realloc(a->array, a->size * sizeof(double));
	  }
	  a->array[a->used++] = element;
	}

	void freeArray(Array *a) {
	  free(a->array);
	  a->array = NULL;
	  a->used = a->size = 0;
	}

	/*double* joinArrays(Array** arrays, long arraySize){
		long size, i2, i, counter;
		double* a;
		size = 0;
		for(i = 0; i <arraySize;i++){
			size += arrays[i].size;
		}
		a = (double*)malloc(size * sizeof(double));
		counter = 0;
		for(i = 0;i < arraySize;i++){
			for(i2 = 0;i2 < arrays[i].size;i2++){
				a[counter] = arrays[i].array[i2];
				counter += 1;
			}
		}
	}*/


	/*################################################################################################################
	End Dynamic Array Code
	################################################################################################################*/














	/*################################################################################################################
	Collect Data from Files
	################################################################################################################*/

	int Length(char* fileName){
		FILE* myfile = fopen(fileName, "r");
		int ch, lines = 0;

		do
		{
		    ch = fgetc(myfile);
		    if(ch == '\n')
		        lines++;
		} while (ch != EOF);

		if(ch != '\n' && lines != 0)
		    lines++;

		fclose(myfile);
		return lines;
	}


	void SplitData(char* line, double* dataSet){
		char* tempData1, *tempData2, *tempData3;
		long i, i2, start;

		i = 0;
		while(line[i] == ' '){
			i++;
		}
		start = i;

		while(line[i+1] != ' ' && line[i+1] != '-' && line[i+1] != '\n' && line[i+1] != EOF){
			i++;
		}

		tempData1 = (char*)malloc((i-start) * sizeof(char));

		for(i2 = 0; i2 <= i - start;i2++){
			tempData1[i2] = line[start + i2];
		}

		if(line[i] == ' '){
			i++;
		}
		start = i;

		while(line[i+1] != ' ' && line[i+1] != '-' && line[i+1] != '\n' && line[i+1] != EOF){
			i++;
		}

		tempData2 = (char*)malloc((i - start) * sizeof(char));

		for(i2 = 0; i2 <= i - start;i2++){
			tempData2[i2] = line[start + i2];
		}

		if(line[i] == ' '){
			i++;
		}
		start = i;

		while(line[i+1] != ' ' && line[i+1] != '-' && line[i+1] != '\n' && line[i+1] != EOF){
			i++;
		}

		tempData3 = (char*)malloc((i - start) * sizeof(char));

		for(i2 = 0; i2 <= i - start;i2++){
			tempData3[i2] = line[start + i2];
		}

		dataSet[0] = strtod(tempData1,NULL);
		dataSet[1] = strtod(tempData2,NULL);
		dataSet[3] = strtod(tempData3,NULL);

		free(tempData1);
		free(tempData2);
		free(tempData3);
	}

	double** GetFileData(char* fileName, long startLine, int splitIntoArrays){
		long i, index;
		FILE* f;
		char* tempLine;
		char tempChar;
	    double* dataSet;
	    double* xData;
	    double* yData;
	    double* clData;
	    int len;
	    int maxChar;
	    maxChar = 100;

	    len = Length(fileName);

	    dataSet = (double*)malloc(len * sizeof(double));
	    xData = (double*)malloc(len * sizeof(double));
	    yData = (double*)malloc(len * sizeof(double));
	    clData = (double*)malloc(len * sizeof(double));
	    tempLine = (char*)malloc(maxChar * sizeof(char));

	    f = fopen(fileName,"r");
		if(f == NULL){
			printf("File could not be opened");
			return NULL;
		}
		i = 0;
		while(i < startLine){
			fscanf("%s",f,tempLine);
			i++;
		}

		index = 0;
		while(tempLine != EOF){
			SplitData(tempLine,dataSet);
			xData[index] = &dataSet[0];
			yData[index] = &dataSet[1];
			clData[index] = &dataSet[2];
			index ++;
			fscanf("%s",f,tempLine);
		}


		return dataSet;
	}



	/*################################################################################################################
	Calculate Cl and AoA from Cp
	################################################################################################################*/

	int AoAFromCp(double* results, double x, double y, double cp, double* cpData, double* aoaData, long dataLen){
		long i;
		int arraySize;

		arraySize = 0;
		results = (double*)malloc(0);
		for(i = 1; i < dataLen; i++){
			if((cp >= cpData[i - 1] && cp < cpData[i]) || (cp >= cpData[i] && cp < cpData[i - 1]))
			{
	            if(aoaData[i-1] < 13 && aoaData[i] < 13){
	            	arraySize ++;
	            	realloc(results, arraySize * sizeof(double));
	                results[arraySize-1] = (aoaData[i-1] + (aoaData[i] - aoaData[i-1]) * (cp - cpData[i - 1])/(cpData[i] - cpData[i - 1]));
	            }
			}
		}
		return arraySize;
	}
	double ApproxAOA(double* xSet, double* ySet, double* cpSet, double* wSet, long posLen, double* dataSet,long dataLen){
		long i, double aoaSum, double mode;
		Array* aoas;
		double* fullList;
		initArray(aoas,posLen);

		for(i = 0; i < posLen; i++){
			aoa[i] = AoAFromCp(xSet[i],ySet[i],cp[i],dataSet[i],dataLen);
		}
		aoaSum = 0;
		fullList = joinArrays(aos,posLen);


		free(fullList);
	}




	double CLFromCp(double* xSet, double* ySet, double* cp, long aLen, double aoa, int degrees){
		/*TRAPEZOIDAL METHOD OF APPROXIMATING CL*/
		double normal, angle, dx, force;
		long i;


		normal = 0;
		if(degrees == 1){
			angle = aoa * math.pi/180.0;
		}
		else{
			angle = aoa;
		}

		for(i=0;i<aLen - 1;i++){
			dx = (xSet[i+1] * math.cos(angle) + ySet[i+1] * math.sin(angle)) - (xSet[i] * math.cos(angle) + ySet[i] * math.sin(angle));
			force = 0.5 * dx * (cp[i+1] + cp[i])
			normal += force;
		}
		if(xSet[0] != 0){
			dx = (xSet[0] * math.cos(angle) + ySet[0] * math.sin(angle)) - (0 * math.cos(angle) + 0 * math.sin(angle))
	        force = dx * (cp[0] + 0)
	        normal += force
		}

		return normal;
	}



	int main(){

		return 0;
	}