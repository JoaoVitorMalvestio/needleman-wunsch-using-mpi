build: a.out

a.out: clean NeedlemanWunschWithMpi.o
	mpicc -g NeedlemanWunschWithMpi.o

mpi.out: clean NeedlemanWunschWithMpi.o
	mpicc -g NeedlemanWunschWithMpi.o

sequecial.out: NeedlemanWunschSequecial.c
	cc NeedlemanWunschSequecial.c -o a.out

NeedlemanWunschWithMpi.o: NeedlemanWunschWithMpi.c
	mpicc -g -c NeedlemanWunschWithMpi.c

clean: 
	rm -f a.out *.o