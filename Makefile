all:shm1 shm2

shm1:
	gcc -o shm1 shm1.c
shm2:
	gcc -o shm2 shm2.c

clean:
	rm	shm1 shm2 $(objects1) $(objects2) *~ a.out -rf
