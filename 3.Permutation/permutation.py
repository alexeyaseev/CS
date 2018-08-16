"""
Steinhaus–Johnson–Trotter algorithm for generating permutations
by swapping only adjacent items

16 august 2018
algorithm
size: <40,1000000>, time <1.39>
size: <80,1000000>, time <1.52>
size: <8000,50000>, time <1.41>
size: <800000,100>, time <1.08>

itertools.permutations
size: <40,1000000>, time <0.36>
size: <80,1000000>, time <0.50>
size: <8000,50000>, time <1.39>
size: <800000,100>, time <1.18>
"""


def Permutation0(n):
    P  = list(range(n+1)) #last item is sentinel
    Pi = list(range(n))
    d  = [-1] * (n)
    active = list(range(1,n))
    while active:
        yield P[:-1]
        m = active[-1]
        j = Pi[m]
        P[j], P[j + d[m]] = P[j + d[m]], m
        Pi[m], Pi[P[j]] = j + d[m], j
        if m < P[j + 2*d[m]]:
            d[m] = -d[m]
            active.pop()
        active.extend(range(m+1,n))
    yield P[:-1]

def Permutation1(n):
    from itertools import permutations
    for p in permutations(range(n)):
        yield p


if __name__ == "__main__":
    import time
    tests = ((40,1000000),(80,1000000),(8000,50000),(800000,100))
    for size,maxn in tests:
        times = []
        for runs in range(5):
            Sol = Permutation0(size)
            temp = next(Sol)
            start = time.time()
            counter = 0
            for p in Sol:
                #print(p)
                counter += 1
                if counter == maxn: break
            stop = time.time()
            times.append(stop - start)
        _time = sum(times) / len(times)
        print("size: <{},{}>, time <{:.2f}>".format(size, maxn, _time))
