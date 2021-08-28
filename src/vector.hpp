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

template <typename T, typename... Args>
class ArgsExtractor<T, Args...> : public ArgsExtractor<Args...>
{
public:
    ArgsExtractor(T value, Args... args) : ArgsExtractor<Args...>(args...)
    {
        value_ = static_cast<T>(value);
    }
    const T &get_value() const
    {
        return value_;
    } // const函数不能返回可变引用，只能返回const引用或者copy版本
    const ArgsExtractor<Args...> &get_next() const
    {
        return *this; // 返回值隐式转换为子类引用
    }
    void copy_value_to(T *p)
    {
        *p = value_;
        (static_cast<ArgsExtractor<Args...>>(*this)).copy_value_to(++p);
    }

private:
    T value_;
}; // 偏特化模板，前N-1条调用与此匹配

template <>
class ArgsExtractor<>
{
public:
    template <typename T>
    void copy_value_to(T *p)
    {
        return;
    }
}; // 全特化模板，最后一条调用和此匹配

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
    Vector(Args... args)
    {
        std::cout << "constructor (args...)" << std::endl;
        ArgsExtractor<Args...> args_extractor(args...); // 继承方式展开参数包
        data_ = new T[N]();
        args_extractor.copy_value_to(data_);
    } // 构造函数，允许隐式构造函数，即Vector3f a = {1,2,3}
    Vector(Vector<T, N, Args...> *other_p) : data_(nullptr)
    {
        std::cout << "constructor (*)" << std::endl;
        data_ = other_p->data_;
        other_p->data_ = nullptr;
    } // 基本等同于移动构造函数
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
        std::cout << "[] &" << std::endl;
        assert(i >= 0 && i < N);
        return data_[i];
    } // 可被当成左值对返回元素进行修改
    inline T operator[](int i) const
    {
        std::cout << "[] const" << std::endl;
        assert(i >= 0 && i < N);
        return data_[i];
    } // 实例化后的const Vector会调用此函数
    inline Vector<T, N, Args...> operator+(const Vector<T, N, Args...> &vec)
    {
        std::cout << "vec + vec" << std::endl;
        Vector<T, N, Args...> ret = *this; // 这里会调用copy()
        for (int i = N; i--; ret[i] += vec[i])
            ;
        return ret; // 这里会调用move()
    }               // 这里不能返回局部变量的引用，因为局部变量调用后会销毁，但copy版本的返回值不会被销毁
    inline Vector<T, N, Args...> operator+(T value)
    {
        std::cout << "vec + i" << std::endl;
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] += value)
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator-(const Vector<T, N, Args...> &vec)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] -= vec[i])
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator-(T value)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] -= value)
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator*(const Vector<T, N, Args...> &vec)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] *= vec[i])
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator*(T value)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] *= value)
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator/(const Vector<T, N, Args...> &vec)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] /= vec[i])
            ;
        return ret;
    }
    inline Vector<T, N, Args...> operator/(T value)
    {
        Vector<T, N, Args...> ret = *this;
        for (int i = N; i--; ret[i] /= value)
            ;
        return ret;
    }

    /* 作为友元函数重载运算符，可以重载特殊计算顺序 */
    template <typename T, int N, typename... Args> // 这个必须得加，否则会报错：无法解析的外部符号
    friend Vector<T, N, Args...> operator+(T, const Vector<T, N, Args...> &);
    template <typename T, int N, typename... Args>
    friend Vector<T, N, Args...> operator-(T, const Vector<T, N, Args...> &);
    template <typename T, int N, typename... Args>
    friend Vector<T, N, Args...> operator*(T, const Vector<T, N, Args...> &);
    template <typename T, int N, typename... Args>
    friend Vector<T, N, Args...> operator/(T, const Vector<T, N, Args...> &);
    template <typename T, int N, typename... Args>
    friend std::ostream &operator<<(std::ostream &, const Vector<T, N, Args...> &);

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
template <typename T, int N, typename... Args>
Vector<T, N, Args...> operator-(T value, const Vector<T, N, Args...> &vec)
{
    Vector<T, N, Args...> ret = vec;
    for (int i = N; i--; ret[i] -= value)
        ;
    return ret;
}
template <typename T, int N, typename... Args>
Vector<T, N, Args...> operator*(T value, const Vector<T, N, Args...> &vec)
{
    Vector<T, N, Args...> ret = vec;
    for (int i = N; i--; ret[i] *= value)
        ;
    return ret;
}
template <typename T, int N, typename... Args>
Vector<T, N, Args...> operator/(T value, const Vector<T, N, Args...> &vec)
{
    Vector<T, N, Args...> ret = vec;
    for (int i = N; i--; ret[i] /= value)
        ;
    return ret;
}
template <typename T, int N, typename... Args>
std::ostream &operator<<(std::ostream &out, const Vector<T, N, Args...> &vec)
{
    for (int i = 0; i < N - 1; i++)
    {
        out << vec[i] << " ";
    }
    out << vec[i] << std::endl;
    return out;
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

Vector4f t4d(Vector3f vec, float fill = 0.f)
{
    return Vector4f(vec[0], vec[1], vec[2], fill);
}

Vector3f t3d(Vector4f vec)
{
    return Vector3f(vec[0], vec[1], vec[2]);
}

#endif // VECTORANDMATRIX_VECTOR_H_