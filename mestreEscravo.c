#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN 12

int numberOfDividers(long int number)
{
    int dividers = 2;
    for (long int i = 2; i <= number / 2; i++)
    {
        if (number % i == 0)
            dividers++;
    }
    return dividers;
}

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    MPI_Status status;
    MPI_Group world_group, slaves_group;
    MPI_Comm slaves_comm;
    int rank, n_process;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_process);

    int ranks[n_process - 1];

    for (int i = 0; i < n_process - 1; i++)
    {
        ranks[i] = i + 1;
    }

    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    MPI_Group_incl(world_group, n_process - 1, ranks, &slaves_group);
    MPI_Comm_create(MPI_COMM_WORLD, slaves_group, &slaves_comm);

    long int array[LEN], newArray[LEN / (n_process - 1)];
    /*
    for(int i = 0; i < 99;i++)
       array[i] = i*i; */

    if (rank == 0)
    {
        FILE *reader = fopen("./entrada.txt", "r");
        FILE *writer = fopen("./saida.txt", "w");
        clock_t clock1, clock2;
        long int numbers[LEN], dividers[LEN];
        int i = 0;

        while (!feof(reader))
        {
            char input[30];

            fgets(input, 30, reader);
            input[strlen(input) - 1] = '\0';
            numbers[i] = atoi(input);
            i++;
        }
        fclose(reader);
        clock1 = clock();
        MPI_Send(numbers, LEN, MPI_LONG, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(dividers, LEN, MPI_LONG, 1, 0, MPI_COMM_WORLD, &status);
        printf("Recebi os divisores: %ld\n", dividers[0]);
        clock2 = clock();
        printf("tempo: %f\n", (double) (clock2 - clock1)/CLOCKS_PER_SEC);

        for (int i = 0; i < LEN; i++)
            fprintf(writer, "%ld\n", dividers[i]);
        fclose(writer);
    }
    else
    {
        if (rank == 1)
        {
            MPI_Recv(array, LEN, MPI_LONG, 0, 0, MPI_COMM_WORLD, &status);
        }

        MPI_Scatter(array, LEN / (n_process - 1), MPI_LONG, newArray, LEN / (n_process - 1), MPI_LONG, 0, slaves_comm);

        long int dividers[LEN / (n_process - 1)];

        for (int i = 0; i < LEN / (n_process - 1); i++)
        {
            dividers[i] = numberOfDividers(newArray[i]);
        }
        printf("Acabei de calcular %d\n", rank);
        long int a[] = {0,0,0,0,0,0,0,0,0,0,0,0};
        
        MPI_Barrier(slaves_comm);
        MPI_Gather(dividers, LEN / (n_process - 1), MPI_LONG, a, LEN / (n_process - 1), MPI_LONG, 0, slaves_comm);
        MPI_Barrier(slaves_comm);
        
        if (rank == 1)
        {
            MPI_Send(a, LEN, MPI_LONG, 0, 0, MPI_COMM_WORLD);
        }
    }

    // printf("Eu sou o rank %d\nRecebi %d\n", rank, newArray[0]);

    MPI_Finalize();
}