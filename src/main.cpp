#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <typeinfo>
#include <cassert>
#include <initializer_list>
#include <memory>
#include <tuple>

#include "vector.hpp"
using namespace std;

void test_constructor()
{
    Vector3f a1;
    Vector3f a2 = Vector3f();
    auto fun_a = []() -> Vector3f
    {
        return Vector3f();
    };
    Vector3f a3 = fun_a(); // 这里只调用了constructor(), 与编译器优化有关

    Vector3f b1(1.1f, 2.1f, 3.1f);
    Vector3f b2(1.1f, 2.1, 3);
    Vector3f b3 = {1.1f, 2.1f, 3.1f};         // 这里右侧先转化为调用构造函数的Vector3f(*,*,*), 再被编译器优化为Vector3f b3(*,*,*)
    Vector3f b4(Vector3f(Vector3f(1, 1, 1))); // 等于Vector b4(1,1,1), 编译器将会对其进行优化

    Vector3f c1(new Vector3f(1.1f, 2.1f, 3.1f));

    a1.self_print();
    a2.self_print();
    a3.self_print();
    b1.self_print();
    b2.self_print();
    b3.self_print();
    b4.self_print();
    c1.self_print();
}

void test_copy()
{
    Vector3f a1(1.1f, 2.1f, 3.1f);
    Vector3f a2(a1); // 等同于Vector3f a2 = a1, 调用copy()
    Vector3f a3 = Vector3f();
    a3 = a1; // 调用copy=
    Vector3f *p = &a1;
    Vector3f c4 = *p; // 调用copy(), 可能与编译器优化有关?
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
    a1 = fun1();                    // 调用constuctor() move() move=
    a2 = a1 + 1;                    // 调用vec+i和move=
    Vector3f a3 = a1 + 1;           // 调用vec+i, 将返回值直接赋值给a3, 未调用move=
    Vector3f a4(Vector3f(1, 1, 1)); // 这个只调用了constructor(args...), 可能和编译器优化有关?
    auto fun2 = []() -> Vector3f
    {
        return Vector3f();
    };
    a2 = fun2();     // 这里会调用move=
    a2 = Vector3f(); // 调用move=
}

void test_convert()
{
    Vector3f a1 = 1;            // 先int->double(类型自动转换), 再调用convert i->vec进行了隐式类型转换
    Vector3f a2 = 1.1f;         // 先float->double(类型自动转换), 再调用convert i->vec进行了隐式类型转换
    Vector3f a3 = 1.1;          // 直接调用convert(double)进行了隐式类型转换
    std::vector<float> a4 = a1; // convert vec->std::vector, 隐式类型转换, 左右两侧类型type parameter必须一致
}

void test_overload_operator()
{
    // Vector3f a1 = Vector3f(1.1f, 2.1f, 3.1f);
    // Vector3f a2 = Vector3f(1.1f, 2.1f, 3.1f);
    // Vector3f a3 = Vector3f();
    // a3 = a1 + a2; // 调用vec+vec. 运算符返回右值, 所以这里还会调用一次move=
    // a3.self_print();
    // a3 = a1 + 1.1f; // 调用vec+i
    // a3.self_print();
    // a3 = 1.1f + a1; // 调用i+vec
    // a3.self_print();
    // a3 = a1 + 1;  // 调用vec+i
    // a3.self_print();

    Vector3f b1 = Vector3f(1.1f, 2.1f, 3.1f);
    b1[1] = 2;
    const Vector3f b2 = Vector3f(1.1f, 2.1f, 3.1f);
    b2[1];
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


int main(int argc, char **argv)
{
    test_constructor();
}