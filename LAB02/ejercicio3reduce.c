#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int local_value;
    int global_sum;

    // Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Asignar un valor local a cada proceso
    local_value = rank + 1;

    // Usar MPI_Reduce para calcular la suma global
    MPI_Reduce(&local_value, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // El proceso 0 tiene la suma global
    if (rank == 0) {
        printf("La suma global es: %d\n", global_sum);
    }

    // Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}
