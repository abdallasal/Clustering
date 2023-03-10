#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <string.h>


/*This section contains the helper functions*/

/*
 * Input: x, y: pointers to a double array. d: the length of the array.
 * Output: the euclidean distance between the arrays.
 */
double eucDist(double *x, double *y, int d){
    int i;
    double res=0;
    for(i=0; i<d; i++){
        res+=(double)pow((x[i]-y[i]), 2);
    }
    res=(double)pow(res, 0.5);
    return res;
}


/*
 * Input: x, y: pointers to a double array. d: the length of the array.
 * Output: the weight between x and y.
 */
double RBF(double *x, double *y, int d){
    double res;
    res=eucDist(x, y, d);
    res=res/2;
    res=exp(-res);
    return res;
}


void matPrint(double **mat, int n, int d){
    int i,j;
    for(i=0; i<n; i++){
        for(j=0; j<d; j++){
            printf("%lf, ", mat[i][j]);
        }
        printf("\n");
    }
}


/*
 * Input: row: an array of doubles. n: int of the size of the array.
 * Output: The sum of the numbers in the array.
 */
double rowSum(double *row, int n){
    double res=0;
    int i;
    for(i=0; i<n; i++){
        res+=row[i];
    }
    return res;
}


/*
 * Input: A,B: 2D-array of doubles. n: dimension.
 * Output: A-B.
 */
double** matSub(double **A, double **B, int n){
    double **C;
    double res;
    int i,j;
    C=calloc(n, sizeof(double *));
    assert(C!=NULL);
    for(i=0; i<n; i++){
        C[i]=calloc(n, sizeof(double));
        assert(C[i]!=NULL);
    }
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            res=A[i][j]-B[i][j];
            C[i][j]=res;
        }
    }
    return C;
}


/*
 * Input: A: 2D-array of doubles (diagonal matrix). n: dimension.
 * Output: All the elements in the diagonal is raised to the power -0.5.
 */
double** matSquare(double **A, int n){
    double **B;
    double res;
    int i;
    B=calloc(n, sizeof(double *));
    assert(B!=NULL);
    for(i=0; i<n; i++){
        B[i]=calloc(n, sizeof(double));
        assert(B[i]!=NULL);
    }
    for(i=0; i<n; i++){
        res=(double) pow(A[i][i], 0.5);
        res=1/res;
        B[i][i]=res;
    }
    return B;
}


/*
 * Input: A,B: 2D-array of doubles. n: dimension.
 * Output: A*B. (* is matrices multiplication).
 */
double** matMult(double **A, double **B, int n){
    double **C;
    double res;
    int i, j, k;
    C=calloc(n, sizeof(double *));
    assert(C!=NULL);
    for(i=0; i<n; i++){
        C[i]=calloc(n, sizeof(double));
        assert(C[i]!=NULL);
    }
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            res=0;
            for(k=0; k<n; k++){
                res+=(double)(A[i][k]*B[k][j]);
                C[i][j]=res;
                if(res==-0){
                    C[i][j]=0;
                }
            }
        }
    }
    return C;
}


/*
 * Input: mat: 2D-array of doubles. n: dimension.
 * This function frees memory of a matrix implemented as an array of pointers.
 */
void matFree(double **mat, int n){
    int i;
    for(i=0; i<n; i++){
        free(mat[i]);
    }
    free(mat);
}


/*
 * Input: A: 2D-array of doubles. n, d: dimensions.
 * Output: At: the transpose of A.
 */
double** transMat(double **A, int n, int d){
    double **At;
    int i, j;
    At=calloc(d, sizeof(double *));
    assert(At!=NULL);
    for(i=0; i<d; i++){
        At[i]=calloc(n, sizeof(double));
        assert(At[i]!=NULL);
    }
    for(i=0; i<n; i++){
        for(j=0; j<d; j++){
            At[j][i]=A[i][j];
        }
    }
    return At;
}


/*
 * Input: A: 2D-array of doubles. n: dimension.
 * Output: 0: if the matrix isn't diagonal, else: 1.
 */
int isDiag(double **A, int n){
    int i, j;
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if(i!=j){
                if(A[i][j]!=0){
                    return 0;
                }
            }
        }
    }
    return 1;
}

/*This section contains the algorithm's main functions*/

/*
 * Input: path: string of the file's path. n, d: a pointers to int.
 * This function reads the data from the file and stores it in a 2D-array (matrix) of doubles,
 * with the dimensions nxd. It also updates the values of n and d to a further uses.
 * Output: 2D-array (nxd) containing the points.
 */
