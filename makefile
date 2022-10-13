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
	mpirun -np 18 --hostfile ./maquinas ./a
	rm a

runCluster:
	mpicc mestreEscravo.c -o a
	../../openmpi/bin/mpirun --prefix /home/192050027/192050027/openmpi/ -n 18 --machinefile ./maquinas ./a