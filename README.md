# Principles of Financial Computation
### hw1
Write a program to price the American put and its delta based on the CRR binomial tree. 

Inputs: 
1. S (spot price)
2. X (strike price)
3. r (risk-free interest rate)
4. s (volatility)
5. T (years)
6. n (number of periods).

Output: put price and delta.

> Example:  
> S = 100, X = 105, r = 0.03, s = 0.25, T = 1, and n = 300  
> Put price: 11.4336  
> Delta: -0.5066

### hw2
Write a program to price the European arithmetic average-rate knock-in call on a non-dividend-paying stock.

Input: 
1. S (spot price)
2. X (strike price)
3. H (barrier price)
4. T (years)
5. r (risk-free interest rate)
6. s (volatility)
7. n (number of periods)
8. k (number of buckets).

Output: call price and its delta

> Example:  
> S = 100, X = 100, H = 110, T = 1, r = 0.05, s = 0.30, and n = 200, and k = 100  
> Call price: 8.3514  
> Delta: 0.5726
