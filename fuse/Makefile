clean: cfs
	rm *.o
cfs: cfs.c log.o fswork.o
	gcc -o cfs cfs.c log.o fswork.o -lfuse -D_FILE_OFFSET_BITS=64

log.o: log.c log.h
	gcc -c log.c -o log.o

fswork.o: fswork.c fswork.h
	gcc -c fswork.c -o fswork.o
