#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN 1440

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
    int rank, n_process;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_process);

    int lenPerProcess = LEN / n_process;
    long int array[LEN], newArray[lenPerProcess];
    /*
    for(int i = 0; i < 99;i++)
       array[i] = i*i; */

    clock_t clock1, clock2;
    long int numbers[LEN];

    if (rank == 0)
    {   
        FILE *reader = fopen("./entrada.txt", "r");
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

        printf("inicio dos numeros %ld\n", numbers[0]);
    }

    MPI_Scatter(numbers, lenPerProcess, MPI_LONG, newArray, lenPerProcess, MPI_LONG, 0, MPI_COMM_WORLD);

    long int dividers[lenPerProcess];
    printf("Rank %d vai calcular %ld\n", rank, newArray[0]);
    for (int i = 0; i < lenPerProcess; i++)
    {
        if(i % (80/5) == 0){
            printf("Rank %d calculou %f%%\n", rank, 100.0*(float)i/((float)lenPerProcess));
        }
        dividers[i] = numberOfDividers(newArray[i]);
    }
    printf("Acabei de calcular %d\n", rank);
    long int allDividers[LEN];
    
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Gather(dividers, lenPerProcess, MPI_LONG, allDividers, lenPerProcess, MPI_LONG, 0, MPI_COMM_WORLD);
    if (rank == 0){
        printf("Recebi os divisores: %ld\n", allDividers[0]);
    }

    if(rank == 0){
        clock2 = clock();
        FILE *writer = fopen("./saida.txt", "w");
        for (int i = 0; i < LEN; i++)
            fprintf(writer, "%ld\n", allDividers[i]);
        fclose(writer);
        printf("tempo: %f\n", (double) (clock2 - clock1)/CLOCKS_PER_SEC);

    }

    MPI_Finalize();
}