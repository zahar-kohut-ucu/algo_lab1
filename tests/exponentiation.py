def addTest(a, n, m, k):
    res = (a ** n) % m

    with open('./exponentiation/%s.in' % (k, ), 'w') as f:
        f.write('%s %s %s' % (a, n, m))
    with open('./exponentiation/%s.out' % (k, ), 'w') as f:
        f.write('%s' % (res, ))
    
    print('Generated: %s' % (k, ))

import random

random.seed(47)

i = 1

addTest(0,0,2, i)
i += 1

addTest(0,0,5, i)
i += 1

addTest(1, 0, 2, i)
i += 1

addTest(1,0,10, i)
i += 1

addTest(1,1,5, i)
i += 1

addTest(2,1,10, i)
i += 1

for j in range(10):
    a = random.randint(0, 10)
    b = random.randint(4, 20)
    c = random.randint(2, 100)
    addTest(a,b,c, i)
    i += 1

for j in range(10):
    a = random.randint(0,10)
    b = random.randint(100,200)
    c = random.randint(2, 100)
    addTest(a,b,c, i)
    i += 1

for j in range(74):
    a = random.randint(0,10**9)
    b = random.randint(0,10**5)
    c = random.randint(0,10**9)

    addTest(a,b,c,i)
    i += 1
