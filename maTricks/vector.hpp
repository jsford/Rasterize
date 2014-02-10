#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
#include <iomanip>

template <typename T, typename CRTP>
struct VectorBase {
  CRTP& crtp() { return static_cast<CRTP&>(*this); }
  const CRTP& crtp() const { return static_cast<const CRTP&>(*this); }

  std::size_t size() const {
    return std::extent<decltype(CRTP::data)>::value;
  }

  void zero() {
    std::fill(std::begin(crtp().data), std::end(crtp().data), T());
  }

  typedef T* iterator;
  typedef const T* const_iterator;
  iterator begin() { return &crtp().data[0]; }
  iterator end() { return &crtp().data[0] + size(); }
  const_iterator begin() const { return &crtp().data[0]; }
  const_iterator end() const { return &crtp().data[0] + size(); }

  T& operator [] (std::size_t i) {
    return crtp().data[i];
  }
  const T& operator [] (std::size_t i) const {
    return crtp().data[i];
  }

  float mag() const {
      float  mag = 0;
      for(int i = 0; i < crtp().size(); i++){
          mag += crtp().data[i]*crtp().data[i];
      }
      return sqrt(mag);
  }

  void print(void) const {

      std::cout << "[";
      for(std::size_t i = 0; i < crtp().size() - 1; i++){
          std::cout << std::right << crtp().data[i] << "   ";
      }
      std::cout << crtp().data[crtp().size()-1] << "]";
  }

};

// **************************************************************
// Vector Classes.
// Special versions for duples, tuples, and quadruples.
// **************************************************************

template <typename T, std::size_t N>
struct Vector : VectorBase<T, Vector<T, N>> {
  union {
    T data[N];
    struct {
      T x, y, z, w;
    };
  };
};

template <typename T>
struct Vector<T, 2> : VectorBase<T, Vector<T, 2>> {
  union {
    T data[2];
    struct {
      T x, y;
    };
  };
};

template <typename T>
struct Vector<T, 3> : VectorBase<T, Vector<T, 3>> {
  union {
    T data[3];
    struct {
      T x, y, z;
    };
  };
};

template <typename T>
struct Vector<T, 4> : VectorBase<T, Vector<T, 4>> {
  union {
    T data[4];
    struct {
      T x, y, z, w;
    };
  };
};

// **************************************************************
// Componentwise Addition.
// <1, 2, 3> + < 4, 5, 6> = <5, 7, 9>
// **************************************************************

template <typename T, typename U, std::size_t N>
auto operator + (const Vector<T, N>& a, const Vector<U, N>& b)
 -> Vector<decltype(a[0] + b[0]), N> {
  Vector<decltype(a[0] + b[0]), N> result;
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] + b[i];
  }
  return result;
}

// **************************************************************
// Componentwise Subtraction.
// <1, 2, 3> - <4, 5, 6> = <-3, -3, -3>
// **************************************************************

template <typename T, typename U, std::size_t N>
auto operator - (const Vector<T, N>& a, const Vector<U, N>& b)
 -> Vector<decltype(a[0] - b[0]), N> {
  Vector<decltype(a[0] - b[0]), N> result;
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] - b[i];
  }
  return result;
}

// **************************************************************
// Componentwise Multiplication.
// <1, 2, 3> * <4, 5, 6> = <4, 10, 18>
// **************************************************************

template <typename T, typename U, std::size_t N>
auto operator * (const Vector<T, N>& a, const Vector<U, N>& b)
 -> Vector<decltype(a[0] * b[0]), N> {
  Vector<decltype(a[0] * b[0]), N> result;
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] * b[i];
  }
  return result;
}

// **************************************************************
// Scalar Multiplication.
// 3 * <1, 2, 3> = <3, 6, 9>
// **************************************************************

template <typename T, typename U, std::size_t N>
auto operator * (const Vector<T, N>& a, U c)
 -> Vector<decltype(a[0] * c), N> {
  Vector<decltype(a[0] * c), N> result;
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] * c;
  }
  return result;
}

// Isomorphism for symmetric usage.
template <typename T, typename U, std::size_t N>
auto operator * (U c, const Vector<T, N>& a)
 -> Vector<decltype(a[0] * c), N> {
  Vector<decltype(a[0] * c), N> result;
  for (std::size_t i = 0; i < N; ++i) {
    result[i] = a[i] * c;
  }
  return result;
}

// **************************************************************
// Dot product.
// <1, 2, 3> . <4, 5, 6> = 4 + 10 + 18 = 32
// **************************************************************

template <typename T, typename U, std::size_t N>
auto dot(const Vector<T, N>& a, const Vector<U, N>& b)
 -> decltype(a[0] * b[0]) {
  auto product = a * b;
  typedef decltype(product.x) V;
  return std::accumulate(std::begin(product), std::end(product), V(0));
}

// **************************************************************
// Cross product.
// <1, 2, 3> x <4, 5, 6> = <-3, 6, -3>
// **************************************************************

template <typename S, typename T>
auto cross(const Vector<S, 3>& U, const Vector<T, 3>& V)
 -> Vector<decltype(U[0]*V[0]), 3> {
    Vector<decltype(U[0]*V[0]), 3> ans;
    ans.x = U.y*V.z - U.z*V.y;
    ans.y = U.z*V.x - U.x*V.z;
    ans.z = U.x*V.y - U.y*V.x;
  return ans;
}

// **************************************************************
// Normalize.
// norm(<1, 2, 3>) = <0.26726, 0.53452, 0.80178>
// **************************************************************

template <typename S, size_t N>
auto norm(const Vector<S, N>& U) 
 -> Vector<decltype(U[0] * (float)N), N>{

    Vector<decltype(U[0] * (float)N), N>  ans;
    double magnitude = U.mag();

    for(int i=0; i<N; i++) {
        ans[i] = U[i] / magnitude;
    }
  return ans;
}

// **************************************************************
// Typedefs for Ease of Use.
// **************************************************************

typedef Vector<int, 2> Vec2i;
typedef Vector<int, 3> Vec3i;
typedef Vector<int, 4> Vec4i;
typedef Vector<float, 2> Vec2f;
typedef Vector<float, 3> Vec3f;
typedef Vector<float, 4> Vec4f;
typedef Vector<double, 2> Vec2d;
typedef Vector<double, 3> Vec3d;
typedef Vector<double, 4> Vec4d;

#endif