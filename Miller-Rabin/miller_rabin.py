"""
miller-rabin prime test

Test if n is prime with error probability less than 2^(−𝑠).

"""

import random
from exponentiation import exp

Prime = 0
Composite = 1


def miller_rabin(n, s):
    if n == 2:
        return Prime
    elif n % 2 == 0:
        return Composite

    for _ in range(s):
        a = random.randint(1, n-1)
        if test(a, n) == True:
            return Composite

    return Prime

def test(a, n): #NSR Test & Fermat Test로 구성돼있다 두 테스트로 a와 n이 서로소인지 알 수 있다.
    t = 0
    u = n - 1
    while not u & 1:
        t += 1
        u >>= 1
    # n, u은 항상 홀수 
    for i in range(t):
        x_0 = exp(a, u, n)
        if x_0 == 1:
            return False
        for j in range(t):
            if x_0 == n - 1:
                return False
            x_0 = exp(x_0, 2, n)    
    return True


if __name__ == "__main__":

    primes = [7879, 7883, 7901, 7907, 7919, 7927, 7933, 7937, 7949, 7951,
              7963, 7993, 8009, 8011, 8017, 8039, 8053, 8059, 8069, 8081,
              8087, 8089, 8093, 8101, 8111, 8117, 8123, 8147, 8161, 8167]

    for p in primes:
        result = miller_rabin(p, 20)
        if result == Prime:
            print("Prime")
        elif result == Composite:
            print("Composite")
        else:
            print("Undefined")
