#ifndef MATRIX_H
#define MATRIX_H

#include <initializer_list>
#include "vector.hpp"

#define EPSILON 1e-30

template <typename T, size_t M, size_t N>
struct Matrix {

    Vector<Vector<T, N>, M> data;

    typedef T* iterator;
    typedef const T* const_iterator;
    iterator begin() { return &data[0][0]; }
    iterator end() { return &data[M][N]; }
    const_iterator begin() const { return &data[0][0]; }
    const_iterator end() const { return &data[M][N]; }

    Matrix(void) {}
    Matrix( std::initializer_list<Vector<T, M>> args);
    Matrix(T* arr);
    void print(void);
    void zero(void);


    Vector<T, N>& operator [] (std::size_t i) {
      return data[i];
    }
    const Vector<T, N>& operator [] (std::size_t i) const {
      return data[i];
    }

};

// **************************************************************
// Construct Matrix from Array.
// int arr[] = {1,2,3,4};
// Matrix<int, 2, 2> m(arr);
// **************************************************************

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix(T* arr) {
    size_t pos = 0;
    for(size_t i = 0; i < M; i++) {
        for(size_t j = 0; j < N; j++) {
            data[i][j] = arr[pos++];
        }
    }
}

template <typename T, size_t M, size_t N>
Matrix<T, M, N>::Matrix( std::initializer_list<Vector<T, M>> args) {

    size_t r = 0;
    for(Vector<T, M> vec : args) {
        size_t c = 0;
        for(typename Vector<T, M>::iterator elem = vec.begin(); elem < vec.end(); ++elem) {
            data[c++][r] = *elem;
        }
        r++;
    }
}

// **************************************************************
// Print the Array to stdout.
// m.print();
// **************************************************************

template < typename T, size_t M, size_t N>
void Matrix<T, M, N>::print(void) {

    for(typename Vector<Vector<T, N>, M>::iterator r = data.begin(); r < data.end(); ++r) {
        
        if(r == data.begin()) { std::cout << "["; }
        else { std::cout << " "; }

        r->print();

        if(r+1 == data.end()) {std::cout << "]";}
        else {std::cout << std::endl;}
    }
    std::cout << std::endl;
}

// **************************************************************
// Zero the Matrix.
// m.zero();
// **************************************************************

template < typename T, size_t M, size_t N>
void Matrix<T, M, N>::zero(void) {
    for(typename Vector<Vector<T, N>, M>::iterator r = data.begin(); r < data.end(); ++r) {
        r->zero();
    }
}

// **************************************************************
// Matrix Multiplication.
// m3 = m1 * m2;
// **************************************************************

template <typename T_A, typename T_B, size_t M_A, size_t NM, size_t N_B>
auto operator * (const Matrix<T_A, M_A, NM>& A, const Matrix<T_B, NM, N_B>& B)
 -> Matrix<decltype(A[0][0]*B[0][0]), M_A, N_B> {

  Matrix<decltype(A[0][0]*B[0][0]), M_A, N_B> result;
  
  for(int i = 0; i < M_A; ++i) {
      for(int j = 0; j < N_B; ++j) {

          decltype(A[0][0]*B[0][0]) sum = 0;
          for(int k = 0; k < NM; ++k) {
              sum += A[i][k] * B[k][j];
          }
          result[i][j] = sum;
      }
  }

  return result;
}

// **************************************************************
// Scalar Multiplication.
// m2 = m1 * c;
// **************************************************************

template <typename T_MAT, typename T_CONST, size_t M, size_t N>
auto operator * (const Matrix<T_MAT, M, N>& A, T_CONST c)
 -> Matrix<decltype(A[0][0]*c), M, N> {

  Matrix<decltype(A[0][0]*c), M, N> result;
  
  for(int i = 0; i < M; ++i) {
      for(int j = 0; j < N; ++j) {
          result[i][j] = A[i][j] * c;
      }
  }

  return result;
}

// Isomorphism for symmetric usage.
template <typename T_MAT, typename T_CONST, size_t M, size_t N>
auto operator * (T_CONST c, const Matrix<T_MAT, M, N>& A)
 -> Matrix<decltype(A[0][0]*c), M, N> {

  Matrix<decltype(A[0][0]*c), M, N> result;
  
  for(int i = 0; i < M; ++i) {
      for(int j = 0; j < N; ++j) {
          result[i][j] = A[i][j] * c;
      }
  }

  return result;
}

// **************************************************************
// Scalar Multiplication.
// m *= c;
// **************************************************************

template <typename T, typename U, size_t M, size_t N>
void operator *= (Matrix<T, M, N>& A, U c) {
    for(typename Vector<Vector<T, N>, M>::iterator r = A.data.begin(); r < A.data.end(); ++r) {
        *r = (*r)*c;
    }
}

// **************************************************************
// Matrix Addition.
// |1 2|   |1 1|   |2 3|
// |3 4| + |1 1| = |4 5|
// |5 6|   |1 1|   |6 7|
// **************************************************************

template <typename T_A, typename T_B, size_t M, size_t N>
auto operator + (const Matrix<T_A, M, N>& A, const Matrix<T_B, M, N>& B)
 -> Matrix<decltype(A[0][0]+B[0][0]), M, N> {

  Matrix<decltype(A[0][0]+B[0][0]), M, N> result;
  
  for(int i = 0; i < M; ++i) {
      for(int j = 0; j < N; ++j) {
          result[i][j] = A[i][j] + B[i][j];
      }
  }

  return result;
}

// **************************************************************
// Matrix Subtraction.
// **************************************************************

template <typename T_A, typename T_B, size_t M, size_t N>
auto operator - (const Matrix<T_A, M, N>& A, const Matrix<T_B, M, N>& B)
 -> Matrix<decltype(A[0][0]-B[0][0]), M, N> {

  Matrix<decltype(A[0][0]-B[0][0]), M, N> result;
  
  for(int i = 0; i < M; ++i) {
      for(int j = 0; j < N; ++j) {
          result[i][j] = A[i][j] - B[i][j];
      }
  }

  return result;
}

// **************************************************************
// Gaussian Elimination.
// **************************************************************

template <typename T, size_t M, size_t N>
bool solve_GE(Matrix<T, M, N>& A) {

  for(int k = 0; k < M; ++k) {

      T max = 0;
      size_t max_idx;
      for(int idx = k; idx < M; ++idx) {
          if(abs(A[idx][k]) > max) {
            max = abs(A[idx][k]);
            max_idx = idx;
          }
      }

      if(max <= EPSILON) {
        std::cout << "Yo! Dis Matrix is Singular!\n";
        return(false);
      }

      if(k != max_idx) {
        std::cout << "Swapping Rows " << k << " and " << max_idx << "\n";
        Vector<T, N> temp = A[k];
        A[k] = A[max_idx];
        A[max_idx] = temp;
      }

      for(int i = k+1; i < M; ++i) {
        for(int j = k; j < N; ++j) {

          std::cout << i << " " << k << " " << A[i][k] << " " << A[k][k] << std::endl;
          A.print();
          A[i][j] = A[i][j] - A[k][j] * (A[i][k] / A[k][k]);
          std::cout << A[i][k] << A[k][k] << std::endl;

        }
        A[i][k] = 0;
      }


  }

  return(true);
}

#endif