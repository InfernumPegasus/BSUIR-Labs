#ifndef LAB7_MATRIX_H
#define LAB7_MATRIX_H

#include <iomanip>
#include <iostream>
#include <random>
#include <memory>

template <size_t N>
class Matrix final {
 private:
  int (*matrix)[N];

 public:
  constexpr Matrix() {
    matrix = new int[N][N];
  }

  constexpr ~Matrix() {
    delete[] matrix;
  }

  constexpr void Print() const noexcept {
    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < N; j++) {
        std::cout << std::setw(5) << matrix[i][j];
      }
      std::cout << '\n';
    }
  }

  void Randomize() noexcept {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> distrib(1, 10);

    for (size_t i = 0; i < N; i++) {
      for (size_t j = 0; j < N; j++) {
        matrix[i][j] = distrib(gen);
      }
    }
  }

  [[nodiscard]] constexpr auto PlainArray() noexcept { return matrix; }
  [[nodiscard]] constexpr auto PlainArray() const noexcept { return matrix; }

  constexpr auto operator[](size_t i) const noexcept { return *(matrix + i); }
  constexpr auto operator[](size_t i) noexcept { return *(matrix + i); }
};

#endif  // LAB7_MATRIX_H
