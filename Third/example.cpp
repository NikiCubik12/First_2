#include <iostream>
#include "DynamicArray.hpp"
using namespace std;

int main()
{
    int* mas = new int[];
    const DynamicArray<int> arr(mas, 3);
    cout << arr.GetSize() << endl;
    return 0;
}