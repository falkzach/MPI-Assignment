#include <mpi.h>   /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

  static const int MAX_MULTIPLYER = 128;
  static const int BYTES = 1024;
  static const int MESSAGE_COUNT = 100;//set to 100 later

  int my_rank;
  int size;
  char hostname[100];
  char debug[100];
  double start_t, end_t, delta_t;
  int i, j;
  MPI_Status stat;

  /*
    Setup MPI
  */
  MPI_Init(&argc, &argv); /*START MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
  MPI_Comm_size(MPI_COMM_WORLD, &size); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/
  gethostname(hostname, 100);
  // sprintf(debug, "host: %s\n", hostname);
  // fputs(debug, stdout);

  for(j = 1;j <= MAX_MULTIPLYER; j++)
  {
    int message_size = BYTES * j;
    char message[message_size]; /* chars are 8 bits each or 1 byte */
    /* Use two processes */
    if (my_rank == 0)
    { 
      // sprintf(debug, "rank 0 is alive\n");
      // fputs(debug, stdout);
      
      for (i=0; i < message_size - 1; i++)
      {
         message[i] = 'A' + random() % 26;
      }
      message[message_size] = (char)0;
      // sprintf(debug, "message of %d bytes is:\n%s\n", message_size, "");
      // fputs(debug, stdout);


      // start timer;
      start_t = MPI_Wtime();
      for (i = 0; i < MESSAGE_COUNT; i++)
      { 
        // send message; 
        // sprintf(debug, "rank 0 sending message %s\n", "");
        // fputs(debug, stdout);
        MPI_Send(
          message,
          strlen(message)+1,
          MPI_BYTE,
          1,
          i,
          MPI_COMM_WORLD);
        // receive message;
        MPI_Recv(
          message,
          sizeof(message),
          MPI_BYTE,
          1,
          i,
          MPI_COMM_WORLD,
          &stat);
        // sprintf(debug, "rank 0 received message %s\n", "");
        // fputs(debug, stdout);
      } 
      // stop timer;
      end_t = MPI_Wtime();
      delta_t = end_t - start_t;
      // print Average message time = elapsed time/(2*MESSAGE_COUNT); 
      sprintf(debug, "%d, %f\n", message_size, (delta_t / (2*MESSAGE_COUNT)));
      fputs(debug, stdout);
    } 
    else /* rank 1 */
    { 
      // sprintf(debug, "rank 1 is alive\n");
      // fputs(debug, stdout);
      for (i = 0; i < MESSAGE_COUNT; i++)
      { 
        // receive message;
        MPI_Recv(
          message,
          sizeof(message),
          MPI_BYTE,
          0,
          i,
          MPI_COMM_WORLD,
          &stat);
        // sprintf(debug, "rank 1 received message %s\n", "");
        // fputs(debug, stdout);
        // send message;
        // sprintf(debug, "rank 1 sending message %s\n", "");
        // fputs(debug, stdout);
        MPI_Send(
          message,
          strlen(message)+1,
          MPI_BYTE,
          0,
          i,
          MPI_COMM_WORLD);
      } 
    }
  }

  MPI_Finalize();/* EXIT MPI */
  return 0;
}
