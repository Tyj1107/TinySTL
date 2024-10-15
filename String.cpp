//
// Created by ASUS on 2023/12/3.
//
#include "String.h"

namespace stl
{
    String::String() : m_data(nullptr), m_size(0), m_capacity(default_capacity) {
        init();
    }

    String::String(size_t size, char c) : m_data(nullptr), m_size(0), m_capacity(default_capacity) {
        resize(size);
        memset(m_data, c, m_capacity);
    }

    String::String(const char *str) : m_data(nullptr), m_size(strlen(str)), m_capacity(default_capacity) {
        if (m_size <= default_capacity) {
            init();
        } else {
            resize(m_size);
        }
        strcpy(m_data, str);
    }

    String::String(const String &other) : String(other.m_data) {}

    String &String::operator=(const char *str) {
        copy(str);
        return *this;
    }

    String &String::operator=(const String &str) {
        if (this == &str) {
            return *this;
        }
        copy(str.m_data);
        return *this;
    }

    String &String::operator=(String &&str) noexcept {
        if (this == &str) {
            return *this;
        }
        delete[] m_data;
        m_data = str.m_data;
        m_size = str.m_size;
        m_capacity = str.m_capacity;
        str.m_data = nullptr;
        str.m_size = str.m_capacity = 0;
        return *this;
    }

    bool String::operator==(const char *str) const {
        if (str == nullptr) {
            throw std::runtime_error("str is null pointer");
        }
        return strcmp(m_data, str) == 0;
    }

    bool String::operator==(const String &str) const {
        return m_size == str.m_size && *this == str.m_data;
    }

    bool String::operator!=(const char *str) const {
        return !(*this == str);
    }

    bool String::operator!=(const String &str) const {
        return m_size != str.m_size || *this != str.m_data;
    }

    String String::add(const char *str, const size_t size) const {
        if (str == nullptr) {
            throw std::runtime_error("str is null pointer");
        }
        String result(m_size + size + 1, 0);
        strncpy(result.m_data, m_data, m_size);
        strcpy(result.m_data + m_size, str);
        return result;
    }

    String String::operator+(const char *str) const {
        if (str == nullptr) {
            throw std::runtime_error("str is null pointer");
        }
        return add(str, strlen(str));
    }

    String String::operator+(const String &str) const {
        return add(str.m_data, str.m_size);
    }

    String &String::operator+=(const char *str) {
        append(str, strlen(str));
        return *this;
    }

    String &String::operator+=(const String &str) {
        append(str.m_data, str.m_size);
        return *this;
    }

    void String::init(char c) {
        m_data = new char[m_capacity + 1];
        memset(m_data, c, m_size);
        m_data[m_size] = 0;
    }

    void String::copy(const char *str) {
        if (str == nullptr) {
            throw std::runtime_error("str pointer is null");
        }
        resize(strlen(str));
        strcpy(m_data, str);
    }

    void String::destroy() {
        if (m_data != nullptr) {
            delete[] m_data;
        }
        m_data = nullptr;
        m_size = m_capacity = 0;
    }

    String::~String() {
        destroy();
    }

    char& String::at(size_t index) {
        return const_cast<char&>(static_cast<const String &>(*this).at(index));
    }

    const char &String::at(size_t index) const {
        if (index >= m_size) {
            throw std::out_of_range("index out of range");
        }
        return m_data[index];
    }

    char &String::operator[](size_t index) const {
        return m_data[index];
    }

    void String::append(const char *str, size_t size) {
        if (str == nullptr) {
            throw std::out_of_range("pointer is null");
        }
        size_t end = m_size;
        resize(m_size + size);
        strcpy(m_data + end, str);
    }

    void String::append(const char *str) {
        if (str == nullptr) {
            throw std::out_of_range("pointer is null");
        }
        append(str, strlen(str));
    }

    void String::append(const String &str) {
        append(m_data, m_size);
    }

    void String::insert(const char *str, size_t index) {
        if (index > m_size) {
            throw std::out_of_range("insert index out of range");
        }
        size_t size = strlen(str);
        size_t old_size = m_size; // old end
        resize(m_size + size);
        memmove(m_data + index + size, m_data + index, old_size - index);
        strncpy(m_data + index, str, size);
    }

