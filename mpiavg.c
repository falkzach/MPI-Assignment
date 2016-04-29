#include <mpi.h>   /* PROVIDES THE BASIC MPI DEFINITION AND TYPES */
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

int main(int argc, char **argv) {

  static const char OUTPUT_FILE[] = "avg_output.txt";

  int my_rank;
  int size;
  char hostname[100];

  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;

  int lines = 0;
  int ch = 0;
  double * input;

  int integer_ranks;
  int double_ranks;
  double integer_average = 0.0;
  double double_average = 0.0;

  double local_int_sum = 0.0;
  double local_double_sum = 0.0;
  double global_int_sum;
  double global_double_sum;

  int elements_per_rank;
  int offset;
  int upper_bound;
  int i;

  /* 
  assert input
  */
  if(argc != 4)
  {
    printf("Invalid input arguments!\n%s ./data.txt mpi_ranks_int mpi_ranks_double\n",argv[0]);
    return 1;
  }
  integer_ranks = atoi(argv[2]);
  double_ranks = atoi(argv[3]);
  printf("The file name of the data file that contains the input numbers: %s\n",argv[1]);
  printf("The number of MPI ranks to be used for the integer-precision average computation: %d\n",integer_ranks);
  printf("The number of MPI ranks to be used for the double-precision average computation: %d\n\n",double_ranks);

  /*
    open file
  */
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
      exit(EXIT_FAILURE);
  }
  while(!feof(fp))
  {
    ch = fgetc(fp);
    if(ch == '\n')
    {
      lines++;
    }
  }
  fclose(fp);

  /*
    allocate memmory for the input
  */
  input = malloc(lines * sizeof(double));

  /*
    open file
  */
  fp = fopen(argv[1], "r");
  if (fp == NULL)
  {
      exit(EXIT_FAILURE);
  }
  /*
    read file
  */
  lines = 0;//reset line count
  while ((read = getline(&line, &len, fp)) != -1)
  {
      input[lines] = atof(line);
      lines++;
  }
  fclose(fp);
  if (line)
  {
      free(line);
  }

  /*
    as of here, doubles are stored in the array input
  */



  /*
    Setup MPI
  */
  MPI_Init(&argc, &argv); /*START MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); /*DETERMINE RANK OF THIS PROCESSOR*/
  MPI_Comm_size(MPI_COMM_WORLD, &size); /*DETERMINE TOTAL NUMBER OF PROCESSORS*/

  // TODO: could use additional error checking to ensure ranks add up correctly


    elements_per_rank = (int) ceil((double)lines/(double)size);
    

  if (my_rank < integer_ranks)// do int sum
  {
    offset = my_rank * elements_per_rank;
    //ensure offset within array bounds
    if (offset < lines)
    {
      //ensure an upper bound within the array bounds
      upper_bound = ((offset + elements_per_rank) < lines) ? (offset + elements_per_rank) : lines;
      for (i = offset; i < upper_bound; i++)
      {
        local_int_sum += (int) floor(input[i]);
      }
    }
  }
  else// do double sum
  {
    offset = (my_rank - integer_ranks) * elements_per_rank;
    //ensure offset within array bounds
    if (offset < lines)
    {
      //ensure an upper bound within the array bounds
      upper_bound = ((offset + elements_per_rank) < lines) ? (offset + elements_per_rank) : lines;
      for (i = offset; i < upper_bound; i++)
      {
        local_double_sum += input[i];
      }
    }    
  }

  /*
    do the reductions
  */
  MPI_Reduce(&local_int_sum, &global_int_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  MPI_Reduce(&local_double_sum, &global_double_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);


  if(my_rank == 0)
  {
    /*
      take averages, only need to do this once
    */
    integer_average = global_int_sum / (double)lines;
    double_average = global_double_sum / (double)lines;

    /*
      write the output file!!
      TODO: I noticed when putting the file write after MPI finalize that every thread was still doing it
            Found this curious but put the file writing in the first rank to prevent overwriting the output file.
    */
    fp = fopen(OUTPUT_FILE, "w");
    if (fp == NULL)
    {
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "%f\n", integer_average);
    fprintf(fp, "%f\n", double_average);
    fclose(fp);

    /*
      clean up after yourself
    */
    free(input);
  }

  /* exit MPI */
  MPI_Finalize();
  return 0;
}
