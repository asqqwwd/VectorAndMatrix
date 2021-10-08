#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <typeinfo>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <tuple>

#include "tensor.hpp"
using namespace std;
using namespace Core;

namespace TestVector
{
    void test_constructor()
    {
        Vector3f a1;
        Vector3f a2 = Vector3f();
        auto fun_a = []() -> Vector3f
        {
            return Vector3f();
        };
        Vector3f a3 = fun_a(); // 这里只调用了constructor(), 与编译器优化有关

        Vector3f b1{1.1f, 2.1f, 3.1f};
        Vector3f b2{1.1f, 2.1, 3.1f};
        Vector3f b3 = {1.1, 2.1f, 3.1f};                // 这里右侧先转化为调用构造函数的Vector3f(*,*,*), 再被编译器优化为Vector3f b3(*,*,*)
        Vector3f b4(Vector3f(Vector3f{1.f, 1.f, 1.f})); // 等于Vector b4(1,1,1), 编译器将会对其进行优化

        Vector3f c1(new Vector3f({1.1f, 2.1f, 3.1f}));

        cout << "Vector::test_constructor()" << endl;
    }

    void test_copy()
    {
        Vector3f a1({1.1f, 2.1f, 3.1f});
        Vector3f a2(a1); // 等同于Vector3f a2 = a1, 调用copy()
        Vector3f a3 = Vector3f();
        a3 = a1; // 调用copy=
        Vector3f *p = &a1;
        Vector3f c4 = *p; // 调用copy(), 可能与编译器优化有关?

        cout << "Vector::test_copy()" << endl;
    }

    void test_move()
    {
        Vector3f a1 = Vector3f();
        Vector3f a2 = Vector3f();
        auto fun1 = []() -> Vector3f
        {
            Vector3f ret; // 这里会调用constructor()
            return ret;   // return这里会调用一次move()
        };
        a1 = fun1();                            // 调用constuctor() move() move=
        a2 = a1 + 1;                            // 调用vec+i和move=
        Vector3f a3 = a1 + 1;                   // 调用vec+i, 将返回值直接赋值给a3, 未调用move=
        Vector3f a4(Vector3f({1.f, 1.f, 1.f})); // 这个只调用了constructor(args...), 可能和编译器优化有关?
        auto fun2 = []() -> Vector3f
        {
            return Vector3f();
        };
        a2 = fun2();     // 这里会调用move=
        a2 = Vector3f(); // 调用move=

        cout << "Vector::test_move()" << endl;
    }

    void test_convert()
    {
        Vector3f a1(1);

        cout << "Vector::test_convert()" << endl;
        cout << a1;
    }

    void test_overload_operator()
    {
        Vector3f a1 = Vector3f({1.1f, 2.1f, 3.1f});
        Vector3f a2 = Vector3f({1.1f, 2.1f, 3.1f});
        Vector3f a3 = Vector3f();

        Vector3f b1 = Vector3f({1.1f, 2.1f, 3.1f});
        b1[1] = 2;
        const Vector3f b2 = Vector3f({1.1f, 2.1f, 3.1f});
        b2[1];

        cout << "Vector::test_overload_operator()" << endl;
        a3 = a1 + a2; // 调用vec+vec. 运算符返回右值, 所以这里还会调用一次move=
        cout << a3;
        a3 = a1 + 1.1f; // 调用vec+i
        cout << a3;
        a3 = 1.1f + a1; // 调用i+vec
        cout << a3;
        a3 = a1 + 1; // 调用vec+i
        cout << a3;
        a3 += a1;
        cout << a3;
        a3 += -9;
        cout << a3;
    }

    void test_different_return_type_function()
    {
        auto fun1 = []() -> Vector3f
        {
            return Vector3f();
        };
        auto fun2 = []() -> Vector3f
        {
            Vector3f ret = Vector3f();
            return ret; // 这里会调用一次move(), move完成后会释放局部变量ret
        };
        auto fun3 = []() -> Vector3f *
        {
            Vector3f *ret = new Vector3f(); // 注意，堆中空间不会自动释放，需手动delete
            return ret;
        };
        fun1();
        fun2();
        delete fun3();
        Vector3f a1 = fun1();  // constructor()
        Vector3f a2 = fun2();  // constructor() move()
        Vector3f *a3 = fun3(); // constuctor()
        delete a3;
        Vector3f a4 = Vector3f();
        a4 = fun1(); // constructor() move=
        a4 = fun2(); // constructor() move() deconstructor() move=
    }

    void test_reshape()
    {
        Vector3f a1 = Vector3f{1.1f, 2.1f, 3.1f};
        Vector4f a2 = a1.reshape<4>(-1);
        Tensor<float, 2> a3 = a1.reshape<2>();

        cout << "Vector::test_reshape()" << endl;
        cout << a2;
        cout << a3;
    }

