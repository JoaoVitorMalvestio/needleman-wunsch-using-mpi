build: a.out

a.out: clean NeedlemanWunschWithMpi.o
	mpicc -g NeedlemanWunschWithMpi.o

mpi.out: clean NeedlemanWunschWithMpi.o
	mpicc -g NeedlemanWunschWithMpi.o

sequecial.out: clean NeedlemanWunschSequecial.o
	cc -g NeedlemanWunschSequecial.o

NeedlemanWunschSequecial.o: NeedlemanWunschSequecial.c
	mpicc NeedlemanWunschSequecial.c -o NeedlemanWunschSequecial.o

NeedlemanWunschWithMpi.o: NeedlemanWunschWithMpi.c
	mpicc -g -c NeedlemanWunschWithMpi.c

clean: 
	rm -f a.out *.o