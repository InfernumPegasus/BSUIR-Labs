#include <mpi.h>

#include <cstdio>

#include "Matrix.h"

template <size_t Size>
constexpr void CalculateSynchronized(const Matrix<Size>& A, const Matrix<Size>& B,
                                     Matrix<Size>& C, const int rank,
                                     const int processors, double& totalTime) {
  const int from = rank * Size / processors;
  const int to = (rank + 1) * Size / processors;
  constexpr auto SizeSquare = Size * Size;

  const auto start = MPI_Wtime();

  MPI_Bcast(B.PlainArray(), SizeSquare, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(A.PlainArray(), SizeSquare / processors, MPI_INT, A[from],
              SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD);

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

  if (rank == 0) {
    std::cout << "Result:\n";
    C.Print();
    //    const auto result = MPI_Wtime() - start;
    //    std::cout << "Total time (CalculateSynchronized): " << std::fixed
    //              << std::setprecision(6) << result << " (sec)\n";

    totalTime = MPI_Wtime() - start;
  }
}

template <size_t Size>
constexpr void CalculateAsynchronous(const Matrix<Size>& A, const Matrix<Size>& B,
                                     Matrix<Size>& C, const int rank,
                                     const int processors, double& totalTime) {
  const int from = rank * Size / processors;
  const int to = (rank + 1) * Size / processors;
  constexpr auto SizeSquare = Size * Size;

  const auto start = MPI_Wtime();

  /***
   * Size 2 in MPI_Request request[2] and MPI_Status status[2] to create arrays for
   * storing the MPI request handles and status objects, respectively.
   * The reason for using a size of 2 is that we have two non-blocking communication
   * operations: one for the broadcast (MPI_Ibcast) and one for the scatter (MPI_Iscatter)
   * Each of these operations requires a separate request handle and status object to
   * manage the asynchronous communication.
   */
  MPI_Request request[2];
  MPI_Status status[2];

  // Non-blocking broadcast of matrix B from process 0 to all other processes
  MPI_Ibcast(B.PlainArray(), SizeSquare, MPI_INT, 0, MPI_COMM_WORLD, &request[0]);

  // Non-blocking scatter of matrix A to all processes
  MPI_Iscatter(A.PlainArray(), SizeSquare / processors, MPI_INT, A[from],
               SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD, &request[1]);

  // Wait for both communication operations to complete
  MPI_Waitall(2, request, status);

  // Perform matrix multiplication
  for (int i = from; i < to; i++) {
    for (size_t j = 0; j < Size; j++) {
      C[i][j] = 0;
      for (size_t k = 0; k < Size; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  // Gather the result from all processes to process 0
  MPI_Gather(C.PlainArray()[from], SizeSquare / processors, MPI_INT, C.PlainArray(),
             SizeSquare / processors, MPI_INT, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    std::cout << "Result:\n";
    C.Print();
    //    const auto result = MPI_Wtime() - start;
    //    std::cout << "Total time (CalculateAsynchronous): " << std::fixed
    //              << std::setprecision(6) << result << " (sec)\n";

    totalTime = MPI_Wtime() - start;
  }
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

  double timeSync{}, timeAsync{};

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* who am i */
  MPI_Comm_size(MPI_COMM_WORLD, &size); /* number of processors */

  if (rank == 0) {
    std::cout << "A:\n";
    A.Print();
    std::cout << "B:\n";
    B.Print();
  }

  /* Just to use the simple variants of MPI_Gather and MPI_Scatter we */
  /* impose that MATRIX_SIZE is divisible by size. By using the vector versions, */
  /* (MPI_Gatherv and MPI_Scatterv) it is easy to drop this restriction. */
  if (MATRIX_SIZE % size != 0) {
    if (rank == 0) printf("Matrix size not divisible by number of size\n");
    MPI_Finalize();
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  // Calculate synchronized matrix multiplication
  CalculateSynchronized<MATRIX_SIZE>(A, B, C_sync, rank, size, timeSync);

  // Calculate asynchronous matrix multiplication
  CalculateAsynchronous<MATRIX_SIZE>(A, B, C_async, rank, size, timeAsync);

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
