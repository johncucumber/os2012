import subprocess as su
from random import random

print 'Start test'

su.call('fusermount -u td ', shell=True)
su.call('make', shell=True)
su.call('./createfs', shell=True)
su.call('./cfs td', shell=True)

res = '' 
fname = 'td/ptest'
f = open(fname, 'w')
for i in xrange(100):
    s = ''
    for j in xrange(int(random()*100)):
        s += chr(60 + int(random()*10))
    print i, ':', s
    s += '\n'
    res += s
f.write(res)
f.close
su.call('sleep 3', shell=True)
f = open(fname, 'r')
s = f.read()
if s != res:
    print 'Test failed'
    exit()
f.close()
print 'data valid'

su.call('sleep 3', shell=True)
print 'unmount...'
su.call('fusermount -u td ', shell=True)
su.call('./cfs td', shell=True)
print 'test again'
f = open(fname, 'r')
s = f.read()
if s != res:
    print 'Test failed'
    exit()
print 'data valid'
f.close()
print 'Test passed'
