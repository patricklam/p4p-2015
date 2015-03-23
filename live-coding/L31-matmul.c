/*-------------------------------------------------------------------------
  MPI Matrix Multiply - C Version
  FILE: mpi_mm.c
  OTHER FILES: make.mpi_mm.c
  DESCRIPTION:  
    In this code, the master task distributes a matrix multiply
    operation to numtasks-1 worker tasks.
    NOTE1:  C and Fortran versions of this code differ because of the way
    arrays are stored/passed.  C arrays are row-major order but Fortran
    arrays are column-major order.
  AUTHOR: Ros Leibensperger / Blaise Barney. Converted to MPI: George L. 
    Gusciora (1/25/95)
  LAST REVISED: 12/14/95 Blaise Barney
-------------------------------------------------------------------------*/

#include "mpi.h"               /* required MPI library */
#include <stdio.h>

#define NRA 100                /* number of rows in matrix A */
#define NCA 15                 /* number of columns in matrix A */
#define NCB 7                  /* number of columns in matrix B */
#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          /* setting a message type */

int main(argc,argv)
int argc;
char *argv[];
{
int	numtasks,              /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                 /* message type */
	rows,                  /* rows of matrix A sent to each worker */
	averow, extra, offset, /* determines rows sent to each worker */
	i, j, k, rc;           /* misc */
double	a[NRA][NCA],           /* matrix A to be multiplied */
	b[NCA][NCB],           /* matrix B to be multiplied */
	c[NRA][NCB];           /* result matrix C */

MPI_Status status;

   /*----------------*/
  /* initialize MPI */
 /*----------------*/

   rc = MPI_Init(&argc,&argv);

   /*-----------------------------------*/
  /* get the size of the process group */
 /*-----------------------------------*/

   rc|= MPI_Comm_size(MPI_COMM_WORLD,&numtasks);

   /*---------------------------*/
  /* get the process ID number */
 /*---------------------------*/

   rc|= MPI_Comm_rank(MPI_COMM_WORLD,&taskid); 

   if (rc != 0)
      printf ("error initializing MPI and obtaining task ID info\n");

   numworkers = numtasks-1;

   /*-------------------------------------------------------*/
  /********************** master task **********************/
 /*-------------------------------------------------------*/

   if (taskid == MASTER)
   {
     printf("Number of worker tasks = %d\n",numworkers);
     for (i=0; i<NRA; i++)
       for (j=0; j<NCA; j++)
         a[i][j]= i+j;

     for (i=0; i<NCA; i++)
       for (j=0; j<NCB; j++)
         b[i][j]= i*j;

   /*--------------------------------------*/
  /* send matrix data to the worker tasks */
 /*--------------------------------------*/

     averow = NRA/numworkers;
     extra = NRA%numworkers;
     offset = 0;
     mtype = FROM_MASTER;
     for (dest=1; dest<=numworkers; dest++)
     {
       rows = (dest <= extra) ? averow+1 : averow;   	
       printf("   sending %d rows to task %d\n",rows,dest);

    /*------------------------------------------------------------*/
   /* indicate the offset value at which each process will begin */
  /* looking for data in matrix A                               */
 /*------------------------------------------------------------*/

       MPI_Send(&offset, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);

   /*-------------------------------------------------------------*/
  /* send the number of rows each process is required to compute */
 /*-------------------------------------------------------------*/

       MPI_Send(&rows, 1, MPI_INT, dest, mtype, MPI_COMM_WORLD);

   /*------------------------------------------------------------*/
  /* send each process rows*NCA bits of data starting at offset */
 /*------------------------------------------------------------*/

       MPI_Send(&a[offset][0], rows*NCA, MPI_DOUBLE, dest, mtype,
                MPI_COMM_WORLD);

   /*--------------------------------*/
  /* send each process the matrix B */
 /*--------------------------------*/

       MPI_Send(&b, NCA*NCB, MPI_DOUBLE, dest, mtype, MPI_COMM_WORLD);
         offset = offset + rows;
     }

   /*----------------------------------------*/
  /* wait for results from all worker tasks */
 /*----------------------------------------*/

     mtype = FROM_WORKER;
     for (i=1; i<=numworkers; i++)
     {
       source = i;

   /*---------------------------------------------------------*/
  /* recieve the offset value the sending process ended with */
 /*---------------------------------------------------------*/

       MPI_Recv(&offset, 1, MPI_INT, source, mtype, MPI_COMM_WORLD,                        &status);

   /*---------------------------------------------------------*/
  /* receive the number of rows the sending process computed */
 /*---------------------------------------------------------*/

       MPI_Recv(&rows, 1, MPI_INT, source, mtype, MPI_COMM_WORLD, &status);

    /*------------------------------------------------------*/
   /* receive the final values of matrix C starting at the */
  /* corresponding offset values                          */
 /*------------------------------------------------------*/

       MPI_Recv(&c[offset][0], rows*NCB, MPI_DOUBLE, source, mtype,                        MPI_COMM_WORLD, &status);
     } 

   /*---------------*/
  /* print results */
 /*---------------*/

     printf("Here are the first 30 rows of the result (C) matrix\n");
     for (i=0; i<30; i++)
     {
       printf("\n"); 
       for (j=0; j<NCB; j++) 
         printf("%8.2f  ", c[i][j]);
     }
     printf ("\n");
   }

   /*-------------------------------------------------------*/
  /********************** worker task **********************/
 /*-------------------------------------------------------*/

   if (taskid > MASTER)
   {
     mtype = FROM_MASTER;

    /*------------------------------------------------------------*/
   /* receive the initial offset position of the matrix at which */
  /* I will start                                               */
 /*------------------------------------------------------------*/

     MPI_Recv(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

   /*-----------------------------------------------------*/
  /* receive the number of rows I am required to compute */
 /*-----------------------------------------------------*/

     MPI_Recv(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD, &status);

   /*-----------------------------------------*/
  /* receive the matrix A starting at offset */
 /*-----------------------------------------*/

     MPI_Recv(&a, rows*NCA, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,                   &status);

   /*----------------------*/
  /* receive the matrix B */
 /*----------------------*/

     MPI_Recv(&b, NCA*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD,                    &status);

     for (k=0; k<NCB; k++)
       for (i=0; i<rows; i++)
       {
         c[i][k] = 0.0;
         for (j=0; j<NCA; j++)
           c[i][k] = c[i][k] + a[i][j] * b[j][k];
       }
     mtype = FROM_WORKER;

   /*--------------------------------------------------------------------*/
  /* send the offset value from which point I worked back to the master */
 /*--------------------------------------------------------------------*/

     MPI_Send(&offset, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);

   /*--------------------------------------------------------*/
  /* send the number of rows I worked on back to the master */
 /*--------------------------------------------------------*/

     MPI_Send(&rows, 1, MPI_INT, MASTER, mtype, MPI_COMM_WORLD);

   /*-----------------------------*/
  /* send the final portion of C */
 /*-----------------------------*/

     MPI_Send(&c, rows*NCB, MPI_DOUBLE, MASTER, mtype, MPI_COMM_WORLD);
   }

   /*--------------*/
  /* Finalize MPI */
 /*--------------*/

   MPI_Finalize();
}
