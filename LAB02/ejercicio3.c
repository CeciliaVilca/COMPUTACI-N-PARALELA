/*Escriba un programa MPI que calcule una suma global estructurada en árbol. Primero escriba su programa para el 
caso especial en el que comm_sz es una potencia de dos. Luego, una vez que haya logrado que esta versión funcione,
 modifique su programa para que pueda manejar cualquier comm_sz.*/
 
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int local_value, global_sum = 0;

    // Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Asignar un valor local a cada proceso
    local_value = rank + 1;

    // Suma global en forma de árbol
    int step;
    for (step = 1; step < size; step *= 2) {
        if (rank % (2 * step) == 0) {
            int received_value;
            // Verificar que el proceso hijo exista
            if (rank + step < size) {
                MPI_Recv(&received_value, 1, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                local_value += received_value;
            }
        } else if (rank % (2 * step) == step) {
            MPI_Send(&local_value, 1, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        }
    }

    // Todos los procesos finales se sincronizan en una barrera
    MPI_Barrier(MPI_COMM_WORLD);

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        global_sum = local_value;
        printf("La suma global es: %d\n", global_sum);
    }

    // Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}