#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
using runtime_error = std::runtime_error;
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "TriangleMatrix.hpp"
#include "Complex.hpp"

int main()
{
    Complex num (2.2, 3.3);
    cout << num << endl;

    Complex** arr = new Complex*[2]; // вначале выделяем под массив массивов (строки)
    for (int i = 0; i < 2; i++)
    {
        arr[i] = new Complex[2]; // а внутри выделяем под элементы подмассивов (столбцы)
    }
    for (int i = 0; i < 2; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (i <= j)
            {
                arr[i][j].SetComplex(2, 1);
            }
            else 
            {
                arr[i][j] = 0;
            }
        }
    }
    TriMatrix<Complex> matrix (arr, 2);
    TriMatrix<Complex> matrix2;
    TriMatrix<Complex> matrix3;
    matrix2 = matrix;
    matrix3 = matrix * 4;
    cout << matrix3;
    return 0;
}