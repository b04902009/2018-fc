import numpy as np
days = input("Days before expiration: ")
ar = input("Annual interest rate (%): ")
S0 = input("Stock price at time 0: ")
h0 = input("h0: ")
b0 = input("b0: ")
b1 = input("b1: ")
b2 = input("b2: ")
c = input("c: ")
X = input("Strike price: ")
n_part = input("Number of partitions per day: ")
n_var = input("Number of variances per node: ")

ar /= 100.
r = ar / 365 # daily interest rate
gamma = h0
gamma_n = h0 / (n_part**0.5)
branches = 2 * n_part + 1
# Probability Table
P = [{} for _ in range(days)]
# Eta Table
E  = [{} for _ in range(days)]
# Variance Table
H2  = [{} for _ in range(days+1)]
H2[0][0] = [h0**2]*n_var
    # H2[i][j][0] = h^2_{min} (i,j)
    # H2[i][j][n_var-1] = h^2_{max} (i,j)


## Eta
def compute_eta(h2_t):
    eta = int(np.ceil((h2_t ** 0.5) / gamma))
    while True:
        lower_bound = abs(r - (h2_t/2)) / (2 * eta * gamma * n_part**0.5)
        middle = h2_t / (2 * eta**2 * gamma**2)
        upper_bound = min(1 - abs(r - (h2_t/2)) / (2 * eta * gamma * n_part**0.5), 0.5)
        if lower_bound <= middle and middle <= upper_bound: 
            return eta
        else:
            eta += 1


## Probability
def compute_P(h2_t, eta, P, i, j, k):
    pu = h2_t / (2 * eta**2 * gamma**2) + (r - (h2_t/2)) / (2 * eta * gamma * n_part**0.5)
    pd = h2_t / (2 * eta**2 * gamma**2) - (r - (h2_t/2)) / (2 * eta * gamma * n_part**0.5)
    pm = 1 - h2_t / (eta**2 * gamma**2)
    
    c = np.polynomial.polynomial.polypow([pd, pm, pu], n_part) # (coefficient: low->high, power)
    for l in range(branches):
        P[i][j][k][l] = c[l]


## Variance
def compute_max_min_var(h2_cur, eta, H2, i_next):
    for l in range(-n_part, n_part+1):
        epsilon =  (l * eta * gamma_n - (r - h2_cur / 2))  / (h2_cur ** 0.5)
        h2_next = b0 + b1 * h2_cur + b2 * h2_cur * (epsilon - c)**2
        j_next = j + l * eta
        if H2[i_next].has_key(j_next):
            H2[i_next][j_next][0] = min(H2[i_next][j_next][0], h2_next)
            H2[i_next][j_next][-1] = max(H2[i_next][j_next][-1], h2_next)
        else:
            H2[i_next][j_next] = [h2_next]*n_part
            
def compute_interpolation_var(H2, next_i):
    for j in  sorted(H2[next_i].keys()):
        h2_min = H2[next_i][j][0]
        h2_max = H2[next_i][j][-1]
        for k in range(n_var):
            H2[next_i][j][k] = h2_min + k * (h2_max - h2_min) / (n_var -1)


## Build RT Tree
for i in range(days):
    for j in sorted(H2[i].keys()): 
        E[i][j] = [0] * n_var
        P[i][j] = [[0]*branches for _ in range(n_var)]
        for k in range(n_var):
            h2_cur = H2[i][j][k]
            eta = compute_eta(h2_cur)
            E[i][j][k] = eta
            compute_P(h2_cur, eta, P, i, j, k)
            # compute variance of day i+1 with variance of day i
            compute_max_min_var(h2_cur, eta, H2, i+1)
    # Interpolation
    compute_interpolation_var(H2, i+1)


## Put Option Price at expiration
PP = [{} for _ in range(days+1)] # Put Price
for i in range(days+1):
    for j in sorted(H2[days].keys()):
        PP[i][j] = [max(X - S0 * np.exp(gamma_n * j), 0)] *n_var


## Option price - Interpolation
def compute_interpolation_price(next_i, next_j, h2_cur):
    # Next h^2
    epsilon =  (l * eta * gamma_n - (r - h2_cur / 2))  / (h2_cur ** 0.5)
    h2_next = b0 + b1 * h2_cur + b2 * h2_cur * (epsilon - c)**2
    if h2_next >  H2[next_i][next_j][-1]:  # > h^2_{max}
         return PP[next_i][next_j][-1]
    if h2_next < H2[next_i][next_j][0]:   # < h^2_{min}
        return PP[next_i][next_j][0]
    for next_k in range(n_var-1):
        h2_lower = H2[next_i][next_j][next_k] 
        h2_upper = H2[next_i][next_j][next_k+1]
        if h2_next >= h2_lower and h2_next <= h2_upper:
            if h2_lower == h2_upper:
                return PP[next_i][next_j][next_k]
            x = (h2_next - h2_lower) / (h2_upper - h2_lower)
            return (1-x) * PP[next_i][next_j][next_k] + x * PP[next_i][next_j][next_k+1]


## Backward Induction
for i in range(days-1, -1, -1):
    for j in  sorted(H2[i].keys()):
        for k in range(n_var):
            eta = E[i][j][k]
            h2_cur = H2[i][j][k]
            put = 0
            for l in range(-n_part, n_part+1): # branches
                prob = P[i][j][k][l+n_part]
                next_j = j + eta * l
                price = compute_interpolation_price(i+1, next_j, h2_cur)
                put += price * P[i][j][k][l+n_part]
                    
            PP[i][j][k] = max(put/np.exp(r), 0)
    
    
print 'Put option price: ', PP[0][0][0]