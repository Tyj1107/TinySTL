//
// Created by ASUS on 2023/12/3.
//

#ifndef STL_STRING_H
#define STL_STRING_H

#include <ostream>
#include "Vector.hpp"


namespace stl
{

    class String
    {
    private:
        char *m_data;
        size_t m_capacity;
        size_t m_size;
        static constexpr size_t default_capacity = 15ULL;

    private:
        void init(char c = 0);
        void copy(const char *str);
        void destroy();
        void resize(size_t size);
        String add(const char *str, size_t size) const;
        void append(const char *str, size_t size);

    public:
        using value_type = char;
        using pointer = char *;
        using const_pointer = const char *;
        using reference = char &;
        using const_reference = const char &;
        using iterator = char *;
        using const_iterator = const char *;

        String();
        String(size_t size, char c);
        String(const char *str);
        String(const String &other);
        String &operator=(const char *str);
        String &operator=(const String &str);
        String &operator=(String &&str) noexcept;
        bool operator==(const char *str) const;
        bool operator!=(const char *str) const;
        bool operator==(const String &str) const;
        bool operator!=(const String &str) const;
        ~String();
        char &at(size_t index);
        const char &at(size_t index) const;
        char &operator[](size_t index) const;
        char &front();
        char &back();
        const char &front() const;
        const char &back() const;
        const char *data() const { return m_data; }
        const char *c_str() const { return m_data; }
        void clear();
        void append(const char *str);
        void append(const String &str);
        void insert(const char *str, size_t index);
        void insert(const String &str, size_t index);
        bool remove(size_t index, size_t count);
        size_t find(const char *str) const;
        size_t find(const String &str) const;
        String substr(size_t index, size_t count) const;
        void push_back(char c);
        void pop_back();
        String operator+(const char *str) const;
        String operator+(const String &str) const;
        String &operator+=(const char *str);
        String &operator+=(const String &str);
        Vector<String> split(char delimiter) const;
        Vector<String> split(const String &delimiter) const;
        bool startsWith(const char *str);
        bool startsWith(const String &str);
        iterator begin() const { return m_data; }
        iterator end() const { return m_data + m_size; }
        void reverse();
        size_t size() const { return m_size; }
        bool empty() const { return m_size == 0; }
        size_t capacity() const { return m_capacity; }
        friend std::ostream &operator<<(std::ostream &os, const String &str);
        friend std::istream &operator>>(std::istream &is, String &str);
    };
}; // namespace stl


#endif //STL_STRING_H
