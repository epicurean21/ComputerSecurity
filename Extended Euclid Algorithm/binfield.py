m = 32

def get_poly_str(f):
    polys = []
    for i, v in enumerate(reversed(bin(f)[2:])):
        if v == '1':
            polys.insert(0,(i,v))
    
    return "+".join(["z^{}".format(i) for i, v in polys])

def carry(a):
    if a & 0x100:
        return True
    else:
        return False

def bin_mul(a, b, n):
    buf = n & 0xff
    t = [0] * 8
    t[0] = a
    for i in range(1,8):
        t[i] = t[i - 1] << 1
        if(carry(t[i])):
            t[i] &= 0xff
            t[i] ^= buf

    res = 0
    for i in range(8):
        mask = 1 << i
        if b & mask != 0:
            res ^= t[i]

    return res

def deg(bp):
    for i in reversed(range(m)):
        if (bp & (1 << i)) != 0:
            return i
    return 0


def bin_ext_euclid(a, b):
    # todo
    u,v = a,b
    g1,g2,h1,h2 = 1,0,0,1

    while u != 0:
        j = deg(u) - deg(v)
        if j < 0:
            u, v = v, u
            g1, g2 = g2, g1
            h1, h2 = h2, h1
            j = -j
        u = u ^ (v << j)
        g1 = g1 ^ (g2 << j)
        h1 = h1 ^ (h2 << j)
    d, g, h = v, g2, h2
    return d, g, h


def bin_inv(a, n):
    d, g, h = bin_ext_euclid(a, n)
    return g

if __name__ == "__main__":
    print("deg(10) = {}".format(deg(10)))
    print(get_poly_str(0b100011011))

    print(get_poly_str(bin_mul(0b01010111,0b10000011,0b100011011)))

    d, g, h = bin_ext_euclid(128, 0b100011011)
    print(d, "|", get_poly_str(g), "|", get_poly_str(h))
    print(get_poly_str(bin_inv(128, 0b100011011)))
    print(get_poly_str(bin_mul(128, 131, 0b100011011)))

