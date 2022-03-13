# Tensor Library: generalized implementation of vector and matrix with SFINAE

### SFINAE

```c++
// A
template<typename T, typename = std::enable_if_t<T>>
void func()
{
}

// B
template<typename T>
void func(typename std::enable_if_t<std::is_integral_v<T>>* = nullptr)
{
}
```

### Concept (C++ 20)

```C++
tempalte <typename T>
concept IsIntegral = std::is_integral_v<T>;

// A
IsIntegral auto (IsIntegral auto a, IsIntegral auto b)
{
}

// B
template<typename T> 
requires IsIntegral<T>
void func(T a)
{
}

// C
template<typename T>
void func(T a) requires IsIntegral<T>
{
}

// D
template<IsIntegral T>
void func(T a)
{
}
```

