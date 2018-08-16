"""
Steinhaus–Johnson–Trotter algorithm for generating permutations
by swapping only adjacent items

15 august 2018
size: <40>, time <0.34>
size: <80>, time <0.39>
size: <800>, time <0.88>
"""



def Permutation(n):
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

if __name__ == "__main__":
    import time
    tests = ((40,300000),(80,300000),(800,300000))
    for size,maxn in tests:
        Sol = Permutation(size)
        temp = next(Sol)
        start = time.time()
        counter = 0
        for p in Sol:
            #print(p)
            counter += 1
            if counter == maxn: break
        stop = time.time()
        print("size: <{}>, time <{:.2f}>".format(size, stop - start))
