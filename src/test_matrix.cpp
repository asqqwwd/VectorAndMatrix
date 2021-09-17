#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <typeinfo>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <tuple>

#include "matrix.hpp"
using namespace std;

void test_constructor()
{
    Matrix3f a1 = Matrix3f();
    Matrix3f a2({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix3f a3(new Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f}));
}

void test_copy()
{
    Matrix3f a1({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix3f a2(a1);
    Matrix3f a3 = a1;
    Matrix3f *p = &a1;
    Matrix3f a4 = *p;
}

void test_move()
{
    Matrix3f a1({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix3f a2;
    a2 = a1 + 1; // move=

    auto fun1 = []() -> Matrix3f
    {
        Matrix3f ret;
        return ret; // move()
    };
    a2 = fun1(); // constuctor() move() move=
}

void test_convert()
{
    Matrix3f a1(1);
}

void test_overload_operator()
{
    Matrix3f a1 = Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix3f a2 = Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix3f a3;
    a3 = a1 + a2;   // vec+vec
    a3 = a1 + 1.f;  // vec+i
    a3 = 1.1f + a1; // i+vec
    a3 += 1;        // vec += i
    a3 += a2;       // vec += vec

    Matrix3f b1 = Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    b1[1][2] = 2;
    const Matrix3f b2 = Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    b2[0][1] = 1.1f;
}

void test_reshape()
{
    Matrix3f a1 = Matrix3f({1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f, 1.1f, 2.1f, 3.1f});
    Matrix4f a2 = a1.reshape<4, 4>();
    cout << a2;
    Matrix<float, 2, 4> a3 = a1.reshape<2, 4>(-9);
    cout << a3;
    Matrix<float, 6, 9> a4 = a1.reshape<6, 9>(1);
    cout << a4;
}

void test_mul()
{
    Matrix<float, 2, 3> a1 = Matrix<float, 2, 3>({1.1f, 2.1f, 3.1f, 4.1f, 5.1f, 6.1f});
    Matrix<float, 3, 4> a2 = Matrix<float, 3, 4>({1.1f, 2.1f, 3.1f, 4.1f, 5.1f, 6.1f, 1.1f, 2.1f, 3.1f, 4.1f, 5.1f, 6.1f});
    auto a3 = a1.mul(a2);
    cout << a3;

    auto b1 = indentity<float, 3>();
    auto b2 = indentity<float, 3>();
    cout << b1.reshape<3, 5>().mul(b2.reshape<5, 4>());
}

int main(int argc, char **argv)
{
    test_constructor();
    test_copy();
    test_move();
    test_convert();
    test_overload_operator();
    test_reshape();
    test_mul();
}