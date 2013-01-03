cfs: cfs.c log.o
	gcc -o cfs cfs.c log.o -lfuse -D_FILE_OFFSET_BITS=64

log.o: log.c log.h
	gcc -c log.c log.h -o log.o
