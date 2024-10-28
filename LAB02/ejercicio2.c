/*Escriba un programa MPI que utilice un método de Monte Carlo para estimar. El proceso 0 debería leer el número total 
de lanzamientos y transmitirlo a los demás procesos. Utilice MPI Reduce para encontrar la suma global de la variable 
local número en el círculo y haga que el proceso 0 imprima el resultado. Es posible que desee utilizar números enteros 
largos para la cantidad de golpes en el círculo y la cantidad de lanzamientos, ya que ambos pueden tener que ser muy grandes 
para obtener una estimación razonable de pi.*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int main(int argc, char *argv[]) {
    int rank, size;
    long long num_tosses; 
    long long local_count = 0;  
    long long global_count;  
    double x, y;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Proceso 0 lee el número total de lanzamientos
    if (rank == 0) {
        printf("Ingrese el número total de lanzamientos: ");
        scanf("%lld", &num_tosses);
    }

    // Difundir el número total de lanzamientos a todos los procesos
    MPI_Bcast(&num_tosses, 1, MPI_LONG_LONG, 0, MPI_COMM_WORLD);

    // Inicializar la semilla para la generación de números aleatorios
    srand(time(NULL) + rank);

    // Realizar los lanzamientos de dardos
    for (long long toss = 0; toss < num_tosses / size; toss++) {
        x = ((double)rand() / RAND_MAX) * 2 - 1; // Genera un número aleatorio entre -1 y 1
        y = ((double)rand() / RAND_MAX) * 2 - 1; // Genera un número aleatorio entre -1 y 1
        if (x * x + y * y <= 1) {
            local_count++; // Contar los lanzamientos que cayeron dentro del círculo
        }
    }

    // Reducir los resultados locales a un resultado global
    MPI_Reduce(&local_count, &global_count, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    // Proceso 0 calcula y muestra la estimación de π
    if (rank == 0) {
        double pi_estimate = 4.0 * global_count / num_tosses;
        printf("Estimación de π: %.10f\n", pi_estimate);
    }

    MPI_Finalize();
    return 0;
}
