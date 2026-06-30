#ifndef RECTANGULAR_MATRIX_TPP
#define RECTANGULAR_MATRIX_TPP

#include <cmath>
#include <sstream>
#include <string>
#include "Complex.hpp"


namespace RectangularMatrixDetail
{
    template <class T>
    double ValueNorm(T value)
    {
        return std::fabs(static_cast<double>(value));
    }

    inline double ValueNorm(Complex value)
    {
        return value.Norm();
    }

    template <class T>
    T NormResult(double value, T*)
    {
        return static_cast<T>(value);
    }

    inline Complex NormResult(double value, Complex*)
    {
        return Complex(value, 0);
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix() : rows(nullptr), rowsCount(0), colsCount(0) {}

template <class T>
RectangularMatrix<T>::RectangularMatrix(size_t m, size_t n) : rowsCount(m), colsCount(n)
{
    if (m == 0 || n == 0) throw invalid_argument("Размеры матрицы должны быть положительными");
    
    rows = new MutableArraySequence<MutableArraySequence<T>*>();
    for (size_t i = 0; i < rowsCount; i++)
    {
        MutableArraySequence<T>* row = new MutableArraySequence<T>();
        for (size_t j = 0; j < colsCount; j++)
        {
            row->Append(T());
        }
        rows->Append(row);
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(T** items, size_t m, size_t n) : rowsCount(m), colsCount(n)
{
    if (m == 0 || n == 0) throw invalid_argument("Размеры матрицы должны быть положительными");
    if (items == nullptr) throw invalid_argument("Указатель на данные матрицы пустой");
    
    rows = new MutableArraySequence<MutableArraySequence<T>*>();
    for (size_t i = 0; i < rowsCount; i++)
    {
        if (items[i] == nullptr) throw invalid_argument("Строка " + to_string(i) + " содержит пустой указатель");
        
        MutableArraySequence<T>* row = new MutableArraySequence<T>();
        for (size_t j = 0; j < colsCount; j++)
        {
            row->Append(items[i][j]);
        }
        rows->Append(row);
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(const RectangularMatrix& other)
{
    rowsCount = other.rowsCount;
    colsCount = other.colsCount;
    rows = new MutableArraySequence<MutableArraySequence<T>*>();
    
    for (size_t i = 0; i < rowsCount; i++)
    {
        MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
        for (size_t j = 0; j < colsCount; j++)
        {
            newRow->Append(other.Get(i, j));
        }
        rows->Append(newRow);
    }
}

template <class T>
RectangularMatrix<T>::~RectangularMatrix()
{
    if (rows != nullptr)
    {
        for (size_t i = 0; i < rows->GetLength(); i++)
        {
            delete rows->Get(i);
        }
        delete rows;
    }
}

template <class T>
T RectangularMatrix<T>::Get(size_t i, size_t j) const
{
    if (rows == nullptr) 
        throw runtime_error("Матрица не создана");
    if (i >= rowsCount) 
        throw out_of_range("Строка вне границ");
    if (j >= colsCount) 
        throw out_of_range("Столбец вне границ");
    return rows->Get(i)->Get(j);
}

template <class T>
void RectangularMatrix<T>::Set(size_t i, size_t j, T value)
{
    if (rows == nullptr) throw runtime_error("Матрица не создана");
    if (i >= rowsCount) throw out_of_range("Строка вне границ");
    if (j >= colsCount) throw out_of_range("Столбец вне границ");
    
    MutableArraySequence<T>* oldRow = rows->Get(i);
    MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
    
    for (size_t k = 0; k < colsCount; k++)
    {
        if (k == j)
            newRow->Append(value);
        else
            newRow->Append(oldRow->Get(k));
    }
    
    rows->Set(i, newRow);
    delete oldRow;
}

template <class T>
size_t RectangularMatrix<T>::GetRows()  { return rowsCount; }

template <class T>
size_t RectangularMatrix<T>::GetCols()  { return colsCount; }

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::Add(RectangularMatrix<T> other) 
{
    if (rowsCount != other.rowsCount || colsCount != other.colsCount)
    {
        throw invalid_argument("Размеры матриц не совпадают");
    }
    
    RectangularMatrix<T> result(rowsCount, colsCount);
    for (size_t i = 0; i < rowsCount; i++)
    {
        for (size_t j = 0; j < colsCount; j++)
        {
            result.Set(i, j, Get(i, j) + other.Get(i, j));
        }
    }
    return result;
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::MultiplyScalar(T lambda) 
{
    RectangularMatrix<T> result(rowsCount, colsCount);
    for (size_t i = 0; i < rowsCount; i++)
    {
        for (size_t j = 0; j < colsCount; j++)
        {
            result.Set(i, j, Get(i, j) * lambda);
        }
    }
    return result;
}

template <class T>
T RectangularMatrix<T>::Norm() 
{
    if (rowsCount == 0 || colsCount == 0) return RectangularMatrixDetail::NormResult(0, static_cast<T*>(nullptr));
    
    double maxNorm = 0;
    for (size_t j = 0; j < colsCount; j++)
    {
        double colSum = 0;
        for (size_t i = 0; i < rowsCount; i++)
        {
            colSum += RectangularMatrixDetail::ValueNorm(Get(i, j));
        }
        if (colSum > maxNorm) maxNorm = colSum;
    }
    return RectangularMatrixDetail::NormResult(maxNorm, static_cast<T*>(nullptr));
}


template <class T>
string RectangularMatrix<T>::ToString() 
{
    if (rows == nullptr) return "Матрица не создана";
    
    stringstream ss;
    for (size_t i = 0; i < rowsCount; i++)
    {
        ss << "[";
        for (size_t j = 0; j < colsCount; j++)
        {
            if (j > 0) ss << ", ";
            ss << Get(i, j);
        }
        ss << "]";
        if (i < rowsCount - 1) ss << "\n";
    }
    return ss.str();
}


template <class T>
void RectangularMatrix<T>::SwapRows(size_t i, size_t j)
{
    if (i >= rowsCount || j >= rowsCount)
        throw out_of_range("Ошибка: номер строки вне диапазона");
    if (i == j) return;
    
    MutableArraySequence<T>* temp = rows->Get(i);
    rows->Set(i, rows->Get(j));
    rows->Set(j, temp);
}

template <class T>
void RectangularMatrix<T>::MultiplyRow(size_t i, T lambda)
{
    if (i >= rowsCount)
        throw out_of_range("Ошибка: номер строки вне диапазона");
    
    MutableArraySequence<T>* oldRow = rows->Get(i);
    MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
    
    for (size_t j = 0; j < colsCount; j++)
    {
        newRow->Append(oldRow->Get(j) * lambda);
    }
    
    rows->Set(i, newRow);
    delete oldRow;
}

template <class T>
void RectangularMatrix<T>::AddRowToRow(size_t from, size_t to, T lambda)
{
    if (from >= rowsCount || to >= rowsCount)
        throw out_of_range("Ошибка: номер строки вне диапазона");
    
    MutableArraySequence<T>* fromRow = rows->Get(from);
    MutableArraySequence<T>* oldToRow = rows->Get(to);
    MutableArraySequence<T>* newToRow = new MutableArraySequence<T>();
    
    for (size_t j = 0; j < colsCount; j++)
    {
        newToRow->Append(oldToRow->Get(j) + fromRow->Get(j) * lambda);
    }
    
    rows->Set(to, newToRow);
    delete oldToRow;
}


template <class T>
void RectangularMatrix<T>::SwapCols(size_t i, size_t j)
{
    if (i >= colsCount || j >= colsCount)
        throw out_of_range("Ошибка: номер столбца вне диапазона");
    if (i == j) return;
    
    for (size_t row = 0; row < rowsCount; row++)
    {
        MutableArraySequence<T>* currentRow = rows->Get(row);
        T temp = currentRow->Get(i);
        
        MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
        for (size_t col = 0; col < colsCount; col++)
        {
            if (col == i)
                newRow->Append(currentRow->Get(j));
            else if (col == j)
                newRow->Append(temp);
            else
                newRow->Append(currentRow->Get(col));
        }
        
        rows->Set(row, newRow);
        delete currentRow;
    }
}

template <class T>
void RectangularMatrix<T>::MultiplyCol(size_t j, T lambda)
{
    if (j >= colsCount)
        throw out_of_range("Ошибка: номер столбца вне диапазона");
    
    for (size_t row = 0; row < rowsCount; row++)
    {
        MutableArraySequence<T>* currentRow = rows->Get(row);
        MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
        
        for (size_t col = 0; col < colsCount; col++)
        {
            if (col == j)
                newRow->Append(currentRow->Get(col) * lambda);
            else
                newRow->Append(currentRow->Get(col));
        }
        
        rows->Set(row, newRow);
        delete currentRow;
    }
}

template <class T>
void RectangularMatrix<T>::AddColToCol(size_t from, size_t to, T lambda)
{
    if (from >= colsCount || to >= colsCount)
        throw out_of_range("Ошибка: номер столбца вне диапазона");
    
    for (size_t row = 0; row < rowsCount; row++)
    {
        MutableArraySequence<T>* currentRow = rows->Get(row);
        MutableArraySequence<T>* newRow = new MutableArraySequence<T>();
        
        for (size_t col = 0; col < colsCount; col++)
        {
            if (col == to)
                newRow->Append(currentRow->Get(col) + currentRow->Get(from) * lambda);
            else
                newRow->Append(currentRow->Get(col));
        }
        
        rows->Set(row, newRow);
        delete currentRow;
    }
}

template <typename T>
RectangularMatrix<T> operator+(RectangularMatrix<T> m1, RectangularMatrix<T> m2)
{
    return m1.Add(m2);
}

template <typename T>
RectangularMatrix<T> operator*(RectangularMatrix<T> m, T lambda)
{
    return m.MultiplyScalar(lambda);
}

template <typename T>
RectangularMatrix<T> operator*(T lambda, RectangularMatrix<T> m)
{
    return m * lambda;
}

template <typename T>
ostream& operator<<(ostream& out, RectangularMatrix<T> m)
{
    out << m.ToString();
    return out;
}

#endif