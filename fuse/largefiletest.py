import filecmp

test_dir = 'td/'
print filecmp.cmp('/home/kirill/m.tar.gz', test_dir + 'm.tar.gz')
