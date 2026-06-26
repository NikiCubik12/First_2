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
    Complex operator+(Complex rhs) const;
    Complex operator*(double lambda) const;
    Complex operator*(Complex other) const;
    bool operator>(Complex other) const;
    bool operator<(Complex other) const;
    
    double Norm() const;
    string ToString() const;
    
    friend ostream& operator<<(ostream& out, Complex rhs);
};

#endif