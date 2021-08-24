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
    // /* 1.Test construtor() */
    // cout<<"Test construtor()"<<endl;
    // Vector3f a1 = Vector3f();
    // a1.self_print();

    // /* 2.Test construtor(args...) */
    // cout<<"Test construtor(args...)"<<endl;
    // Vector3f b1(1.1f, 2.1f, 3.1f);
    // Vector3f b2(1.1f, 2.1, 3);
    // b1.self_print();
    // b2.self_print();

    // /* 3.Test copy */
    // cout<<"Test copy"<<endl;
    // Vector3f c1(1.1f, 2.1f, 3.1f);
    // Vector3f c2(c1);  // 等同于Vector3f c2 = c1
    // Vector3f c3 = Vector3f();
    // c3 = c1;
    // c2.self_print();
    // c3.self_print();

    /* 4.Test move */
    cout<<"Test move"<<endl;
    Vector3f d1(Vector3f(1.1f, 2.1f, 3.1f));
}