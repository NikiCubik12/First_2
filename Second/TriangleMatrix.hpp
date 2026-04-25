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
        elems = new ArraySequence<T*> (item, RowsColomns);

        for (int i = 0; i < RowsColomns; i++)
            elems->Get(i) = new T[RowsColomns];

        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                elems->Get(i)[j] = item[i][j];
            }
        }
    }

    TriMatrix (const TriMatrix& mtrx)
    {
        RowsColomns = mtrx.RowsColomns;
        // elems = new ArraySequence<T*> (item, RowsColomns); (доступ к elems у mtrx)

        for (int i = 0; i < RowsColomns; i++)
            elems->Get(i) = new T[RowsColomns];

        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                elems->Get(i)[j] = mtrx.elems->Get(i)[j];
            }
        }
    }

    TriMatrix (ArraySequence<T*>* items)
    {
        RowsColomns = items->GetLength();
        elems = new ArraySequence<T*> (items, RowsColomns);

        for (int i = 0; i < RowsColomns; i++)
            elems->Get(i) = new T[RowsColomns];
        
        for (int i = 0; i < RowsColomns; i++)
        {
            for (int j = 0; j < RowsColomns; j++)
            {
                elems->Get(i)[j] = items->Get(i)[j];
            }
        }
    }

    ~TriMatrix ()
    {
        delete elems;
    }

    T CalcNorm ()
    {
        T mx = -10000000;
        for (int i = 0; i < RowsColomns; i++)
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
    ostream& operator<< (ostream& out, const TriMatrix& mtrx); // вывод матрицы

    bool CorrectMatrix ();
}; 

#endif