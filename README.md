#MPI Assignment

##mpiavg.c

###Compile 
  mpicc -o mpiavg -lm mpiavg.c

###Execute
  mpirun -np 16 ./mpiavg data.txt 7 9

##pingpong.c

###Compile
  mpicc -o pingpong pingpong.c 

###Execute
  mpirun -np 2 ./pingpong