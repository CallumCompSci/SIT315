#include <iostream>
#include <cstdlib>
#include <time.h>
#include <chrono>
#include <mpi.h>


using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size)
{
    for (int i = 0; i < size; i++)
    {
        //ToDo: Add Comment
        vector[i] = rand() % 100;
    }
}


int main(int argc, char* argv[]){

    MPI_Init(&argc, &argv);

    
    int rank, num_procs;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);



    unsigned long size = 100000000;

    srand(time(0));

    int *v1_Head, *v2_Head, *v3_Head;
    int *v1_Node, *v2_Node, *v3_Node;

    //ToDo: Add Comment
    auto start = high_resolution_clock::now();

    //Head has to do this part
    if (rank == 0)
    {
        
        v1_Head = (int *) malloc(size * sizeof(int *));
        v2_Head = (int *) malloc(size * sizeof(int *));
        v3_Head = (int *) malloc(size * sizeof(int *));


        randomVector(v1_Head, size);

        randomVector(v2_Head, size);
    }

    unsigned long chunkSize = size / num_procs;
    v1_Node = (int *) malloc(chunkSize * sizeof(int));
    v2_Node = (int *) malloc(chunkSize * sizeof(int));
    v3_Node = (int *) malloc(chunkSize * sizeof(int));



    MPI_Scatter(v1_Head, chunkSize, MPI_INT, v1_Node, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(v2_Head, chunkSize, MPI_INT, v2_Node, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);


    //ToDo: Add Comment
    for (int i = 0; i < chunkSize; i++)
    {
        v3_Node[i] = v1_Node[i] + v2_Node[i];
    }

    long nodeSum = 0;
    for (int i = 0; i < chunkSize; i++)
    {
        nodeSum += v3_Node[i];
    }


    long headSum = 0;
    MPI_Reduce(&nodeSum, &headSum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Gather(v3_Node, chunkSize, MPI_INT, v3_Head, chunkSize, MPI_INT, 0, MPI_COMM_WORLD);


	

    if (rank == 0)
    {
	cout << "Total sum: " << headSum << endl;
        auto stop = high_resolution_clock::now();
        auto duration = duration_cast<microseconds>(stop - start);
        cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

        cout << "Just to check results, first 5 will be printed here: ";
        for (int i = 0; i < 5; i++) {
            cout << v3_Head[i] << " ";
        }
        cout << endl;


    }

    MPI_Finalize();

    return 0;
}
