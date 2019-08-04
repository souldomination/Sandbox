from sys import stdin
from fractions import Fraction
input = lambda: stdin.readline().strip()

MOD = 1000000007

class Sphere:
    __slots__ = ['x', 'r']
    def __init__(self, x=0, r=0):
        self.x = x
        self.r = r
    def __lt__(self, other) -> bool:
        if self.x + self.r != other.x + other.r:
            return self.x + self.r < other.x + other.r
        return self.r > other.r

class Intersec:
    __slots__ = ['x', 'prev', 'next']
    def __init__(self, x=0, prev=-1, next=-1):
        self.x = x
        self.prev = prev
        self.next = next

n = int(input())
sph = [None for i in range(n)]
stk = [None for i in range(2*n)]
sz = 0

def is_intersec_in_sphere(p: Intersec, s: Sphere) -> bool:
    if p.prev != -1:
        x0 = sph[p.prev].x
        r0 = sph[p.prev].r
    else:
        x0 = sph[p.next].x
        r0 = sph[p.next].r

    ysq = r0**2 - (p.x-x0)**2
    return (s.x-p.x)**2 + ysq <= s.r**2

def sphere_intersec_x(a: Sphere, b: Sphere) -> Fraction:
    if a.x > b.x:
        a, b = b, a
    d = b.x - a.x
    return a.x + Fraction(a.r**2 + d**2 - b.r**2, 2*d)

def integrate_sphere(s: Sphere, a: Fraction, b: Fraction) -> Fraction:
    return -Fraction(1, 3)*(b**3-a**3) + s.x*(b**2-a**2) + (s.r**2-s.x**2)*(b-a)

def powmod(a: int, b: int, m: int) -> int:
    if b == 0:
        return 1
    half = powmod(a, b//2, m)
    if b%2 == 0:
        return half*half%m
    return half*half%m*a%m

for i in range(n):
    sph[i] = Sphere(*map(int, input().split()))

sph.sort()

stk[0] = Intersec(sph[0].x - sph[0].r, -1, 0)
stk[1] = Intersec(sph[0].x + sph[0].r, 0, -1)
sz = 2

for i in range(1, n):
    if sph[i-1].x + sph[i-1].r == sph[i].x + sph[i].r:
        continue
    if is_intersec_in_sphere(stk[0], sph[i]):
        stk[0] = Intersec(sph[i].x - sph[i].r, -1, i)
        stk[1] = Intersec(sph[i].x + sph[i].r, i, -1)
        sz = 2
        continue
    if not is_intersec_in_sphere(stk[sz-1], sph[i]):
        stk[sz] = Intersec(sph[i].x - sph[i].r, -1, i)
        stk[sz+1] = Intersec(sph[i].x + sph[i].r, i, -1)
        sz += 2
        continue

    low = 0
    high = sz-1
    while high - low > 1:
        mid = (low + high) // 2
        if is_intersec_in_sphere(stk[mid], sph[i]):
            high = mid
        else:
            low = mid

    sz = high
    if stk[sz-1].next != -1:
        stk[sz] = Intersec(sphere_intersec_x(sph[stk[sz-1].next], sph[i]),
                           stk[sz-1].next, i)
    else:
        stk[sz] = Intersec(sph[i].x - sph[i].r, -1, i)
    stk[sz+1] = Intersec(sph[i].x + sph[i].r, i, -1)
    sz += 2

res: Fraction = 0
for i in range(sz-1):
    if stk[i].next == -1:
        continue
    res += integrate_sphere(sph[stk[i].next], stk[i].x, stk[i+1].x)
print(res.numerator*powmod(res.denominator, MOD-2, MOD)%MOD)
