#include <mpi.h>   /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {

  static const int MAX_MULTIPLYER = 128;
  static const int BYTES = 1024;
  static const int MESSAGE_COUNT = 3;//set to 100 later

  int my_rank;
  int size;
  char hostname[100];
  double start_t, end_t, delta_t;
  int i;
  MPI_Status stat;

  /* 
  assert input
  */
  // if(argc != 1)
  // {
  //   printf("Invalid input arguments!\n%s ./data.txt mpi_ranks_int mpi_ranks_double\n",argv[0]);
  //   return 1;
  // }



  /*
    Setup MPI
  */
  MPI_Init(&argc, &argv); /*START MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
  MPI_Comm_size(MPI_COMM_WORLD, &size); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/

  int message_size = BYTES;
  char message[message_size]; /* chars are 8 bits each or 1 byte */
  /* Use two processes */
  if (my_rank == 0)
  { 
    printf("ranek 0 is alive\n");
    
    for (i=0; i < message_size - 1; i++)
    {
       message[i] = 'A' + random() % 26;
    }
    message[message_size] = (char)0;
    printf("message of %d bytes is:\n%s\n", message_size, message);


    // start timer;
    start_t = MPI_Wtime();
    for (i = 0; i < MESSAGE_COUNT; i++)
    { 
      // send message; 
      printf("rank 0 sending message %s\n", message);
      MPI_Send(
        message,
        strlen(message)+1,
        MPI_BYTE,
        1,
        1,
        MPI_COMM_WORLD);
      // receive message;
      MPI_Recv(
        message,
        sizeof(message),
        MPI_BYTE,
        1,
        1,
        MPI_COMM_WORLD,
        &stat);
      printf("rank 0 received message %s\n", message);
    } 
    // stop timer;
    end_t = MPI_Wtime();
    delta_t = MPI_Wtime();
    // print Average message time = elapsed time/(2*MESSAGE_COUNT); 
    printf("Average message time = %f seconds\n", (delta_t / (2*MESSAGE_COUNT)));
  } 
  else /* rank 1 */
  { 
    printf("ranek 1 is alive\n");
    for (i = 0; i < MESSAGE_COUNT; i++)
    { 
      // receive message;
      MPI_Recv(
        message,
        sizeof(message),
        MPI_BYTE,
        0,
        1,
        MPI_COMM_WORLD,
        &stat);
      printf("rank 1 received message %s\n", message);
      // send message;
      printf("rank 1 sending message %s\n", message);
      MPI_Send(
        message,
        strlen(message)+1,
        MPI_BYTE,
        0,
        1,
        MPI_COMM_WORLD);
    } 
  } 


  /*
    ping pong game
  */

  MPI_Finalize();/* EXIT MPI */
  return 0;
}
