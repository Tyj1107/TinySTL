//
// Created by ASUS on 2023/10/18.
//

#ifndef STL_UTILITY_H
#define STL_UTILITY_H

#include <iostream>
#include <string>

template<typename T>
using iterator_t = typename std::iterator_traits<T>::iterator_category;

template<typename T, typename = void>
inline constexpr bool is_iterator_v = false;

template<typename T>
inline constexpr bool is_iterator_v<T, std::void_t<iterator_t<T>>> = true;

template<typename T, std::enable_if_t<is_iterator_v<T>, int> = 0>
void print(T first, T last, const std::string& name) {
    const size_t n = last - first;
    std::cout << name << " {";
    for (bool begin = true; first != last; ++first, begin = false) {
        if (!begin) {
            std::cout << ", ";
        }
        std::cout << *first;
    }
    std::cout << "}\n";
}

template<typename T>
void print(const T& container, const std::string& name) {
    std::cout << name << " {";
    for (auto iter = container.begin(); iter != container.end(); ++iter) {
        if (iter != container.begin()) std::cout << ", ";
        std::cout << *iter;
    }
    std::cout << "}  size: " << container.size() << std::endl;
}

// std::string 的字符串格式化函数
template<typename ... Args>
static std::string str_format(const std::string &format, Args&& ... args)
{
    // 计算格式化后字符串的长度，加 1 用来存放 C 字符串的 '\0' 终止符
    int size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
    // 使用 std::nothrow 可以在内存空间申请失败时不抛出 std::bad_alloc 异常，返回空指针
    std::unique_ptr<char[]> buf(new(std::nothrow) char[size]);
    // 如果申请内存空间失败返回空字符串
    if (!buf) return std::string("");
    // 格式化字符串到 buffer 中
    std::snprintf(buf.get(), size, format.c_str(), args...);
    // C字符串转 string 对象
    return std::string(buf.get(), buf.get() + size - 1);
}

#endif //STL_UTILITY_H
