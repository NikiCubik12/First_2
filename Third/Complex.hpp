#ifndef COMPLEX_HPP
#define COMPLEX_HPP

#include <iostream>
#include <sstream>
#include <cmath>
#include <string>

using namespace std;

class Complex
{
private:
    double Re;
    double Im;

public:
    Complex();
    Complex(int value);
    Complex(double Re, double Im);
    Complex(const Complex& rhs);
    ~Complex() = default;
    
    Complex& operator=(Complex rhs);
    Complex& operator+=(Complex rhs);
    Complex operator+(Complex rhs);
    Complex operator*(double lambda);
    Complex operator*(Complex other);
    bool operator>(Complex other);
    bool operator<(Complex other);
    
    double Norm();
    string ToString();
    
    friend ostream& operator<<(ostream& out, Complex rhs);
};

#endif