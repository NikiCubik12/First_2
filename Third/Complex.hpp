#ifndef COMPLEX
#define COMPLEX

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

class Complex
{
    private:
    double Re;
    double Im;

    public:
    Complex ()
    {
        Re = 0.0;
        Im = 0.0;
    }
    Complex (double Re, double Im)
    {
        this->Re = Re;
        this->Im = Im;
    }
    Complex (const Complex& rhs)
    {
        Re = rhs.Re;
        Im = rhs.Im;
    }
    Complex operator=(const Complex& rhs)
    {
        if (&rhs != this) // защита от самоприсваивания
        {
            Re = rhs.Re;
            Im = rhs.Im;
        }
        return *this;
    }
    Complex operator=(const double& rhs) // когда есть только действительная часть, а мнимая = 0
    {
        Re = rhs;
        Im = 0.0;
        return *this;
    }
    Complex operator*(double lambda)
    {
        Re *= lambda;
        Im *= lambda;
        return *this;
    }
    friend ostream& operator<<(ostream& out, const Complex& rhs); // rhs -- Right Hand Side
    void SetComplex (double Re, double Im)
    {
        this->Re = Re;
        this->Im = Im;
    }
    double GetRe ()
    {
        return this->Re;
    }
    double GetIm ()
    {
        return this->Im;
    }
};

ostream& operator<<(ostream& out, const Complex& rhs) // объект out меняется, поэтому его же и возвращаем
{
    out << "(" << rhs.Re << ", " << rhs.Im << ")";
    return out;
}

#endif