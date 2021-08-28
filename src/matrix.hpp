#ifndef VECTORANDMATRIX_MATRIX_H_
#define VECTORANDMATRIX_MATRIX_H_

#include <iostream>
#include <cassert>

/* Generalized matrix template class declaration and definition*/
template <typename T, int M, int N>
class Matrix
{
public:
    /* Constructor and Destructor */
    Matrix()
    {
        data_ = new T[M * N]();
    }
    Matrix(Matrix<T, M, N> *other_p) : data_(nullptr)
    {
        data_ = other_p->data_;
        other_p->data_ = nullptr;
    }
    ~Matrix()
    {
        if (data_ != nullptr)
        {
            delete[] data_;
        }
    }

    /* Copy semantics */
    Matrix(const Matrix<T, M, N> &other) : data_(new T[M * N])
    {
        std::copy(other.data_, other.data_ + M * N, data_);
    }
    Matrix<T, M, N> &operator=(const Matrix<T, M, N> &other)
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = new T[M * N];
            std::copy(other.data_, other.data_ + M * N, data_);
        }
        return *this;
    }

    /* Move semantics */
    Matrix(Matrix<T, M, N> &&other) : data_(nullptr)
    {
        data_ = other.data_;
        other.data_ = nullptr;
    }
    Matrix<T, M, N> &operator=(Matrix<T, M, N> &&other)
    {
        if (this != &other)
        {
            delete[] data_;
            data_ = other.data_;
            other.data_ = nullptr;
        }
        return *this;
    }

    /* Convert semantics */
    Matrix(double value)
    {
        data_ = new T[M * N]();
        for (int i = M * N; i--; data_[i] = static_cast<T>(value))
            ;
    }

    /* Overloading operator as membership function */
    inline T &operator[](int i)
    {
        assert(i >= 0 && i < M * N);
        return data_[i];
    }
    inline T operator[](int i) const
    {
        assert(i >= 0 && i < M * N);
        return data_[i];
    }
    inline Matrix<T, M, N> operator+(const Matrix<T, M, N> &mat)
    {
        Matrix<T, M, N> ret = *this;
        for (int i = M * N; i--; ret[i] += mat[i])
            ;
        return ret;
    }
    inline Matrix<T, M, N> operator+(T value)
    {
        Matrix<T, M, N> ret = *this;
        for (int i = M * N; i--; ret[i] += value)
            ;
        return ret;
    }

    /* Overloading operator as friend function */
    template <typename T, int M, int N>
    friend Matrix<T, M, N> operator+(T, const Matrix<T, M, N> &);

private:
    T *data_;
};

template <typename T, int M, int N>
Matrix<T, M, N> operator+(T value, const Matrix<T, M, N> &vec)
{
    Matrix<T, M, N> ret = vec;
    for (int i = M * N; i--; ret[i] += value)
        ;
    return ret;
}

/* Alias template declaration */

#endif // VECTORANDMATRIX_MATRIX_H_