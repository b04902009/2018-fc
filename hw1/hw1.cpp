/*****************************************************************************************
Write a program to price the American put and its delta based on the CRR binomial tree.
Inputs: (1) S (spot price), (2) X (strike price), (3) r (risk-free interest rate), 
        (4) s (volatility), (5) T (years), (6) n (number of periods). 
Output: put price and delta. 
For example, assume S = 100, X = 105, r = 0.03, s = 0.25, T = 1, and n = 300. 
Then the put price is 11.4336 and the delta is −0.5066.
*****************************************************************************************/
#include <iostream>
#include <cmath>
using namespace std;
int main(){
    double S, X, T, s, r;
    int n;
    printf("Spot price: ");                 scanf("%lf", &S);
    printf("Strike price: ");               scanf("%lf", &X);
    printf("Risk-free interest rate: ");    scanf("%lf", &r);
    printf("Volatility: ");                 scanf("%lf", &s);
    printf("Maturity in years: ");          scanf("%lf", &T);
    printf("Number of periods: ");          scanf("%d", &n);

    double R = exp(r * T/n);
    double u = exp(s * sqrt(T/n)), d = 1 / u;
    double p = (R-d) / (u-d);
    
// Initial values at time t
    /*
    The CRR method ensures that the tree is recombinant.
    This property allows that the value at each final node be calculated directly by
        S_n = S * u^(N_u - N_d)
    where N_u(N_d) is the number of up(down) ticks.
    */
    double P[n+1];
    for(int i = 0; i <= n; i++){
        double Sn = S * pow(u, 2*i-n);  // N_u = i, N_d = n-i => N_u - N_d = 2*i-n
        P[i] = X - Sn;                  // The option value at each final node = Strike price - Spot price
        if(P[i] < 0)  P[i] = 0;
    }
    
// Move to earlier times
    /*
    C_{t-\Delta t,i} = (p*C_{t,i+1} + (1-p)*C_{t,i-1}) / R,
    where C_{t,i} is the option's value for the i^th node at time t.
    => C_{t-\Delta t,i} = p0 * C_{t,i+1} + p1 *C_{t,i-1}),
    where p0 = p / R and p1 = (1-p) / R
    */
    double p0 = p / R;
    double p1 = (1-p) / R;
    double h;   // hedge ratio
    for(int j = n-1; j >= 0; j--){              // period j
        for(int i = 0; i <= j; i++){            // node i
            P[i] = p0 * P[i+1] + p1 * P[i];     // Binomial value C_{j,i}
            double e = X - S * pow(u, 2*i-j);   // Exercise value
            if(P[i] < e)  P[i] = e;             // American option: value = max(Binomial Value, Exercise Value)
        }
        if(j == 1)
            h = (P[1]-P[0])/(S*u-S*d);
    }
    printf("American put price: %lf\nDelta: %lf\n", P[0], h);
    return 0;
}

/*
S = 50; X = 50; r = 0.05; s = 0.2; T = 0.5; n = 100;
put price: 2.3246
delta: -0.432792

S = 100, K = 105, r = 0.03, s = 0.25, T = 1, and n = 300
put price: 11.4336
delta: −0.5066
*/