    void String::insert(const String &str, size_t index) {
        insert(str.m_data, index);
    }

    bool String::remove(size_t index, size_t count) {
        if (index > m_size) {
            return false;
        }
        if (index + count >= m_size) {
            m_size = index;
            m_data[m_size] = '\0';
        } else {
            m_size -= count;
            strcpy(m_data + index, m_data + index + count);
        }
        return true;
    }

    size_t String::find(const char *str) const {
        if (str == nullptr) {
            throw std::runtime_error("str is null pointer");
        }
        size_t index = strstr(m_data, str) - m_data;
        return (index >= 0 && index < m_size) ? index : -1;
    }

    size_t String::find(const String &str) const {
        return find(str.m_data);
    }

    String String::substr(size_t index, size_t count) const {
        if (index >= m_size || index + count > m_size) {
            throw std::out_of_range("index out of range");
        }
        String sub(count, 0);
        strncpy(sub.m_data, m_data + index, count);
        sub.m_data[sub.m_size] = 0;
        return sub;
    }

    void String::push_back(char c) {
        resize(m_size + 1);
        m_data[m_size - 1] = c;
    }

    void String::pop_back() {
        if (m_size == 0) {
            throw std::runtime_error("String is empty");
        }
        m_data[--m_size] = 0;
    }

    char& String::front() {
        return at(0);
    }

    char &String::back() {
        return at(m_size - 1);
    }

    const char& String::front() const {
        return at(0);
    }

    const char &String::back() const {
        return at(m_size - 1);
    }

    void String::resize(size_t size)
    {
        if (m_data && size <= m_capacity) {
            m_size = size;
            m_data[m_size] = '\0';
            return;
        }
        size_t capacity = default_capacity;
        while (capacity < size)
        {
            capacity += capacity / 2;
        }
        m_capacity = capacity;
        const char *temp = m_data;
        m_data = new char[m_capacity + 1];
        if (temp != nullptr) {
            strcpy(m_data, temp);
            delete[] temp;
        }
        m_size = size;
    }

    void String::clear() {
        if (m_data != nullptr) {
            memset(m_data, 0, m_size);
        }
        m_size = 0;
    }

    Vector<String> String::split(char delimiter) const {
        Vector<String> strs;
        size_t l = 0, r = 0;
        for (; r < m_size; ++r) {
            if (m_data[r] == delimiter) {
                String temp = substr(l, r - l);
                strs.push_back(temp);
                l = r + 1;
            }

        }
        if (l < m_size) {
            strs.push_back(substr(l, m_size - l));
        }
        return strs;
    }

    Vector<String> String::split(const String &delimiter) const {
        Vector<String> strs;
        size_t l = 0, r = 0;
        char *p = nullptr;
        while ((p = strstr(m_data + l, delimiter.m_data)) != nullptr) {
            // 123--456--789
            r = p - m_data;
            strs.push_back(substr(l, r - l));
            l = r + delimiter.m_size;
        }
        if (l < m_size) {
            strs.push_back(substr(l, m_size - l));
        }
        return strs;
    }

    bool String::startsWith(const char *str) {
        if (str == nullptr) {
            throw std::runtime_error("str is null pointer");
        }
        size_t size = strlen(str);
        if (size > m_size) {
            return false;
        }
        for (size_t i = 0; i < size; ++i) {
            if (m_data[i] != str[i]) {
                return false;
            }
        }
        return true;
    }

    bool String::startsWith(const String &str) {
        return str.m_size <= m_size && startsWith(str.m_data);
    }

    void String::reverse() {
        for (size_t i = 0, j = m_size - 1; i < j; ++i, --j) {
            std::swap(m_data[i], m_data[j]);
        }
    }

    std::ostream &operator<<(std::ostream &os, const String &str) {
        os << str.m_data;
        return os;
    }

    std::istream &operator>>(std::istream &is, String &str) {
        char buffer[1024] = {0};
        std::cin >> buffer;
        str.m_size = strlen(buffer);
        str.resize(str.m_size);
        strcpy(str.m_data, buffer);
        str.m_data[str.m_size] = 0;
        return is;
    }
};
