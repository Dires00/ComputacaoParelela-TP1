#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LEN 12

int numberOfDividers(long int number){
    int dividers = 2;
    for (long int i = 2; i <= number/2; i++){
        if(number % i == 0) dividers++;
    }
    return dividers;
}

int main(void){
    FILE *reader = fopen("./entrada.txt", "r");
    FILE *writer = fopen("./saida.txt", "w");
    long int numbers[LEN];
    int i = 0;
    while(!feof(reader)){
        char input[30];
        fgets(input, 30, reader);
        input[strlen(input) - 1] = '\0';
        
        numbers[i] = atoi(input);
        i++;
    }

    
    long int dividers[LEN];
    time_t time1, time2;
    clock_t clock1, clock2;
    time1 = time(0);
    clock1 = (double) clock () / CLOCKS_PER_SEC;
    for(int i = 0; i < LEN; i++){
        dividers[i] = numberOfDividers(numbers[i]);
    }
    time2 = time(0);
    clock2 = (double) clock () / CLOCKS_PER_SEC;
    printf("tempo real: %ld\nTempo de processamento: %f\n", (time2 - time1), (double) (clock2 - clock1));
    // fprintf(writer, "%d\n", numberOfDividers(numbers[i]));

    fclose(reader);
    fclose(writer);

    return 0;
}