    void test_mul()
    {
        Vector3f a1 = Vector3f{1.1f, 2.1f, 3.1f};
        Vector3f a2 = Vector3f{1.1f, 2.1f, 3.1f};

        cout << "Vector::test_mul()" << endl;
        cout << a1.mul(a2) << endl;
    }
}

namespace TestMatrix
{
    void test_constructor()
    {
        Matrix3f a1 = Matrix3f();
        Matrix3f a2{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        Matrix3f a3(new Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}});

        cout << "Matrix::test_constructor()" << endl;
        cout << a1;
        cout << a2;
        cout << a3;
    }

    void test_copy()
    {
        Matrix3f a1{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        Matrix3f a2(a1);
        Matrix3f a3 = a1;
        Matrix3f *p = &a1;
        Matrix3f a4 = *p;

        cout << "Matrix::test_copy()" << endl;
        cout << a1;
        cout << a2;
        cout << a3;
        cout << a4;
    }

    void test_move()
    {
        Matrix3f a1{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
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
        cout << "Matrix::test_convert()" << endl;
        cout << a1;
    }

    void test_overload_operator()
    {
        Matrix3f a1 = Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        Matrix3f a2 = Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        Matrix3f a3;
        a3 = a1 + a2;   // vec+vec
        a3 = a1 + 1.f;  // vec+i
        a3 = 1.1f + a1; // i+vec
        a3 += 1;        // vec += i
        a3 += a2;       // vec += vec

        Matrix3f b1 = Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        b1[1][2] = 2;
        const Matrix3f b2 = Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        b2[0][1] = 1.1f;

        cout << "Matrix::test_overload_operator()" << endl;
        cout << a1;
        cout << 1 / a1;
        cout << a1 / 2;
        cout << a1 / a2;
    }

    void test_reshape()
    {
        Matrix3f a1 = Matrix3f{{1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}, {1.1f, 2.1f, 3.1f}};
        Matrix4f a2 = a1.reshape<4, 4>();
        Tensor<Tensor<float, 4>, 2> a3 = a1.reshape<2, 4>(-9);
        Tensor<Tensor<float, 9>, 6> a4 = a1.reshape<6, 9>(1);

        cout << "Matrix::test_reshape()" << endl;
        cout << a2;
        cout << a3;
        cout << a4;
    }

    void test_mul()
    {
        Tensor<Tensor<float, 3>, 2> a1{{1.1f, 2.1f, 3.1f}, {4.1f, 5.1f, 6.1f}};
        Tensor<Tensor<float, 4>, 3> a2{{1.1f, 2.1f, 3.1f, 4.1f}, {5.1f, 6.1f, 1.1f, 2.1f}, {3.1f, 4.1f, 5.1f, 6.1f}};
        Tensor<Tensor<float, 2>, 2> a3{{1.1f, 2.1f}, {4.1f, 5.1f}};

        Matrix4f a4{{1.1f, 2.1f, 3.1f, 4.1f}, {1.1f, 2.1f, 3.1f, 4.1}, {1.1f, 2.1f, 3.1f, 4.1f}, {1.1f, 2.1f, 3.1f, 4.1f}};
        Matrix4f a5{{1.1f, 2.1f, 3.1f, 4.1f}, {1.1f, 2.1f, 3.1f, 4.1}, {1.1f, 2.1f, 3.1f, 4.1f}, {1.1f, 2.1f, 3.1f, 4.1f}};

        auto b1 = indentity<float, 3>();
        auto b2 = indentity<float, 3>();

        cout << "Matrix::test_mul()" << endl;
        // cout << a1.mul(a2);
        // cout << a4.mul(a5);
        // cout << b1.reshape<3, 5>().mul(b2.reshape<5, 4>());
        // cout << a1.mul(Vector3f{1.f, 2.f, 3.f});
        // cout << a4.mul(Vector3f{1.f, 2.f, 3.f}.reshape<4>());

        cout << Vector3f{1.1f, 2.2f, 3.3f}.normal();
    }
}

int main(int argc, char **argv)
{
    // TestVector::test_constructor();
    // TestVector::test_copy();
    // TestVector::test_move();
    // TestVector::test_convert();
    // TestVector::test_overload_operator();
    // TestVector::test_different_return_type_function();
    // TestVector::test_reshape();
    // TestVector::test_mul();

    // TestMatrix::test_constructor();
    // TestMatrix::test_copy();
    // TestMatrix::test_move();
    // TestMatrix::test_convert();
    // TestMatrix::test_overload_operator();
    // TestMatrix::test_reshape();
    TestMatrix::test_mul();
}