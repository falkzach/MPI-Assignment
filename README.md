#MPI Assignment

##Compile 
 mpicc -o mpiavg -lm mpiavg.c

##Execute
 mpirun -np 16 ./mpiavg data.txt 7 9
 