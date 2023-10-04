import random

import numpy as np

def addTest(m,n,k,idx):
    a = np.zeros((m, n), dtype=int)
    b = np.zeros((n, k), dtype=int)

    for i in range(m):
        for j in range(n):
            a[i,j] = random.randint(0, 100)
    for i in range(n):
        for j in range(k):
            b[i,j] = random.randint(0, 100)
    
    c = a.dot(b)

    with open('./matrix-multiplication/%s.in' % (idx, ), 'w') as f:
        f.write("%s %s\n" % (m, n))
        for i in range(m):
            for j in range(n):
                f.write('%s ' % (a[i,j], ))
            f.write('\n')

        f.write("%s %s\n" % (n, k))
        for i in range(n):
            for j in range(k):
                f.write('%s ' % (b[i,j], ))
            f.write('\n')
        
    with open('./matrix-multiplication/%s.out' % (idx, ), 'w') as f:
        for i in range(m):
            for j in range(k):
                f.write('%s ' % (c[i,j], ))
            f.write('\n')
    
    print('Generated: %s' % (idx, ))

random.seed(47)

i = 1

addTest(1, 1, 1, i)
i += 1

addTest(2, 2, 2, i)
i += 1

addTest(8, 8, 8, i)
i += 1

addTest(16, 8, 5, i)
i += 1

for _ in range(94):
    m = random.randint(64, 128)
    n = random.randint(64, 128)
    k = random.randint(64, 128)

    addTest(m, n, k, i)
    i += 1

addTest(512, 128, 256, i)
i += 1

addTest(512, 512, 512, i)
i += 1
