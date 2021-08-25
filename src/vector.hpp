#ifndef VECTORANDMATRIX_VECTOR_H_
#define VECTORANDMATRIX_VECTOR_H_

#include <iostream>
#include <cassert>
#include <vector>

/* 参数提取器声明+定义 */
template <typename... Args>
class ArgsExtractor
{
}; // 泛化模板，没有任何调用和它匹配

template <typename T, typename U, typename... Args>
class ArgsExtractor<T *, U, Args...> : public ArgsExtractor<T *, Args...>
{
public:
    ArgsExtractor(T *value_p, U value, Args... args) : ArgsExtractor<T *, Args...>(++value_p, args...) // 这里不能用value_p++，因为value_p++为右值，无法被修改
    {
        --value_p; // 恢复指针
        value_ = *value_p = static_cast<U>(value);
    }
    const U &get_value() const
    {
        return value_;
    } // const函数不能返回可变引用，只能返回const引用或者copy版本
    const ArgsExtractor<T *, Args...> &get_next() const
    {
        return *this; // 返回值隐式转换为子类引用
    }

private:
    U value_;
}; // 偏特化模板，前N-1条调用与此匹配

template <typename T>
class ArgsExtractor<T *>
{
public:
    ArgsExtractor(T *value_p)
    {
        // --value_p;  // 这里进行指针移动，没有任何效果？
    } // 若最后args为空后仍带参数，偏特化模板中的public构造函数的必要的！
};    // 偏特化模板，最后一条调用和此匹配

/* Vector泛化模板声明+定义 */
template <typename T, int N, typename... Args>
class Vector
{
public:
    /* 构造/析构函数 */
    Vector()
    {
        std::cout << "constructor ()" << std::endl;
        data_ = new T[N](); // 全0动态数组
    }
    Vector(Vector<T, N, Args...>* p)
    {
        std::cout << "constructor (*)" << std::endl;
        *this = *p;  // p左值:指针内存地址 p右值:指针内存地址中的值 *p左值:指针指向的内存地址 *p右值:指针指向的内存地址中值 
    }  // 有问题
    Vector(Args... args)
    {
        std::cout << "constructor (args...)" << std::endl;
        data_ = new T[N]();
        ArgsExtractor<T *, Args...> args_extractor(data_, args...); // 将args...中的数据提取到data_中
    }                                                               // 构造函数，允许隐式构造函数，即Vector3f a = {1,2,3}
    ~Vector()
    {
        std::cout << "destructor ()" << std::endl;
        if (data_ != nullptr)
        {
            delete[] data_;
        }
    } // 析构函数

    /* 拷贝 */
    Vector(const Vector<T, N, Args...> &other) : data_(new T[N])
    {
        std::cout << "copy ()" << std::endl;
        std::copy(other.data_, other.data_ + N, data_);
    } // 拷贝构造函数
    Vector<T, N, Args...> &operator=(const Vector<T, N, Args...> &other)
    {
        std::cout << "copy =" << std::endl;
        if (this != &other)
        {
            delete[] data_;
            data_ = new T[N];
            std::copy(other.data_, other.data_ + N, data_);
        }
        return *this;
    } // 拷贝赋值运算符

    /* 移动 */
    Vector(Vector<T, N, Args...> &&other) : data_(nullptr)
    {
        std::cout << "move ()" << std::endl;
        data_ = other.data_;   // 这里是可以直接访问other的私有变量的
        other.data_ = nullptr; // 源对象的数据成员分配给默认值，这可以防止析构函数多次释放资源
    }                          // 移动构造函数
    Vector<T, N, Args...> &operator=(Vector<T, N, Args...> &&other)
    {
        std::cout << "move =" << std::endl;
        if (this != &other)
        {
            delete[] data_; // 由于是在堆中创建的动态数组，所以需要手动delete
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    } // 移动赋值运算符

    /* 转换 */
    // 转换构造函数，即允许哪些类型转换为该类
    Vector(double value)
    {
        std::cout << "convert i->vec" << std::endl;
        data_ = new T[N](); // 全0动态数组
        for (int i = N; i--; data_[i] = static_cast<T>(value))
            ;
    } // 允许隐式转换,Vector3f a = 1;
    // 自定义转换函数，即允许该类转化为其他哪些类型(标准类 or 非标准类)
    operator std::vector<T>()
    {
        std::cout << "convert vec->std::vector" << std::endl;
        std::vector<T> ret;
        for (int i = 0; i < N; i++)
        {
            ret.push_back(data_[i]);
        }
        return ret;
    } // 允许隐式转换,Vector3f a;vector<float> b = a;

    /* 作为成员函数重载运算符，= -> [] ()不能重载为友元函数 */
    inline T &operator[](int i)
    {
        assert(i >= 0 && i < N);
        return data_[i];
    } // 可被当成左值对返回元素进行修改
    inline T operator[](int i) const
    {
        assert(i >= 0 && i < N);
        return data_[i];
    } // 实例化后的const Vector会调用此函数
    inline Vector<T, N, Args...> operator+(const Vector<T, N, Args...> &vec)
    {
        std::cout << "vec + vec" << std::endl;
        Vector<T, N, Args...> ret = *this;  // 这里会调用copy()
        for (int i = N; i--; ret[i] += vec[i])
            ;
        return ret;  // 这里会调用move()
    } // 这里不能返回局部变量的引用，因为局部变量调用后会销毁，但copy版本的返回值不会被销毁
    inline Vector<T, N, Args...> operator+(T value)
    {
        std::cout << "vec + i" << std::endl;
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] += value)
            ;
        return ret;
    }

    /* 作为友元函数重载运算符，可以重载特殊计算顺序 */
    template <typename T, int N, typename... Args> // 这个必须得加，否则会报错：无法解析的外部符号
    friend Vector<T, N, Args...> operator+(T, const Vector<T, N, Args...> &);

    // friend std::ostream &operator<<(std::ostream &out, const Vector<N> &v);
    // friend Vector<N1> embed(const Vector<N2> &v, float fill = 1);
    // friend Vector<N1> proj(const Vector<N2> &v);
    void self_print()
    {
        for (int i = 0; i < N; i++)
        {
            std::cout << data_[i] << " ";
        }
        std::cout << std::endl;
    }

private:
    T *data_;
};

template <typename T, int N, typename... Args>
Vector<T, N, Args...> operator+(T value, const Vector<T, N, Args...> &vec)
{
    std::cout << "i + vec" << std::endl;
    Vector<T, N, Args...> ret = vec;
    for (int i = N; i--; ret[i] += value)
        ;
    return ret;
}

/* 别名定义 */
template <typename T, int N>
using Vector__ = Vector<T, N, T, T, T>;

template <int N>
using Vector_f = Vector<float, N, float, float, float>;

template <typename T>
using Vector3_ = Vector<T, 3, T, T, T>;

template <typename T>
using Vector4_ = Vector<T, 4, T, T, T>;

using Vector3f = Vector<float, 3, float, float, float>;
using Vector4f = Vector<float, 4, float, float, float, float>;

#endif // VECTORANDMATRIX_VECTOR_H_