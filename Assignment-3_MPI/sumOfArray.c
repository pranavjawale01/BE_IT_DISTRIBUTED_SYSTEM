// Sum of an array using MPI
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
int a[N] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

int main(int argc, char* argv[]) {
    int pid, np, elements_per_process, n_elements_received;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    if (np > N) {
        if (pid == 0)
            printf("Number of processes should not exceed %d\n", N);
        MPI_Finalize();
        return 1;
    }

    if (pid == 0) {
        int index = 0, i;
        int sum = 0;
        elements_per_process = N / np;
        int elements_left = N;

        // Distribute parts of array to processes
        for (i = 1; i < np; i++) {
            int count = (i == np - 1) ? elements_left - elements_per_process * (np - 1) : elements_per_process;
            MPI_Send(&count, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&a[elements_per_process * i], count, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Sum own elements
        for (i = 0; i < elements_per_process; i++) {
            sum += a[i];
        }

        int tmp;
        for (i = 1; i < np; i++) {
            MPI_Recv(&tmp, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            sum += tmp;
        }

        printf("Sum of array is: %d\n", sum);
    } else {
        MPI_Recv(&n_elements_received, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int* a2 = (int*)malloc(sizeof(int) * n_elements_received);
        if (!a2) {
            fprintf(stderr, "Memory allocation failed at rank %d\n", pid);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        MPI_Recv(a2, n_elements_received, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        int partial_sum = 0;
        for (int i = 0; i < n_elements_received; i++)
            partial_sum += a2[i];

        MPI_Send(&partial_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        free(a2);
    }

    MPI_Finalize();
    return 0;
}