// Intermediate Sums at Different Processors
#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    int rank, size;
    int num[20];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 4) {
        if (rank == 0)
            printf("This program requires exactly 4 MPI processes.\n");
        MPI_Finalize();
        return 1;
    }

    // Initialize array
    for (int i = 0; i < 20; i++) {
        num[i] = i + 1;
    }

    if (rank == 0) {
        printf("Distribution at rank %d\n", rank);

        // Send 5 elements to each of ranks 1–3
        for (int i = 1; i < 4; i++) {
            MPI_Send(&num[i * 5], 5, MPI_INT, i, 1, MPI_COMM_WORLD);
        }

        // Local sum for rank 0
        int local_sum = 0;
        for (int i = 0; i < 5; i++) {
            local_sum += num[i];
        }
        printf("Local sum at rank %d is %d\n", rank, local_sum);

        int sum = local_sum, received_sum;

        // Receive partial sums from ranks 1–3
        for (int i = 1; i < 4; i++) {
            MPI_Recv(&received_sum, 1, MPI_INT, i, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received sum from rank %d: %d\n", i, received_sum);
            sum += received_sum;
        }

        printf("Final sum = %d\n", sum);
    } else {
        int k[5];
        MPI_Recv(k, 5, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int local_sum = 0;
        for (int i = 0; i < 5; i++) {
            local_sum += k[i];
        }

        printf("Local sum at rank %d is %d\n", rank, local_sum);
        MPI_Send(&local_sum, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}