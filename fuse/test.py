
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
    print('subdir test')
    su.call('mkdir %stestdir/d2' % test_dir, shell=True)
    if not os.path.exists(test_dir + 'testdir/d2'):
        print(' >Mk subdir failed')
        test_finish()
        exit()
    else:
        print(' >Making subdir sucsessfull')

    print('mv test')
    new_test_file_path = '%stestdir/d2/mvfile' % test_dir
    su.call('mv %s %s' % (test_file_path, new_test_file_path), shell=True)
    if os.path.exists(test_file_path):
        print(' >mv file failed: old file exists')
        test_finish()
        exit()
    else:
        if not os.path.exists(new_test_file_path):
            print(' >mv file failed: file not exists')
            test_finish()
            exit()
        else:
            if not filecmp.cmp('fswork.c', new_test_file_path):
                print(' >mv file failed: bad data')
                test_finish()
                exit()
            else:
                print(' >mv file sucsessfull. Data valid')

    print('Unmount test')
    print('Waiting for unmount')
    waiting()
    umount()
    mount()
    if os.path.exists(test_file_path):
        print(' >unmount  failed: old file exists')
        test_finish()
        exit()
    else:
        if not os.path.exists(new_test_file_path):
            print(' >unmount failed: file not exists')
            test_finish()
            exit()
        else:
            if not filecmp.cmp('fswork.c', new_test_file_path):
                print(' >unmount failed: bad data')
                test_finish()
                exit()
            else:
                print(' >unmount sucsessfull. Data valid')


    print('many files cp test')
    for i in xrange(40):
        tmp_test_file_path = new_test_file_path + str(i)
        su.call('cp %s %s' % (new_test_file_path, tmp_test_file_path), shell=True)
        if not os.path.exists(tmp_test_file_path):
            print(' >cp file failed: file not exists')
            test_finish()
            exit()
        else:
            if not filecmp.cmp(new_test_file_path, tmp_test_file_path):
                print(' >cp file failed: bad data')
                test_finish()
                exit()
    print(' >many cp test passed!') 
    print('Unmount test2')
    print('Waiting for unmount')
    waiting()
    umount()
    mount()
    for i in xrange(40):
        tmp_test_file_path = new_test_file_path + str(i)
        if not os.path.exists(tmp_test_file_path):
            print(' >cp file failed: file not exists')
            test_finish()
            exit()
        else:
            if not filecmp.cmp(new_test_file_path, tmp_test_file_path):
                print(' >cp file failed: bad data')
                test_finish()
                exit()
    print(' >Unmount 2 test passed!') 
    print('\nAll tests passed!')
    test_finish()

