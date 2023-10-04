def addTest(a, b, k):
    c = int(a) * int(b)

    with open('./num-multiplication/%s.in' % (k, ), 'w') as f:
        f.write('%s %s' % (a, b))
    with open('./num-multiplication/%s.out' % (k, ), 'w') as f:
        f.write('%s' % (c, ))
    
    print('Generated: %s' % (k, ))

import random

random.seed(47)

i = 1

addTest("0","0", i)
i += 1

addTest("0", "1", i)
i += 1

addTest("2", "3", i)
i += 1

addTest("3", "4", i)
i += 1

addTest("23", "45", i)
i += 1

addTest("00045", "023", i)
i += 1

addTest("36", "23000", i)
i += 1

for j in range(10):
    a = random.randint(0, 10)
    b = random.randint(4, 20)
    addTest(str(a), str(b), i)
    i += 1

for j in range(10):
    a = random.randint(10**4, 10**5 - 1)
    b = random.randint(4, 20)

    a = '0' * random.randint(0, 10) + str(a)
    b = '0' * random.randint(0, 5) + str(b)

    addTest(a, b, i)
    i += 1

for j in range(73):
    a = random.randint(0, 10**(1000))
    b = random.randint(0, 10**(1000))

    addTest(str(a), str(b), i)
    i += 1
