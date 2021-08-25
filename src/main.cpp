#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <typeinfo>
#include <cassert>
#include <initializer_list>
#include <memory>

#include "vector.hpp"
using namespace std;

int main(int argc, char **argv)
{
    /* 1.Test construtor() */
    Vector3f a1 = Vector3f();
    auto fun_a = []() -> Vector3f
    {
        return Vector3f();
    };
    Vector3f a2 = fun_a(); // 这里只调用了constructor(), 可能与编译器优化有关?

    /* 2.Test construtor(args...) */
    Vector3f b1(1.1f, 2.1f, 3.1f);
    Vector3f b2(1.1f, 2.1, 3);
    Vector3f b3 = {1.1f, 2.1f, 3.1f};         // 这里右侧先转化为调用构造函数的Vector3f(*,*,*), 再被编译器优化为Vector3f b3(*,*,*)
    Vector3f b4(Vector3f(Vector3f(1, 1, 1))); // 编译器将会对其进行优化
    // Vector3f b5(new Vector3f(1,1,1));
    // b5.self_print();

    /* 3.Test copy */
    Vector3f c1(1.1f, 2.1f, 3.1f);
    Vector3f c2(c1); // 等同于Vector3f c2 = c1, 调用copy()
    Vector3f c3 = Vector3f();
    c3 = c1; // 调用copy=
    Vector3f *p = &c1;
    Vector3f c4 = *p; // 调用copy(), 可能与编译器优化有关?

    /* 4.Test move */
    Vector3f d1 = Vector3f();
    Vector3f d2 = Vector3f();
    auto fun_d1 = []() -> Vector3f
    {
        Vector3f ret; // 这里会调用constructor()
        return ret;   // return这里会调用一次move()
    };
    fun_d1();                       // 调用constuctor()和move()
    d1 = fun_d1();                  // 调用move=
    d2 = d1 + 1;                    // 调用vec+i和move=
    Vector3f d3 = d1 + 1;           // 调用vec+i, 将返回值直接赋值给d3, 未调用move=
    Vector3f d4(Vector3f(1, 1, 1)); // 这个只调用了constructor(args...), 可能和编译器优化有关?
    auto fun_d2 = []() -> Vector3f
    {
        return Vector3f();
    };
    Vector3f d5 = Vector3f();
    d5 = fun_d2(); // 这里会调用move=

    /* 5.Test convert*/
    Vector3f e1 = 1;            // 先int->double(整型提升), 再调用convert i->vec进行了隐式类型转换
    Vector3f e2 = 1.1f;         // 先float->double(整型提升), 再调用convert i->vec进行了隐式类型转换
    Vector3f e3 = 1.1;          // 直接调用convert(double)进行了隐式类型转换
    std::vector<float> e4 = e2; // convert vec->std::vector, 隐式类型转换, 左右两侧类型type parameter必须一致

    /* 6.Test overload operator*/
    Vector3f f1 = Vector3f(1.1f, 2.1f, 3.1f);
    Vector3f f2 = Vector3f(1.1f, 2.1f, 3.1f);
    Vector3f f3 = Vector3f();
    f3 = f1 + f2; // 调用vec+vec. 运算符返回右值, 所以这里还会调用一次move=
    f3.self_print();
    f3 = f1 + 1.1f; // 调用vec+i
    f3.self_print();
    f3 = 1.1f + f1; // 调用i+vec
    f3.self_print();

    /* 7.Test different return type function */
    auto fun_g1 = []() -> Vector3f
    {
        return Vector3f();
    };
    auto fun_g2 = []() -> Vector3f
    {
        Vector3f ret = Vector3f();
        return ret; // 这里会调用一次move(), move完成后会释放局部变量ret
    };
    auto fun_g3 = []() -> Vector3f *
    {
        Vector3f *ret = new Vector3f(); // 注意，堆中空间不会自动释放，需手动delete
        return ret;
    };
    fun_g1();
    fun_g2();
    delete fun_g3();
    Vector3f g1 = fun_g1();  // constructor()
    Vector3f g2 = fun_g2();  // constructor() move()
    Vector3f *g3 = fun_g3(); // constuctor()
    delete g3;
    Vector3f g4 = Vector3f();
    g4 = fun_g1(); // constructor() move=
    g4 = fun_g2(); // constructor() move() deconstructor() move=

}