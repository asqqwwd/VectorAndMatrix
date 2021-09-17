#ifndef VECTORANDMATRIX_MATRIX_H_
#define VECTORANDMATRIX_MATRIX_H_

#include "vector.hpp"
#include <vector>
#include <iostream>
#include <cassert>
#include <typeinfo>

/* Generalized matrix template class declaration and definition*/
template <typename T, int M, int N>
class Matrix
{
public:
    /* Constructor and Destructor */
    Matrix()
    {
        for (int i = M; i--;)
        {
            data_.push_back(Vector<T, N>());
        }
    }
    Matrix(std::initializer_list<T> il)
    {
        for (int i = M; i--;)
        {
            data_.push_back(Vector<T, N>());
        }
        int i = 0, j = 0;
        for (T value : il)
        {
            data_[i][j++] = value;
            if (j >= N)
            {
                j = 0;
                ++i;
            }
            if (i >= M)
            {
                break;
            }
        }
    }
    Matrix(Matrix *other_p)
    {
        data_.assign(other_p->data_.begin(), other_p->data_.end());
        other_p->data_.clear();
        other_p->data_.shrink_to_fit();
    }
    ~Matrix()
    {
        data_.clear();         // call destructor for each element in vector, and set size to zero
        data_.shrink_to_fit(); // set capacity of vector to zero
    }

    /* Copy semantics */
    Matrix(const Matrix &other) noexcept
    {
        data_.assign(other.data_.begin(), other.data_.end()); // clear this.data and deep copy with other.data
    }
    Matrix &operator=(const Matrix &other) noexcept
    {
        if (this != &other)
        {
            data_.assign(other.data_.begin(), other.data_.end());
        }
        return *this;
    }

    /* Move semantics */
    Matrix(Matrix &&other) noexcept
    {
        data_.assign(other.data_.begin(), other.data_.end());
        std::vector<Vector<T, N>>().swap(other.data_); // swap empty vector and other.data, equal to clear and shrink other.data
    }
    Matrix &operator=(Matrix &&other) noexcept
    {
        if (this != &other)
        {
            data_.assign(other.data_.begin(), other.data_.end());
            std::vector<Vector<T, N>>().swap(other.data_);
        }
        return *this;
    }

    /* Convert semantics */
    explicit Matrix(double value)
    {
        for (int i = M; i--;)
        {
            data_.push_back(Vector<T, N>(value));
        }
    }

