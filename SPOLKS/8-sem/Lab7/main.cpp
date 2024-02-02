#include <mpi.h>

#include <cstdio>

#include "Matrix.h"

template <size_t Size>
constexpr void CalculateSynchronized(Matrix<Size>& A, Matrix<Size>& B, Matrix<Size>& C,
                                     const int size, const int to, const int from) {
  /* Process 0 fills the input matrices and broadcasts them to the rest */
  /* (actually, only the relevant stripe of A is sent to each process) */

  constexpr auto SizeSquare = Size * Size;

  MPI_Bcast(B.PlainArray(), SizeSquare, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Scatter(A.PlainArray(), SizeSquare / size, MPI_INT, A[from], SizeSquare / size,
              MPI_INT, 0, MPI_COMM_WORLD);

  for (int i = from; i < to; i++) {
    for (size_t j = 0; j < Size; j++) {
      C[i][j] = 0;
      for (size_t k = 0; k < Size; k++) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }

  MPI_Gather(C.PlainArray()[from], SizeSquare / size, MPI_INT, C.PlainArray(),
             SizeSquare / size, MPI_INT, 0, MPI_COMM_WORLD);
}

int main(int argc, char* argv[]) {
  constexpr auto MATRIX_SIZE = 3; /* Size of matrices */

  Matrix<MATRIX_SIZE> A{};
  Matrix<MATRIX_SIZE> B{};
  Matrix<MATRIX_SIZE> C{};

  A.Randomize();
  B.Randomize();

  int rank, size, from, to;

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
    if (rank == 0) printf("Matrix size not divisible by number of processors\n");
    MPI_Finalize();
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }

  from = rank * MATRIX_SIZE / size;
  to = (rank + 1) * MATRIX_SIZE / size;

  CalculateSynchronized<MATRIX_SIZE>(A, B, C, size, to, from);

  if (rank == 0) {
    std::cout << "Result:\n";
    C.Print();
    std::cout << "\n\n";
  }

  MPI_Finalize();

  return 0;
}
