/*****************************************************************************************
Write a program to price the American arithmetic average-rate knock-in call on a
non-dividend-paying stock; calculate its delta too. Note that the payoff is the same
as the Asian call, and the knock-in barrier is triggered by the average price too. 
Recall that the option cannot be exercised unless it has been knocked in. 
Inputs: (1) S (spot price), (2) X (strike price), (3) H (barrier price), 
        (4) T(years), (5) r (risk-free interest rate), (6) s (volatility),
        (7) n (number of periods), (8) k (number of buckets). 
Output: call price and its delta. 
For example, S = 100, X = 100, H = 110, T = 1, r = 0.05, s = 0.30, n = 200 and k = 100. 
Then the call price is x.xxxx and its delta is 0.xxx.
*******************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #define American

double S0, X, H, T, s, r;
int n, bk;
double u, d;

double max(double a, double b){return a > b ? a : b;}
void init_stock_price(double **S, double ***A){
    u = exp(s * sqrt(T/n)), d = 1 / u;
    // Stock price
    for(int j = 0; j <= n; j++)
        for(int i = 0; i <= j; i++)
            S[j][i] = S0* pow(u, j-2*i);
    
    // Running average
    double sum_max, sum_min, A_max, A_min;
    for(int j = 0; j <= n; j++)
        for(int i = 0; i <= j; i++){
            sum_min = S0*(1-pow(d, i+1))/(1-d) + S0*pow(d, i)*u*(1-pow(u, j-i))/(1-u);
            sum_max = S0*(1-pow(u, j-i+1))/(1-u) + S0*pow(u, j-i)*d*(1-pow(d, i))/(1-d);
            A_min = sum_min / (j+1);
            A_max = sum_max / (j+1);
            for(int k = 0; k <= bk; k++)
                A[j][i][k] = (double)(bk-k)/bk * A_min + (double)k/bk * A_max;
        }
}
void init_call_price(double ***A, double ***C, int knock_out){
    // Maturity call price
    for(int i = 0; i <= n; i++)
        for(int k = 0; k <= bk; k++){
            C[n][i][k] = max(A[n][i][k]-X, 0);
            if(knock_out && A[n][i][k] > H)  C[n][i][k] = 0;
        }
}
int compute_l(double a, int j, int i, double ***A){
    double A_min = A[j][i][0];
    double A_max = A[j][i][bk];
    if(A_max == A_min)  return 0;
    double l = floor((a-A_min)/(A_max-A_min)*bk);
    return (int)l;
}
double compute_x(double a, int j, int i, int l, double ***A){
    double left = A[j][i][l], right = A[j][i][l+1];
    if(left == right)   return 0;
    return (a-right) / (left-right);
}
double backward_induction(double **S, double ***A, double ***C, int knock_out){
    double R = exp(r * T/n), p = (R-d) / (u-d);
    double Au, Ad, xu, xd, Cu, Cd;
    double binomial, exercise = 0.0;
    int lu, ld;
    for(int j = n-1; j >= 0; j--){              // period j
        for(int i = 0; i <= j; i++){            // node i
            for(int k = 0; k <= bk; k++){
                // Move upward
                Au = ((j+1)*A[j][i][k]+S[j+1][i]) / (j+2);
                if(Au <= A[j+1][i][0])   Cu = C[j+1][i][0];
                else if(Au >= A[j+1][i][bk])  Cu = C[j+1][i][bk];
                else{
                    lu = compute_l(Au, j+1, i, A);
                    xu = compute_x(Au, j+1, i, lu, A);
                    Cu = xu * C[j+1][i][lu] + (1-xu) * C[j+1][i][lu+1];
                }
                // Move downward
                Ad = ((j+1)*A[j][i][k]+S[j+1][i+1]) / (j+2);
                if(Ad <= A[j+1][i+1][0])   Cd = C[j+1][i+1][0];
                else if(Ad >= A[j+1][i+1][bk])  Cd = C[j+1][i+1][bk];
                else{
                    ld = compute_l(Ad, j+1, i+1, A);
                    xd = compute_x(Ad, j+1, i+1, ld, A);
                    Cd = xd * C[j+1][i+1][ld] + (1-xd) * C[j+1][i+1][ld+1];
                }
                binomial = (p * Cu + (1-p) * Cd) / R;
#ifdef American                
                exercise = A[j][i][k] - X;
#endif
                C[j][i][k] = (exercise > binomial)? exercise : binomial;
                if(knock_out && A[j][i][k] > H)  C[j][i][k] = 0;
            }
        }
    }
    return (C[1][0][0]-C[1][1][0]) / (S[1][0]-S[1][1]);
}
double **malloc_2Darray(int x, int y){
    double **a = (double**)malloc(sizeof(double*) * x);
    for(int i = 0; i < x; i++)
        a[i] = (double*)malloc(sizeof(double) * y);
    return a;
}
double ***malloc_3Darray(int x, int y, int z){
    double ***a = (double***)malloc(sizeof(double**) * x);
    for(int i = 0; i < x; i++){
        a[i] = (double**)malloc(sizeof(double*) * y);
        for(int j = 0; j < y; j++)
            a[i][j] = (double*)malloc(sizeof(double) * z);
    }
    return a;
}
int main(){
    printf("Spot price: ");                 scanf("%lf", &S0);
    printf("Strike price: ");               scanf("%lf", &X);
    printf("Barrier price: ");              scanf("%lf", &H);
    printf("Maturity in years: ");          scanf("%lf", &T);
    printf("Risk-free interest rate: ");    scanf("%lf", &r);
    printf("Volatility: ");                 scanf("%lf", &s);
    printf("Number of periods: ");          scanf("%d", &n);
    printf("Number of buckets: ");          scanf("%d", &bk);
 
    double **S = malloc_2Darray(n+1, n+1);
    double ***A = malloc_3Darray(n+1, n+1, bk+1);
    double ***C = malloc_3Darray(n+1, n+1, bk+1);

    // European arithmetic call on a non-dividend-paying stock (No barrier)
    init_stock_price(S, A);
    init_call_price(A, C, 0);
    double delta = backward_induction(S, A, C, 0);
    double call_price = C[0][0][0];

    // European arithmetic average-rate knock-out call on a non-dividend-paying stock
    init_call_price(A, C, 1);
    double delta_back = backward_induction(S, A, C, 1);

    // By in-out parity
    printf("Call price: %f\n", call_price-C[0][0][0]);
    printf("Delta: %f\n", delta-delta_back);
    return 0;
}
/*
knock-out call option
S0 = 100, X = 80, H = 130, T = 1, r = 0.1, s = 0.3, n = 100, bk = 300;
American: 25.6562
European:17.2982, delta: 0.3152

knock-in call option
S0 = 100, X = 100, H = 110, T = 1, r = 0.05, s = 0.30, n = 200, bk = 100;
European: 8.3514, delta: 0.5726.
*/