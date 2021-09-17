#ifndef VECTORANDMATRIX_VECTOR_H_
#define VECTORANDMATRIX_VECTOR_H_

#include <iostream>
#include <cassert>
#include <vector>
#include <cstdarg>
#include <typeinfo>
#include <initializer_list>

/* Vector泛化模板声明+定义 */
template <typename T, int N>
class Vector
{
public:
    /* 构造/析构函数 */
    Vector()
    {
        data_ = new T[N](); // 全0动态数组
    }
    Vector(std::initializer_list<T> il)
    {
        data_ = new T[N]();
        int i = 0;
        for (T value : il)
        {
            data_[i++] = value;
            if (i >= N)
            {
                break;
            }
        }
    } // 构造函数，允许隐式构造函数，即Vector3f a = {1,2,3}
    Vector(Vector *other_p) : data_(nullptr)
    {
        data_ = other_p->data_;
        other_p->data_ = nullptr;
    } // 基本等同于移动构造函数
    ~Vector()
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }
    } // 析构函数

    /* 拷贝 */
    Vector(const Vector &other) noexcept : data_(new T[N])
    {
        std::copy(other.data_, other.data_ + N, data_);
    } // 拷贝构造函数
    Vector &operator=(const Vector &other) noexcept
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = new T[N];
            std::copy(other.data_, other.data_ + N, data_);
        }
        return *this;
    } // 拷贝赋值运算符

    /* 移动 */
    Vector(Vector &&other) noexcept : data_(nullptr)
    {
        data_ = other.data_;   // 这里是可以直接访问other的私有变量的
        other.data_ = nullptr; // 源对象的数据成员分配给默认值，这可以防止析构函数多次释放资源
    }                          // 移动构造函数
    Vector &operator=(Vector &&other) noexcept
    {
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
    explicit Vector(double value)
    {
        data_ = new T[N](); // 全0动态数组
        for (int i = N; i--; data_[i] = static_cast<T>(value))
            ;
    } // 不允许隐式转换
    // 自定义转换函数，即允许该类转化为其他哪些类型(标准类 or 非标准类)
    operator std::vector<T>()
    {
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
    inline Vector &operator+=(const Vector &vec)
    {
        for (int i = N; i--; data_[i] += vec[i])
            ;
        return *this; // move()
    }                 // 这里不能返回局部变量的引用，因为局部变量调用后会销毁，但copy版本的返回值不会被销毁
    inline Vector &operator+=(T value)
    {
        for (int i = N; i--; data_[i] += value)
            ;
        return *this;
    }
    inline Vector operator+(const Vector &vec)
    {
        return Vector(*this) += vec;
    }
    inline Vector operator+(T value)
    {
        return Vector(*this) += value;
    }
    inline Vector &operator-=(const Vector &vec)
    {
        for (int i = N; i--; data_[i] -= vec[i])
            ;
        return *this;
    }
    inline Vector &operator-=(T value)
    {
        for (int i = N; i--; data_[i] -= value)
            ;
        return *this;
    }
    inline Vector operator-(const Vector &vec)
    {
        return Vector(*this) -= vec;
    }
    inline Vector operator-(T value)
    {
        return Vector(*this) -= value;
    }
    inline Vector &operator*=(const Vector &vec)
    {
        for (int i = N; i--; data_[i] *= vec[i])
            ;
        return *this;
    }
    inline Vector &operator*=(T value)
    {
        for (int i = N; i--; data_[i] *= value)
            ;
        return *this;
    }
    inline Vector operator*(const Vector &vec)
    {
        return Vector(*this) *= vec;
    }
    inline Vector operator*(T value)
    {
        return Vector(*this) *= value;
    }
    inline Vector &operator/=(const Vector &vec)
    {
        try
        {
            for (int i = N; i--; data_[i] /= vec[i])
                ;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return *this;
    }
    inline Vector &operator/=(T value)
    {
        try
        {
            for (int i = N; i--; data_[i] /= value)
                ;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return *this;
    }
    inline Vector operator/(const Vector &vec)
    {
        return Vector(*this) /= vec;
    }
    inline Vector operator/(T value)
    {
        return Vector(*this) /= value;
    }

    /* 作为友元函数重载运算符，可以重载特殊计算顺序 */
    template <typename T1, int N1> // friend function type parameters can't be same with class type parameters
    friend Vector operator+(T1, const Vector &);
    template <typename T1, int N1>
    friend Vector operator-(T1, const Vector &);
    template <typename T1, int N1>
    friend Vector operator*(T1, const Vector &);
    template <typename T1, int N1>
    friend Vector operator/(T1, const Vector &);
    template <typename T1, int N1>
    friend std::ostream &operator<<(std::ostream &, const Vector &);

    template <int M>
    Vector<T, M> reshape(T fill = 0)
    {
        Vector<T, M> ret = Vector<T, M>();
        for (int i = M; i--; ret[i] = (i < N ? data_[i] : fill))
            ;
        return ret;
    }

    T mul(const Vector<T, N> &vec)
    {
        T ret = 0;
        for (int i = N; i--;)
        {
            ret += data_[i] * vec[i];
        }
        return ret;
    }

private:
    T *data_;
};

template <typename T, int N>
Vector<T, N> operator+(T value, const Vector<T, N> &vec)
{
    return Vector<T, N>(vec) += value;
}
template <typename T, int N>
Vector<T, N> operator-(T value, const Vector<T, N> &vec)
{
    return Vector<T, N>(vec) -= value;
}
template <typename T, int N>
Vector<T, N> operator*(T value, const Vector<T, N> &vec)
{
    return Vector<T, N>(vec) *= value;
}
template <typename T, int N>
Vector<T, N> operator/(T value, const Vector<T, N> &vec)
{
    Vector<T, N> ret = vec;
    try
    {
        for (int i = N; i--; ret[i] = value / vec[i])
            ;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return ret;
}
template <typename T, int N>
std::ostream &operator<<(std::ostream &out, const Vector<T, N> &vec)
{
    for (int i = 0; i < N; i++)
    {
        out << vec[i] << " ";
    }
    out << std::endl;
    return out;
}

/* 别名定义 */
template <typename T, int N>
using Vector__ = Vector<T, N>;

template <int N>
using Vector_f = Vector<float, N>;

template <typename T>
using Vector3_ = Vector<T, 3>;

template <typename T>
using Vector4_ = Vector<T, 4>;

using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;

#endif // VECTORANDMATRIX_VECTOR_H_