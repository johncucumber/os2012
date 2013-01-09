
import os
import sys
import subprocess as su
import filecmp

def mount():
    print('Mounting...') 
    su.call('./cfs %s' % test_dir, shell=True)

def umount():
    print('Unmounting...')
    su.call('fusermount -u %s' % test_dir, shell=True)

def waiting():
    su.call('sleep 3', shell=True)

def test_finish():
    print('')
    print('Finishing test')
    print('Waiting for unmount')
    waiting()
    umount()

if __name__ == '__main__':

    test_dir = sys.argv[1] if len(sys.argv) > 1 else 'td'
    if test_dir[-1] != '/': test_dir += '/'
    print('Start test dir %s' % test_dir)
    umount()
    print('Creating fs')
    su.call('./createfs ', shell=True)
    print('Making') 
    su.call('make', shell=True)
    mount()
    print('') 
    print('Making new dir')
    su.call('mkdir %stestdir' % test_dir, shell=True)
    if not os.path.exists(test_dir + 'testdir'):
        print(' >Mkdir failed')
        test_finish()
        exit()
    else:
        print(' >Making dir sucsessfull')

    print('Test cp file')
    test_file_path = '%stestdir/fswork.c' % test_dir
    su.call('cp fswork.c %s' % test_file_path, shell=True)
    if not os.path.exists(test_file_path):
        print(' >cp file failed: file not exists')
        test_finish()
        exit()
    else:
        if not filecmp.cmp('fswork.c', test_file_path):
            print(' >cp file failed: bad data')
            test_finish()
            exit()
        else:
            print(' >cp file sucsessfull. Data valid')
    test_finish()

