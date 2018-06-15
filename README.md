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

### hw3
Write a least-squares Monte Carlo program to price American-style Asian puts. 
Note that the payoff is the same as the Asian put (of course, it is based on the running average if early exercised). 

Input:
1. S (spot price)
2. X (strike price)
3. T (years)
4. r (risk-free interest rate)
5. s (volatility)
6. n (number of periods)
7. k (number of simulation paths)

Output:  price and standard error

> Example:  
> S = 100, X = 100, T = 1, r = 0.05, s = 0.30, n = 100, and k = 100000  
> Put price: 5.5126  
> Standard error is 0.0251

### hw4
Write a program to price European put options based on the GARCH model (Ritchken-Trevor algorithm).

Inputs:
1. E (days before expiration)
2. r (%) (interest rate)
3. S (stock price at time 0)
4. h0
5. b0
6. b1
7. b2
8. c 
9. X (strike price)
10. n1 (number of partitions per day)
11. n2 (number of variances per node). 

Output:  price

> Example:  
> E = 30, r (%) = 5, S = 100, h0 = 0.010469, b0 = 0.000006575, b1 = 0.9, b2 = 0.04, c = 0, X = 100, n1 = 3, n2 = 3  
> Option price: about 2.0163
