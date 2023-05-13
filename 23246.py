def prod(t):
    _, c1, c2, c3 = t
    return c1 * c2 * c3

def sum(t):
    _, c1, c2, c3 = t
    return c1 + c2 + c3

def less(c1, c2):
    back1 = c1[0]
    back2 = c2[0]
    return prod(c1) < prod(c2) or \
        (prod(c1) == prod(c2) and sum(c1) < sum(c2)) or \
        (prod(c1) == prod(c2) and sum(c1) == sum(c2) and back1 < back2)

n = int(input().strip())
a = []
for i in range(n):
    back, r1, r2, r3 = [int(x.strip()) for x in input().split()]
    a.append((back, r1, r2, r3))

ans = []
for i in range(3):
    top = a[0]
    for c in a:
        if less(c, top):
            top = c
    ans.append(top[0])
    a.remove(top)
print(' '.join([str(x) for x in ans]))
