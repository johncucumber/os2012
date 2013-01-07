import subprocess as su
#import sys
from random import random

print 'Start test'

su.call('fusermount -u td ', shell=True)
su.call('make', shell=True)
#su.call('./createfs', shell=True)
su.call('./cfs td', shell=True)

res = '' 
#fname = sys.argv[1]
dirname = 'td/testdir'
su.call('mkdir ' + dirname, shell=True)
fname = 'td/testdir/test'
su.call('rm ' + fname, shell=True)
f = open(fname, 'w')
for i in xrange(100000):
    s = ''
    for j in xrange(int(random()*200)):
        s += chr(60 + int(random()*30))
    #print i, ':', s
    s += '\n'
    res += s
f.write(res)
f.close
print 'data len', len(res)
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
print 'rm', fname
su.call('rm ' + fname, shell=True)
print 'rmdir', dirname
su.call('rmdir ' + dirname, shell=True)
