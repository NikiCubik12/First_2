#include "Complex.hpp"

Complex::Complex() : Re(0.0), Im(0.0) {}

Complex::Complex(int value) : Re((double)value), Im(0.0) {} 

Complex::Complex(double Re, double Im) : Re(Re), Im(Im) {}

Complex::Complex(const Complex& rhs) : Re(rhs.Re), Im(rhs.Im) {}

Complex& Complex::operator=(Complex rhs) // a = b, если копия, то a не изменится, а надо!!!
{
    if (this != &rhs) 
    {
        Re = rhs.Re;
        Im = rhs.Im;
    }
    return *this;
}

Complex& Complex::operator+=(Complex rhs) 
{
    Re += rhs.Re;
    Im += rhs.Im;
    return *this;
}

Complex Complex::operator+(Complex rhs) const 
{
    return Complex(Re + rhs.Re, Im + rhs.Im);
}

Complex Complex::operator*(double lambda) const 
{
    return Complex(Re * lambda, Im * lambda);
}

Complex Complex::operator*(Complex other) const 
{
    return Complex(Re * other.Re - Im * other.Im, Re * other.Im + Im * other.Re);
}

bool Complex::operator>(Complex other) const 
{
    return (Re*Re + Im*Im) > (other.Re*other.Re + other.Im*other.Im);
}

bool Complex::operator<(Complex other) const 
{
    return (Re*Re + Im*Im) < (other.Re*other.Re + other.Im*other.Im);
}

double Complex::Norm() const 
{
    return sqrt(Re * Re + Im * Im);
}

string Complex::ToString() const 
{
    stringstream ss;
    if (Im == 0) 
        ss << Re;
    else if (Re == 0) 
        ss << Im << "i";
    else 
        ss << Re << (Im > 0 ? "+" : "") << Im << "i";
    return ss.str();
}

ostream& operator<<(ostream& out, Complex rhs) 
{
    out << rhs.ToString();
    return out;
}