#!/usr/bin/env python

import struct
from mpmath import mp

N = 6
"""
Number of terms in polynomial approximation
"""

A = mp.mpf(0.0)
"""
Beginning of range over which to approximate
"""

B = mp.pi / 4.0
"""
End of range over which to approximate
"""


def f(x):
    """
    Function to approximate

    Note: The polynomial approximation, p, can be fixed to be C at 0.0 by
    letting the first coefficient be C, and then approximating
    (f(x) - C) / x instead. This can be used to make the error zero at
    x == 0.0.

    :param x:
    :return:
    """

    return mp.sin(x) / x


def T(i, x):
    """
    computed ith Chebyshev polynomial at x

    :param i:
    :param x:
    :return:
    """
    if i == 0:
        return 1

    elif i == 1:
        return x

    return (x + x) * T(i - 1, x) - T(i - 2, x)


def u(x):
    """
    Compute u value for x
    :param x:
    :return:
    """

    return 0.5 * ((B - A) * x + A + B)


def chebyshev_node_x(i):
    """
    Compute the x value of the ith Chebyshev node
    :param i:
    :return:
    """
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

    desmos_polynomial = ""
    for (i, C) in enumerate(reversed(x_polynomial_coefficients)):
        coefficient_as_string = f"{C}"
        if 'e' in coefficient_as_string:
            coefficient_as_string = coefficient_as_string.replace('e', '* 10^{')
            coefficient_as_string += '}'

        desmos_polynomial += coefficient_as_string + "x^" + "{" + f"{N - i - 1}" + "} + "

    desmos_polynomial = desmos_polynomial.rstrip(" + ")

    print()
    print(f"Desmos polynomial (x):\n{desmos_polynomial}")

    hex_polynomial = ""
    for (i, C) in enumerate(reversed(x_polynomial_coefficients)):
        as_hex = hex(struct.unpack('<I', struct.pack('<f', C))[0])
        hex_polynomial += f"x^{N - i - 1}: {as_hex}\n"

    print()
    print(f"Hex polynomial (x):\n{hex_polynomial}")


if __name__ == "__main__":
    main()
