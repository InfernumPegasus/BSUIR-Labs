#include <mpi.h>

#include <iostream>
#include <random>
#include <source_location>

#define N 5
#define X 5
#define Y 5

template <typename T, typename U>
void AssertEquals(T expressionResult, U expectedResult,
                  const std::source_location& loc = std::source_location::current()) {
  if (expressionResult != expectedResult) {
    std::cerr << "Assertion failed: " << loc.file_name() << " at line " << loc.line()
              << std::endl;
    std::abort();
  }
}

void FillRandom(int **vec, size_t n, size_t m) {
  static std::random_device rd;
  static std::mt19937 gen(rd());
  static std::uniform_int_distribution<> distrib(1, 255);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      vec[i][j] = distrib(gen);
    }
  }
}

int main(int argc, char** argv) {
  int size, rank, sum = 0, i, j, k;
  int** matrix1 = new int*[X]; // matrix1[X][N]
  int** matrix2 = new int*[N]; // matrix2[N][Y]
  int** mat_res = new int*[X]; // mat_res[X][Y]
  double t1, t2, result;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  for (i = 0; i < X; i++) {
    matrix1[i] = new int[N];
    mat_res[i] = new int[Y];
  }

  for (i = 0; i < N; i++) {
    matrix2[i] = new int[Y];
  }

  FillRandom(matrix1, X, N); // Initialize matrix1 with value 1
  FillRandom(matrix2, N, Y); // Initialize matrix2 with value 2

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) t1 = MPI_Wtime();
  for (i = rank; i < X; i = i + size) { // divide the task in multiple processes
    for (j = 0; j < Y; j++) {
      sum = 0;
      for (k = 0; k < N; k++) {
        sum = sum + matrix1[i][k] * matrix2[k][j];
      }
      mat_res[i][j] = sum;
    }
  }

  if (rank != 0) {
    for (i = rank; i < X; i = i + size)
      MPI_Send(&mat_res[i][0], Y, MPI_INT, 0, 10 + i,
               MPI_COMM_WORLD); // send calculated rows to process with rank 0
  }

  if (rank == 0) {
    for (j = 1; j < size; j++) {
      for (i = j; i < X; i = i + size) {
        MPI_Recv(&mat_res[i][0], Y, MPI_INT, j, 10 + i, MPI_COMM_WORLD,
                 &status); // receive calculated rows from respective process
      }
    }
  }
  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == 0) t2 = MPI_Wtime();
  result = t2 - t1;
  if (rank == 0) {
    for (i = 0; i < X; i++) {
      for (j = 0; j < Y; j++) {
        std::cout << mat_res[i][j] << "\t"; // print the result
      }
      std::cout << std::endl;
    }
  }
  if (rank == 0) std::cout << "Time taken = " << result << " seconds\n"; // time taken

  for (i = 0; i < X; i++) {
    delete[] matrix1[i];
    delete[] mat_res[i];
  }
  delete[] matrix1;
  delete[] mat_res;

  for (i = 0; i < N; i++) {
    delete[] matrix2[i];
  }
  delete[] matrix2;

  MPI_Finalize();
  return 0;
}
