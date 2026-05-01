#include <iostream>
using namespace std;
using std::cin;
using std::cout;
using out_of_range = std::out_of_range;
using invalid_argument = std::invalid_argument;
#include "Sequence.hpp"
#include "DynamicArray.hpp"
#include "LinkedList.hpp"
#include "TriangleMatrix.hpp"

int main()
{
    int RowsColomns = 2;
    int** matrix = new int* [RowsColomns];
    for (int i = 0; i < RowsColomns; i++)
    {
        matrix[i] = new int[RowsColomns];
    }
    for (int i = 0; i < RowsColomns; i++)
        for (int j = 0; j < RowsColomns; j++)
            matrix[i][j] = i+1;

    TriMatrix<int> mtrx (matrix, RowsColomns);
    cout << mtrx << endl;
}