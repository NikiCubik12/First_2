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
    ArraySequence<T*>* elems; // массив, элементами которого являются массивы (два указателя)
    
    public:
    TriMatrix ()
    {
        RowsColomns = 0;
        elems = nullptr;
    }

    TriMatrix (T** item, int RowsColomns)
    {
        this->RowsColomns = RowsColomns;

        // Создаем пустой ArraySequence
        elems = new ArraySequence<T*>();

        // Заполняем его строками с глубоким копированием
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

    // Конструктор копирования
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

    TriMatrix (ArraySequence<T*>* items)
    {
        RowsColomns = items->GetLength();
        elems = new ArraySequence<T*> (items, RowsColomns);

        for (int i = 0; i < RowsColomns; i++)
        {
            T* row = new T[RowsColomns]; 
            for (int j = 0; j < RowsColomns; j++)
            {
                row[j] = items->Get(i)[j];
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
            mx += elems->Get(i)[0]; // прошлись по 1-му столбцу

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
    
    int GetRowsColomns ()
    {
        return RowsColomns;
    }

    friend TriMatrix operator* (TriMatrix& mtrx, double lambda); // справа умножаем на скаляр
    friend TriMatrix operator* (double lambda, TriMatrix& mtrx); // слева умножаем на скаляр
    friend TriMatrix operator+ (TriMatrix& mtrx1, TriMatrix& mtrx2); // сложение матриц
    friend ostream& operator<< (ostream& out, const TriMatrix& mtrx); // вывод матрицы
    TriMatrix& operator= (const TriMatrix& mtrx) // перегрузка оператора присваивания
    {
        if (&mtrx != this) // защита от самоприсваивания, типа: a = a
        {
            for (int i = 0; i < RowsColomns; i++)
            {
                delete elems->Get(i);
            }
            delete elems;
        
            RowsColomns = mtrx.RowsColomns;
            elems = new ArraySequence<T*> ();

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
        return *this;  
    }
    

    int CorrectMatrix ()
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
}; 

template <typename T>
TriMatrix<T> operator* (TriMatrix<T>& mtrx, double lambda)
{
    TriMatrix<T> result;
    result.RowsColomns = mtrx.RowsColomns;
    result.elems = new ArraySequence<T*>();

    
    for (int i = 0; i < result.RowsColomns; i++)
    {
        T* row = new T[result.RowsColomns];
        for (int j = 0; j < result.RowsColomns; j++)
        {
            row[j] = mtrx.elems->Get(i)[j] * lambda;
        }
        result.elems->Append(row);
    }
    return result;
}

template <typename T>
TriMatrix<T> operator* (double lambda, TriMatrix<T>& mtrx)
{
    TriMatrix<T> result;
    result.RowsColomns = mtrx.RowsColomns;
    result.elems = new ArraySequence<T*>();

    
    for (int i = 0; i < result.RowsColomns; i++)
    {
        T* row = new T[result.RowsColomns];
        for (int j = 0; j < result.RowsColomns; j++)
        {
            row[j] = lambda * mtrx.elems->Get(i)[j];
        }
        result.elems->Append(row);
       
    } 
    return result;
}

template <typename T>
TriMatrix<T> operator+ (TriMatrix<T>& mtrx1, TriMatrix<T>& mtrx2)
{
    if ((mtrx1.CorrectMatrix() == mtrx2.CorrectMatrix()) && (mtrx1.CorrectMatrix() != 0) 
        && (mtrx1.GetRowsColomns() == mtrx2.GetRowsColomns()) && (mtrx1.GetRowsColomns() != 0))
    {
        TriMatrix<T> result;
        for (int i = 0; i < result.RowsColomns; i++)
        {
            T* row = new T[result.RowsColomns];
            for (int j = 0; j < result.RowsColomns; j++)
            {
                row[j] = mtrx1->Get(i)[j] + mtrx2->Get(i)[j];
            }
            result.elems->Append(row);
        }
        return result;
    }
    else 
    {
        throw invalid_argument("Матрицы нельзя сложить");
    }
}

template <typename T>
ostream& operator<< (ostream& out, const TriMatrix<T>& mtrx)
{
    for (int i = 0; i < mtrx.RowsColomns; i++)
    {
        for (int j = 0; j < mtrx.RowsColomns; j++)
        {
            out << mtrx.Get(i)[j] << " ";
        }
        out << endl;
    }
    return out;
} 




#endif