double** readFile(char* path, int *n, int *d){
    FILE *fp;
    int i, j, num=0, dim=1;
    char c;
    double cord;
    double **points;
    /*
    * First iterate over the file to get the number of points (num),
    * and to get the dimension of the points (dim).
    * this iteration will provide the dimension of the data matrix.
    */
    fp=fopen(path, "r");
    if (fp==NULL){
        printf("Invalid Input!");
        exit(0);
    }
    for (c=getc(fp); c!=EOF; c=getc(fp)){
        if (num==0){
            if(c==','){
            dim+=1;
            }
        }
        if (c=='\n'){
        num+=1;
        }
    }
    if(num==0){
        printf("Invalid Input!");
        fclose(fp);
        return NULL;
    }
    rewind(fp);
    //updating n and d
    *n=num;
    *d=dim;
    points= calloc(num, sizeof(double *));
    assert(points!=NULL);
    for(i=0; i<num; i++){
        points[i]=calloc(dim, sizeof(double));
        assert(points[i]!=NULL);
    }
    for (i=0; i<num; i++) {
        for (j=0; j<dim; j++) {
            fscanf(fp, "%lf", &cord);
            points[i][j]=(double) cord;
            fgetc(fp);
        }
    }
    fclose(fp);
    return points;
}


/*
 * Input: points: the points from the file in the form of a 2D-array. n, d: the dimension
 * of the points array.
 * This function creates the weighted adjacency matrix.
 * Output: W: the weighted adjacency matrix.
 */
double** createW(double **points, int n, int d){
    double **W;
    double res;
    int i,j;
    W=calloc(n, sizeof(double *));
    assert(W!=NULL);
    for(i=0; i<n; i++){
        W[i]=calloc(n, sizeof(double));
        assert(W[i]!=NULL);
    }
    for(i=0; i<n; i++){
        for(j=0; j<n; j++){
            if(i==j){
                continue;
            }
            res=RBF(points[i], points[j], d);
            if(res<0){
                continue;
            }
            W[i][j]=res;
        }
        printf("%lf\n", W[0][i]);
    }
    return W;
}


/*
 * Input: W: 2D-array of doubles displaying the weights. n: dimension.
 * Output: D: diagonal matrix with the sum of each W's row weights in the diagonal.
 */
double** createD(double **W, int n){
    double **D;
    double res;
    int i;
    D=calloc(n, sizeof(double *));
    assert(D!=NULL);
    for(i=0; i<n; i++){
        D[i]=calloc(n, sizeof(double));
        assert(D[i]!=NULL);
    }
    for(i=0; i<n; i++){
        res=rowSum(W[i], n);
        D[i][i]=res;
    }
    return D;
}


/*
 * Input: n: dimension.
 * Output: Identity matrix.
 */
double** createI(int n){
    double **I;
    int i;
    I=calloc(n, sizeof(double *));
    assert(I!=NULL);
    for(i=0; i<n; i++){
        I[i]=calloc(n, sizeof(double));
        assert(I[i]!=NULL);
    }
    for(i=0; i<n; i++){
        I[i][i]=(double)1;
    }
    return I;
}




/*
 * Input:
 * Output:
 */


void main(){
    char *pa="C:/Users/IDEAPAD330S/Desktop/University/8th Semester/Software Project/Homeworks/Proj1/input.txt";
    int *n,*d;
    double **points;
    double **W;
    double **D;
    double **Ds;
    double **I;
    double **DsW;
    double **DsWDs;
    double **Ln;
    points=readFile(pa, &n, &d);
    printf("%d %d\n", n, d);
    matPrint(points, n, d);

    W=createW(points, n, d);
    printf("W\n");
    matPrint(W, n, n);

    D=createD(W, n);
    printf("D\n");
    matPrint(D, n, n);

    Ds=matSquare(D, n);
    printf("Ds\n");
    matPrint(Ds, n, n);

    I=createI(n);
    printf("I\n");
    matPrint(I, n, n);

    DsW=matMult(Ds, W, n);
    printf("DsW\n");
    matPrint(DsW, n, n);

    DsWDs=matMult(DsW, Ds, n);
    printf("DsWDs\n");
    matPrint(DsWDs, n, n);

    Ln=matSub(I, DsWDs, n);
    printf("Ln\n");
    matPrint(Ln, n, n);


    /*freeing memory*/
    matFree(Ln, n);
    matFree(DsWDs, n);
    matFree(DsW, n);
    matFree(I, n);
    matFree(Ds, n);
    matFree(D, n);
    matFree(W, n);
    matFree(points, n);
    free(n);
    free(d);
    free(pa);

}