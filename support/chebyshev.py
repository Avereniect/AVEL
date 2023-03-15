#!/usr/bin/python

import mpmath
from mpmath import mp

# Number of terms in polynomial approximation
N = 6

# Beginning of range over which to approximate
A = mp.mpf(0)

# End of range over which to approximate
B = mp.mpf(mp.pi / 4)


# Note: The polynomial approximation, p, can be fixed to be 1.0 at 0.0 by
# letting the first coefficient be 1.0, and then approximating
# (f(x) - 1.0) / x instead
#

#
# Function to approximate
#
def f(x):
    return (mp.cos(x) - 1.0) / x


#
# computed ith Chebyshev polynomial at x
#
def T(i, x):
    if i == 0:
        return 1

    elif i == 1:
        return x

    return (x + x) * T(i - 1, x) - T(i - 2, x)


#
# Compute u value for x
#
def u(x):
    return 0.5 * ((B - A) * x + A + B)


#
# computed the x value of the ith chebyshev node
#
def chebyshev_node_x(i):
    numer = (2 * mp.mpf(i) - 1) * mp.pi
    denom = 2 * N

    return mp.cos(numer / denom)


def compute_chebyshev_coefficients(n, list_u, list_y):
    sum = mp.mpf(0.0)

    for i in range(1, N + 1):
        t0 = T(n, list_u[i - 1])
        t1 = list_y[i - 1]
        sum += t0 * t1

    if n == 0:
        return sum / N
    else:
        return 2 * sum / N


def compute_chebyshev_polynomial_coefficients(n):
    if n == 0:
        return [mp.mpf(1)]

    if n == 1:
        return [mp.mpf(0.0), mp.mpf(1.0)]

    n_1 = compute_chebyshev_polynomial_coefficients(n - 1)
    n_1 = [2 * x for x in n_1]
    n_1.insert(0, mp.mpf(0))
    n_2 = compute_chebyshev_polynomial_coefficients(n - 2)

    ret = n_1
    for i in range(0, n - 1):
        ret[i] -= n_2[i]

    return ret


def binomial_coefficients(n):
    ret = [0.0] * (n + 1)
    ret[0] = 1.0
    for j in range(1, n + 1):
        ret[j] = ret[j - 1] * (n - j + 1) / j

    return ret


def u_polynomial_to_x_polynomial(u_coefficients, a, b):
    term_count = len(u_coefficients)
    x_coefficients = [0] * term_count

    s = 2 / (b - a)
    t = (-a - b) / (b - a)

    for i in range(0, term_count):
        # Factor in contribution made by ith coefficient

        binom_coefficients = binomial_coefficients(i)

        curr_row = [0] * term_count
        for j in range(0, i + 1):
            s_contrib = mp.power(s, j)
            t_contrib = mp.power(t, i - j)
            curr_row[j] = s_contrib * t_contrib * binom_coefficients[j]

        for j in range(0, term_count):
            curr_row[j] *= u_coefficients[i]

        for j in range(0, term_count):
            x_coefficients[j] += curr_row[j]

    return x_coefficients


def main():
    mp.dps = 256

    list_i = list(range(1, N + 1))
    list_u = [chebyshev_node_x(t) for t in list_i]
    list_x = [u(t) for t in list_u]
    list_y = [f(t) for t in list_x]

    chebyshev_coefficients = [compute_chebyshev_coefficients(t - 1, list_u, list_y) for t in list_i]

    u_polynomial_coefficients = [0] * N
    for i in range(0, N):
        tmp = compute_chebyshev_polynomial_coefficients(i)

        for j in range(0, len(tmp)):
            u_polynomial_coefficients[j] += tmp[j] * chebyshev_coefficients[i]

    mp.dps = 16

    print("Chebyshev polynomial coefficients:")
    for (x, i) in zip(chebyshev_coefficients, range(0, N)):
        print(f"T_{i}: {x}")

    print()

    print("Polynomial approximation (u):")
    for (x, i) in zip(u_polynomial_coefficients, range(0, N)):
        print(f"u^{i}: {x}")

    print()

    x_polynomial_coefficients = u_polynomial_to_x_polynomial(u_polynomial_coefficients, A, B)

    print("Polynomial approximation (x):")
    for (x, i) in zip(x_polynomial_coefficients, range(0, N)):
        print(f"x^{i}: {x}")


if __name__ == "__main__":
    main()
