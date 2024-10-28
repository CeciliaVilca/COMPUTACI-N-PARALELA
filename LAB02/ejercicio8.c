/*La ordenación por fusión paralela comienza con las claves n/comm_sz asignadas a cada proceso. 
Termina con todas las claves almacenadas en el proceso 0 en orden ordenado. Para lograr esto, 
utiliza la misma comunicación estructurada en árbol que utilizamos para implementar una suma global. 
Sin embargo, cuando un proceso recibe las claves de otro proceso, fusiona las nuevas claves en su lista 
de claves ya ordenada. Escriba un programa que implemente la ordenación por fusión paralela. El proceso
 0 debe leer n y transmitirlo a los otros procesos. Cada proceso debe utilizar un generador de números 
 aleatorios para crear una lista local de n/comm_sz ints. Luego, cada proceso debe ordenar su lista local
  y el proceso 0 debe recopilar e imprimir las listas locales. Luego, los procesos deben utilizar la 
  comunicación estructurada en árbol para fusionar la lista global en el proceso 0, que imprime el resultado.*/
  
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

void merge(int *left, int left_count, int *right, int right_count, int *result) {
    int i = 0, j = 0, k = 0;

    while (i < left_count && j < right_count) {
        if (left[i] < right[j]) {
            result[k++] = left[i++];
        } else {
            result[k++] = right[j++];
        }
    }

    while (i < left_count) {
        result[k++] = left[i++];
    }

    while (j < right_count) {
        result[k++] = right[j++];
    }
}

void parallel_merge_sort(int *local_array, int local_count) {
    // Ordenar la lista local utilizando qsort
    qsort(local_array, local_count, sizeof(int), (int (*)(const void *, const void *))compare);
}

int compare(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}

int main(int argc, char *argv[]) {
    int rank, size, n, local_count;
    int *local_array = NULL;
    int *global_array = NULL;
    int *sorted_array = NULL;

    // Inicializar el entorno MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Proceso 0 lee el tamaño n y lo transmite a los demás procesos
    if (rank == 0) {
        printf("Ingrese el tamaño total del arreglo (n): ");
        scanf("%d", &n);
    }

    // Broadcast del tamaño n a todos los procesos
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calcular el número de elementos locales por proceso
    local_count = n / size;

    // Asignar memoria para el arreglo local
    local_array = (int *)malloc(local_count * sizeof(int));
    srand(time(NULL) + rank);  // Inicializar la semilla del generador de números aleatorios

    // Generar números aleatorios para el arreglo local
    for (int i = 0; i < local_count; i++) {
        local_array[i] = rand() % 100;  // Números aleatorios entre 0 y 99
    }

    // Ordenar el arreglo local
    parallel_merge_sort(local_array, local_count);

    // Proceso 0 recopila los arreglos locales
    if (rank == 0) {
        global_array = (int *)malloc(n * sizeof(int));
    }

    // Recolectar todos los arreglos locales en el proceso 0
    MPI_Gather(local_array, local_count, MPI_INT, global_array, local_count, MPI_INT, 0, MPI_COMM_WORLD);

    // Proceso 0 imprime los arreglos locales
    if (rank == 0) {
        printf("Arreglos locales ordenados:\n");
        for (int i = 0; i < size; i++) {
            printf("Proceso %d: ", i);
            for (int j = 0; j < local_count; j++) {
                printf("%d ", global_array[i * local_count + j]);
            }
            printf("\n");
        }
    }

    // Fusión en árbol
    int step = 1;
    while (step < size) {
        if (rank % (2 * step) == 0) {
            // Proceso que recibe y fusiona
            int *received_array = (int *)malloc(local_count * sizeof(int) * step);
            MPI_Recv(received_array, local_count * step, MPI_INT, rank + step, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            
            // Fusionar el arreglo local con el recibido
            int *temp_array = (int *)malloc((local_count * (step + 1)) * sizeof(int));
            merge(local_array, local_count, received_array, local_count * step, temp_array);
            free(local_array);
            local_array = temp_array;
            local_count += local_count * step;

            free(received_array);
        } else if (rank % (2 * step) == step) {
            // Proceso que envía
            MPI_Send(local_array, local_count, MPI_INT, rank - step, 0, MPI_COMM_WORLD);
        }
        step *= 2;
    }

    // El proceso 0 imprime el arreglo final ordenado
    if (rank == 0) {
        printf("Arreglo final ordenado:\n");
        for (int i = 0; i < local_count; i++) {
            printf("%d ", local_array[i]);
        }
        printf("\n");
        free(global_array);
    }

    // Liberar memoria
    free(local_array);

    // Finalizar el entorno MPI
    MPI_Finalize();
    return 0;
}