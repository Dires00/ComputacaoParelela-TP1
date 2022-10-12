sincrono:
	gcc sincrono.c -o a
	
runSincrono:
	gcc sincrono.c -o a
	./a
	rm a

async:
	mpicc mestreEscravo.c -o a

runAsync:
	mpicc mestreEscravo.c -o a
	mpirun -np 4 ./a
	rm a