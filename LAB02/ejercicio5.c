/*Implemente la multiplicación de matriz-vector utilizando una distribución de bloques y columnas de la matriz. 
Puede hacer que el proceso 0 lea la matriz y simplemente utilizar un bucle de envíos para distribuirla entre los
 procesos. Suponga que la matriz es cuadrada de orden n y que n es divisible de manera uniforme por comm_sz. Es 
 posible que desee consultar la función MPI_Reduce_scatter.*/

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void read_matrix(int n, double *matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i * n + j] = i + j;  
        }
    }
}

void read_vector(int n, double *vector) {
    for (int i = 0; i < n; i++) {
        vector[i] = i + 1;  
    }
}

void print_result(int n, double *result) {
    printf("Resultado de la multiplicación:\n");
    for (int i = 0; i < n; i++) {
        printf("%f ", result[i]);
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    int rank, size, n = 4;  // Orden de la matriz
    double *matrix = NULL;
    double *vector = NULL;
    double *local_matrix = NULL;
    double *local_vector = NULL;
    double *local_result = NULL;
    double *result = NULL;

    // Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Asegurarse de que n sea divisible por el tamaño del comunicador
    if (n % size != 0) {
        if (rank == 0) {
            fprintf(stderr, "El orden de la matriz debe ser divisible por el número de procesos.\n");
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    // Proceso 0 lee la matriz y el vector
    if (rank == 0) {
        matrix = (double *)malloc(n * n * sizeof(double));
        vector = (double *)malloc(n * sizeof(double));
        read_matrix(n, matrix);
        read_vector(n, vector);
    }

    // Cada proceso recibe su bloque de columnas de la matriz
    int rows_per_process = n / size;
    local_matrix = (double *)malloc(rows_per_process * n * sizeof(double));
    local_vector = (double *)malloc(n * sizeof(double));
    local_result = (double *)malloc(rows_per_process * sizeof(double));

    // Distribuir columnas de la matriz y el vector
    MPI_Scatter(matrix, rows_per_process * n, MPI_DOUBLE, local_matrix, rows_per_process * n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(vector, n, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Multiplicar el bloque local de la matriz por el vector
    for (int i = 0; i < rows_per_process; i++) {
        local_result[i] = 0;
        for (int j = 0; j < n; j++) {
            local_result[i] += local_matrix[i * n + j] * vector[j];
        }
    }

    // Recolectar los resultados en el proceso 0
    if (rank == 0) {
        result = (double *)malloc(n * sizeof(double));
    }

    MPI_Gather(local_result, rows_per_process, MPI_DOUBLE, result, rows_per_process, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Proceso 0 imprime el resultado
    if (rank == 0) {
        print_result(n, result);
        free(matrix);
        free(vector);
        free(result);
    }

    // Liberar memoria
    free(local_matrix);
    free(local_vector);
    free(local_result);

    // Finalizar el entorno MPI
    MPI_Finalize();
    return EXIT_SUCCESS;
}