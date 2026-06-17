#include <iostream>
#include "MutableArraySequence.hpp"
#include "Generator.hpp"
#include "LazySequence.hpp"
#include <clocale>
using namespace std;

class Box {
private:
    int value;

public:
    explicit Box(int x) {
        value = x;
    }

    void print() {
        cout << "Box value = " << value << endl;
    }
};

void show(Box b) {
    b.print();
}

int main() {
    Box b1(10);     
    Box b2 = 20; // "конфликт"  

    show(b1);       
    show(30);       

    return 0;
}


// вспомогательная функция как в Run_Tests.cpp
// static MutableArraySequence<int> ilist(std::initializer_list<int> vals)
// {
//     MutableArraySequence<int> s;
//     for (int v : vals) s.Append(v);
//     return s;
// }

// int main()
// {
//     setlocale(LC_ALL, "Rus");
//     // --- 1. Числа Фибоначчи (бесконечный генератор) ---
//     // auto fibInit = ilist({1, 1});
//     // Generator<int> fibGen([](const BoundedQueue<int>& q) {
//     //     return q.At(0) + q.At(1);
//     // }, fibInit, 2);

//     // LazySequence<int> fibs(fibGen);

//     // std::cout << "Фибоначчи (первые 8): ";
//     // for (size_t i = 0; i < 8; ++i)
//     //     std::cout << fibs.Get(i) << " ";
//     // std::cout << "\n";
//     // std::cout << "Материализовано: " << fibs.GetMaterializedCount() << "\n";
//     // std::cout << "Бесконечная: " << (fibs.IsInfinite() ? "да" : "нет") << "\n\n";

//     // // --- 2. Map: возводим в квадрат ---
//     // auto* squares = fibs.Map<int>([](int x) { return x * x; });

//     // std::cout << "Квадраты Фибоначчи (первые 6): ";
//     // for (size_t i = 0; i < 6; ++i)
//     //     std::cout << squares->Get(i) << " ";
//     // std::cout << "\n\n";

//     // // --- 3. Where: только чётные ---
//     // auto* evens = fibs.Where([](int x) { return x % 2 == 0; });

//     // std::cout << "Чётные Фибоначчи (первые 4): ";
//     // for (size_t i = 0; i < 4; ++i)
//     //     std::cout << evens->Get(i) << " ";
//     // std::cout << "\n\n";

//     // // --- 4. Конечный генератор: степени двойки ---
//     // auto powInit = ilist({1});
//     // Generator<int> powGen([](const BoundedQueue<int>& q) {
//     //     return q.Back() * 2;
//     // }, powInit, 1, 6); // maxCount = 6

//     // LazySequence<int> powers(powGen);

//     // std::cout << "Степени двойки (конечная, 6 штук): ";
//     // for (size_t i = 0; i < powers.GetLength(); ++i)
//     //     std::cout << powers.Get(i) << " ";
//     // std::cout << "\n";
//     // std::cout << "Мощность: " << powers.GetCardinality().GetFinite() << "\n\n";

//     // // --- 5. Reduce: сумма ---
//     // int sum = powers.Reduce([](int a, int b) { return a + b; }, 0);
//     // std::cout << "Сумма степеней двойки: " << sum << "\n";

//     // delete squares;
//     // delete evens;
//     // return 0;
// }
