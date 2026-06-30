#include <iostream>
#include "DynamicArray.hpp"
using namespace std;

class Counter
{
    private:
    static int count;

    public:
    Counter() 
    {
        count++;
    }
    ~Counter()
    {
        count--;
    }
    static int GetCount()
    {
        return count;
    }
};

int Counter::count = 0; 

int main()
{
    Counter c1, c2;
    cout << Counter::GetCount() << endl;
    return 0;
}



// int main()
// {
//     int mas[3] {1, 2, 3};
//     DynamicArray<int> arr(mas, 3);
//     cout << arr.GetSize() << endl;

//     int x = 42;
//     int& p = 10;

//     return 0;
// }