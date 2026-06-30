#ifndef RECTANGULAR_MATRIX_HPP
#define RECTANGULAR_MATRIX_HPP

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"

using namespace std;

template <class T>
class RectangularMatrix
{
private:
    MutableArraySequence<MutableArraySequence<T>*>* rows;
    size_t rowsCount;
    size_t colsCount;
    
public:
    RectangularMatrix();
    RectangularMatrix(size_t m, size_t n);
    RectangularMatrix(T** items, size_t m, size_t n);
    RectangularMatrix(const RectangularMatrix& other);
    ~RectangularMatrix();
    
    T Get(size_t i, size_t j) const;
    void Set(size_t i, size_t j, T value);
    size_t GetRows();
    size_t GetCols();
    
    RectangularMatrix<T> Add(RectangularMatrix<T> other);
    RectangularMatrix<T> MultiplyScalar(T lambda);
    T Norm();
    string ToString();
    
    // Элементарные преобразования строк
    void SwapRows(size_t i, size_t j);
    void MultiplyRow(size_t i, T lambda);
    void AddRowToRow(size_t from, size_t to, T lambda = 1);
    
    // Элементарные преобразования столбцов
    void SwapCols(size_t i, size_t j);
    void MultiplyCol(size_t j, T lambda);
    void AddColToCol(size_t from, size_t to, T lambda = 1);
};

template <typename T>
RectangularMatrix<T> operator+(RectangularMatrix<T> m1, RectangularMatrix<T> m2);

template <typename T>
RectangularMatrix<T> operator*(RectangularMatrix<T> m, T lambda);

template <typename T>
RectangularMatrix<T> operator*(T lambda, RectangularMatrix<T> m);

template <typename T>
ostream& operator<<(ostream& out, RectangularMatrix<T> m);

#include "RectangularMatrix.tpp"

#endif