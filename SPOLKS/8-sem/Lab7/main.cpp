#include <mpi.h>

#include <iomanip>
#include <iostream>

#include "Matrix.h"

template <size_t Size>
constexpr double PerformMatrixMultiplication(const Matrix<Size>& A, const Matrix<Size>& B,
                                             Matrix<Size>& C, const int rank,
                                             const int processors,
                                             const bool synchronous) {
  const int from = rank * Size / processors;
  const int to = (rank + 1) * Size / processors;
  constexpr auto SizeSquare = Size * Size;

  const auto start = MPI_Wtime();

  if (synchronous) {
    MPI_Bcast(B.PlainArray(), SizeSquare, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(A.PlainArray(), SizeSquare / processors, MPI_INT, A[from],
                SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD);
  } else {
    MPI_Request request[2];
    MPI_Status status[2];

    MPI_Ibcast(B.PlainArray(), SizeSquare, MPI_INT, 0, MPI_COMM_WORLD, &request[0]);
    MPI_Iscatter(A.PlainArray(), SizeSquare / processors, MPI_INT, A[from],
                 SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD, &request[1]);

    MPI_Waitall(2, request, status);
  }

  for (int i = from; i < to; i++) {
    for (size_t j = 0; j < Size; j++) {
      C[i][j] = 0;
      for (size_t k = 0; k < Size; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  MPI_Gather(C.PlainArray()[from], SizeSquare / processors, MPI_INT, C.PlainArray(),
             SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD);

  double totalTime = 0.0;
  if (rank == 0) {
    totalTime = MPI_Wtime() - start;
    std::cout << "Result:\n";
    C.Print();
  }

  return totalTime;
}

int main(int argc, char* argv[]) {
  constexpr auto MATRIX_SIZE = 3;

  Matrix<MATRIX_SIZE> A{};
  Matrix<MATRIX_SIZE> B{};
  Matrix<MATRIX_SIZE> C_sync{};
  Matrix<MATRIX_SIZE> C_async{};

  A.Randomize();
  B.Randomize();

  int rank, size;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* who am i */
  MPI_Comm_size(MPI_COMM_WORLD, &size); /* number of processors */

  if (rank == 0) {
    std::cout << "A:\n";
    A.Print();
    std::cout << "B:\n";
    B.Print();
  }

  if (MATRIX_SIZE % size != 0) {
    if (rank == 0) printf("Matrix size not divisible by number of size\n");
    MPI_Finalize();
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  // Calculate synchronized matrix multiplication
  double timeSync =
      PerformMatrixMultiplication<MATRIX_SIZE>(A, B, C_sync, rank, size, true);

  // Calculate asynchronous matrix multiplication
  double timeAsync =
      PerformMatrixMultiplication<MATRIX_SIZE>(A, B, C_async, rank, size, false);

  MPI_Finalize();

  if (rank == 0) {
    std::cout << "\nTotal time (CalculateSynchronized): " << std::fixed
              << std::setprecision(6) << timeSync << " (sec)\n";
    std::cout << "Total time (CalculateAsynchronous): " << std::fixed
              << std::setprecision(6) << timeAsync << " (sec)\n";

    const auto equal = C_sync == C_async;
    std::cout << "Matrices equal: " << std::boolalpha << equal << std::noboolalpha
              << "\n\n";

    const auto syncToAsyncTimeCoefficient = timeSync / timeAsync;
    std::cout << "Sync mode is slower then Async by " << syncToAsyncTimeCoefficient
              << " times\n";
  }

  return 0;
}
