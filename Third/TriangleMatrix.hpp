#ifndef TRIANGLE_MATRIX
#define TRIANGLE_MATRIX

#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"

template <class T>
class TriMatrix 
{
    private:
    int RowsColomns;
    ArraySequence<T*>* elems;
    
    public:
    TriMatrix ()
    {
        RowsColomns = 0;
        elems = nullptr;
    }

    TriMatrix (T** item, int RowsColomns)
    {
        this->RowsColomns = RowsColomns;
        elems = new ArraySequence<T*>();

        for (int i = 0; i < RowsColomns; i++)
        {
            T* row = new T[RowsColomns];
            for (int j = 0; j < RowsColomns; j++)
            {
                row[j] = item[i][j];
            }
            elems->Append(row);
        }
    }

    TriMatrix (const TriMatrix& mtrx)
    {
        RowsColomns = mtrx.RowsColomns;
        elems = new ArraySequence<T*>();

        for (int i = 0; i < RowsColomns; i++)
        {
            T* row = new T[RowsColomns];
            for (int j = 0; j < RowsColomns; j++)
            {
                row[j] = mtrx.elems->Get(i)[j];
            }
            elems->Append(row);
        }
    }

    ~TriMatrix ()
    {
        if (elems != nullptr)
        {
            for (int i = 0; i < RowsColomns; i++)
            {
                if (elems->Get(i) != nullptr)
                    delete [] elems->Get(i);
            }
            delete elems;
        }
    }

    T CalcNorm ()
    {
        if (RowsColomns == 0)
            throw invalid_argument("Матрица пустая");
        
        T mx = 0;
        for (int i = 0; i < RowsColomns; i++)
            mx += elems->Get(i)[0];

        for (int i = 1; i < RowsColomns; i++)
        {
            T summ = 0;
            for (int j = 0; j < RowsColomns; j++)
            {
                summ += elems->Get(j)[i];
                if (mx < summ)
                {
                    mx = summ;
                }
            }
        }
        return mx;
    }
    
    int GetRowsColomns () const
    {
        return RowsColomns;
    }

    T GetElement(int i, int j) const
    {
        if (i < 0 || i >= RowsColomns || j < 0 || j >= RowsColomns)
            throw out_of_range("Index out of range");
        return elems->Get(i)[j];
    }

    void SetElement(int i, int j, T value)
    {
        if (i < 0 || i >= RowsColomns || j < 0 || j >= RowsColomns)
            throw out_of_range("Index out of range");
        elems->Get(i)[j] = value;
    }

    TriMatrix& operator= (const TriMatrix& mtrx)
    {
        if (&mtrx != this)
        {
            if (elems != nullptr)
            {
                for (int i = 0; i < RowsColomns; i++)
                {
                    if (elems->Get(i) != nullptr)
                        delete [] elems->Get(i);
                }
                delete elems;
            }
        
            RowsColomns = mtrx.RowsColomns;
            elems = new ArraySequence<T*>();

            for (int i = 0; i < RowsColomns; i++)
            {
                T* row = new T[RowsColomns];
                for (int j = 0; j < RowsColomns; j++)
                {
                    row[j] = mtrx.GetElement(i, j);
                }
                elems->Append(row);
            } 
        }
        return *this;  
    }
    
    int CorrectMatrix () const
    {
        int flag = 1;
        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                if ((i > j) && (elems->Get(i)[j] != 0))
                {
                    flag = 0;
                    break;
                }
            }
        }
        if (flag == 1)
        {
            return 1;
        }
        flag = 1;
        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                if ((i < j) && (elems->Get(i)[j] != 0))
                {
                    flag = 0;
                    break;
                }
            }
        }
        if (flag == 1)
        {
            return 2;
        }
        return 0;
    }

    void Print() const
    {
        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                cout << GetElement(i, j) << " ";
            }
            cout << endl;
        }
    }
}; 

// Оператор умножения (справа)
template <typename T>
TriMatrix<T> operator* (TriMatrix<T>& mtrx, double lambda)
{
    TriMatrix<T> result;
    int size = mtrx.GetRowsColomns();
    
    // Создаем временную матрицу
    T** temp = new T*[size];
    for (int i = 0; i < size; i++)
    {
        temp[i] = new T[size];
        for (int j = 0; j < size; j++)
        {
            temp[i][j] = mtrx.GetElement(i, j) * lambda;
        }
    }
    
    result = TriMatrix<T>(temp, size);
    
    // Очищаем временную матрицу
    for (int i = 0; i < size; i++)
        delete[] temp[i];
    delete[] temp;
    
    return result;
}

// Оператор умножения (слева)
template <typename T>
TriMatrix<T> operator* (double lambda, TriMatrix<T>& mtrx)
{
    return mtrx * lambda;
}

// Оператор сложения
template <typename T>
TriMatrix<T> operator+ (TriMatrix<T>& mtrx1, TriMatrix<T>& mtrx2)
{
    if ((mtrx1.CorrectMatrix() == mtrx2.CorrectMatrix()) && (mtrx1.CorrectMatrix() != 0) 
        && (mtrx1.GetRowsColomns() == mtrx2.GetRowsColomns()) && (mtrx1.GetRowsColomns() != 0))
    {
        int size = mtrx1.GetRowsColomns();
        
        // Создаем временную матрицу
        T** temp = new T*[size];
        for (int i = 0; i < size; i++)
        {
            temp[i] = new T[size];
            for (int j = 0; j < size; j++)
            {
                temp[i][j] = mtrx1.GetElement(i, j) + mtrx2.GetElement(i, j);
            }
        }
        
        TriMatrix<T> result(temp, size);
        
        // Очищаем временную матрицу
        for (int i = 0; i < size; i++)
            delete[] temp[i];
        delete[] temp;
        
        return result;
    }
    else 
    {
        throw invalid_argument("Матрицы нельзя сложить");
    }
}

// Оператор вывода
template <typename T>
ostream& operator<< (ostream& out, const TriMatrix<T>& mtrx)
{
    for (int i = 0; i < mtrx.GetRowsColomns(); i++)
    {
        for (int j = 0; j < mtrx.GetRowsColomns(); j++)
        {
            out << mtrx.GetElement(i, j) << " ";
        }
        out << endl;
    }
    return out;
}

#endif