    /* Overloading operator as membership function */
    inline Vector<T, N> &operator[](int i)
    {
        assert(i >= 0 && i < M);
        return data_[i];
    }
    inline Vector<T, N> operator[](int i) const
    {
        assert(i >= 0 && i < M);
        return data_[i];
    }
    inline Matrix &operator+=(const Matrix &mat)
    {
        for (int i = M; i--; data_[i] += mat[i])
            ;
        return *this;
    }
    inline Matrix &operator+=(T value)
    {
        for (int i = M; i--; data_[i] += value)
            ;
        return *this;
    }
    inline Matrix operator+(const Matrix &mat)
    {
        return Matrix(*this) += mat;
    }
    inline Matrix operator+(T value)
    {
        return Matrix(*this) += value;
    }
    inline Matrix &operator-=(const Matrix &mat)
    {
        for (int i = M; i--; data_[i] -= mat[i])
            ;
        return *this;
    }
    inline Matrix &operator-=(T value)
    {
        for (int i = M; i--; data_[i] -= value)
            ;
        return *this;
    }
    inline Matrix operator-(const Matrix &mat)
    {
        return Matrix(*this) -= mat;
    }
    inline Matrix operator-(T value)
    {
        return Matrix(*this) -= value;
    }
    inline Matrix &operator*=(const Matrix &mat)
    {
        for (int i = M; i--; data_[i] *= mat[i])
            ;
        return *this;
    }
    inline Matrix &operator*=(T value)
    {
        for (int i = M; i--; data_[i] *= value)
            ;
        return *this;
    }
    inline Matrix operator*(const Matrix &mat)
    {
        return Matrix(*this) *= mat;
    }
    inline Matrix operator*(T value)
    {
        return Matrix(*this) *= value;
    }
    inline Matrix &operator/=(const Matrix &mat)
    {
        try
        {
            for (int i = M; i--; data_[i] /= mat[i])
                ;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return *this;
    }
    inline Matrix &operator/=(T value)
    {
        try
        {
            for (int i = M; i--; data_[i] /= value)
                ;
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        return *this;
    }
    inline Matrix operator/(const Matrix &mat)
    {
        return Matrix(*this) /= mat;
    }
    inline Matrix operator/(T value)
    {
        return Matrix(*this) /= value;
    }

    /* Overloading operator as friend function */
    template <typename T1, int M1, int N1>
    friend Matrix operator+(T1, const Matrix &);
    template <typename T1, int M1, int N1>
    friend Matrix operator-(T1, const Matrix &);
    template <typename T1, int M1, int N1>
    friend Matrix operator*(T1, const Matrix &);
    template <typename T1, int M1, int N1>
    friend Matrix operator/(T1, const Matrix &);
    template <typename T1, int M1, int N1>
    friend std::ostream &operator<<(std::ostream &, const Matrix &);

    template <int _NEWROW, int _NEWCOL>
    Matrix<T, _NEWROW, _NEWCOL> reshape(T fill = 0)
    {
        Matrix<T, _NEWROW, _NEWCOL> ret(fill);
        for (int i = std::min(M, _NEWROW); i--;)
        {
            // ret[i] = (*this)[i].reshape<_NEWCOL>(fill);
            for (int j = std::min(N, _NEWCOL); j--;)
            {
                ret[i][j] = data_[i][j];
            }
        }
        return ret;
    }

    Matrix<T, N, M> transpose()
    {
        Matrix<T, N, M> ret;
        for (int i = N; i--;)
        {
            for (int j = M; j--;)
            {
                ret[i][j] = data_[j][i];
            }
        }
        return ret;
    }

    Matrix<T, N, M> transpose() const
    {
        Matrix<T, N, M> ret;
        for (int i = N; i--;)
        {
            for (int j = M; j--;)
            {
                ret[i][j] = data_[j][i];
            }
        }
        return ret;
    }

    template <int L>
    Matrix<T, M, L> mul(const Matrix<T, N, L> &mat)
    {
        Matrix<T, M, L> ret;
        Matrix<T, L, N> transpose_mat = mat.transpose();
        for (int i = M; i--;)
        {
            for (int j = L; j--;)
            {
                ret[i][j] = data_[i].mul(transpose_mat[j]);
            }
        }
        return ret;
    }

private:
    std::vector<Vector<T, N>> data_;
};

template <typename T, int M, int N>
Matrix<T, M, N> operator+(T value, const Matrix<T, M, N> &mat)
{
    return Matrix<T, M, N>(mat) += value;
}
template <typename T, int M, int N>
Matrix<T, M, N> operator-(T value, const Matrix<T, M, N> &mat)
{
    return Matrix<T, M, N>(mat) -= value;
}
template <typename T, int M, int N>
Matrix<T, M, N> operator*(T value, const Matrix<T, M, N> &mat)
{
    return Matrix<T, M, N>(mat) *= value;
}
template <typename T, int M, int N>
Matrix<T, M, N> operator/(T value, const Matrix<T, M, N> &mat)
{
    Matrix<T, M, N> ret = mat;
    try
    {
        for (int i = N; i--; ret[i] = value / mat[i])
            ;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
    return ret;
}
template <typename T, int M, int N>
std::ostream &operator<<(std::ostream &out, const Matrix<T, M, N> &mat)
{
    for (int i = 0; i < M; i++)
    {
        out << mat[i];
    }
    return out;
}

template <typename T, int M>
Matrix<T, M, M> indentity()
{
    Matrix<T, M, M> ret;
    for (int i = M; i--;)
    {
        ret[i][i] = 1;
    }
    return ret;
}

/* Alias template declaration */
using Matrix3f = Matrix<float, 3, 3>;
using Matrix4f = Matrix<float, 4, 4>;

#endif // VECTORANDMATRIX_MATRIX_H_