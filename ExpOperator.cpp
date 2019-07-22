#include <iostream>

struct B
{
    int _n;

    B(int n) : _n(n) {}
};

struct A
{
    int _n;

    A() {}
    A(int n) : _n(n) {}

    B operator*()
    {
        return B(this->_n);
    }
    A operator*(B b)
    {
        int res = 1;
        for (int i = 0; i < b._n; i++)
            res *= this->_n;
        return A(res);
    }
};

std::istream &operator>>(std::istream &in, A &a)
{
    in >> a._n;
    return in;
}

std::ostream &operator<<(std::ostream &out, A a)
{
    out << a._n;
    return out;
}

int main(void)
{
    A x, y;

    std::cin >> x >> y;
    std::cout << x * *y;

    return 0;
}