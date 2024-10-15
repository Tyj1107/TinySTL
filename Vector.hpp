//
// Created by ASUS on 2023/10/19.
//

#ifndef STL_VECTOR_HPP
#define STL_VECTOR_HPP

#include <cstdint>
#include <cstring>
#include <iterator>
#include <stdexcept>
#include <memory>
#include <initializer_list>
#include <type_traits>
#include "Utility.hpp"


namespace stl {

    template<typename T>
    class Vector {
    private:
        T *m_data;
        size_t m_size;
        size_t m_capacity;

    public:
        using allocator = std::allocator<T>; // 分配器，可以保证内存对齐

        using value_type = T;
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_pointer = const T *;
        using const_reference = const T &;
        using const_iterator = const T *;

        inline static const char *out_of_range = "vector subscript out of range";

    private:

        void free() {
            if (m_data) {
                allocator().deallocate(m_data, m_capacity);
            }
            m_data = nullptr;
            m_size = m_capacity = 0;
        }

        // subscript range: [first, last)
        void assign(size_t first, size_t last, const T& value) {
            for (size_t i = first; i < last; ++i) {
                ::new(m_data + i) T(value);
            }
        }

        template <typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0 >
        void assign(const_iterator iter, InputIt first, InputIt last) {
            for (size_t i = iter - m_data; first != last ; ++i, ++first) {
                ::new(m_data + i) T(*first);
            }
        }

        size_t increase(size_t need) const {
            size_t new_capacity = m_capacity ? m_capacity : 1;
            while (new_capacity < need) {
                new_capacity <<= 1ULL; // 扩容到原来的 2 倍
                //new_capacity = (new_capacity << 1ULL) - (new_capacity >> 1ULL); // 扩容到原来的 1.5 倍
            }
            return new_capacity;
        }

        // 将 [iter, end) 处的元素移动到 iter + n 处 (腾出 n 个空间)
        // 可能会改变 m_data
        void move_data(const_iterator iter, size_t n, bool new_memory) {
            const size_t pos = iter - m_data;
            if (new_memory) {
                const size_t new_capacity = increase(m_size + n);
                T *new_data = allocator().allocate(new_capacity);
                if (m_data) {
                    std::memcpy(new_data, m_data, sizeof(T) * pos);
                    std::memcpy(new_data + pos + n, m_data + pos, sizeof(T) * (m_size - pos));
                    allocator().deallocate(m_data, m_capacity);
                }
                m_data = new_data;
                m_capacity = new_capacity;
            } else {
                std::memmove(m_data + pos + n, m_data + pos, sizeof(T) * (m_size - pos));
            }
        }

    public:

        Vector() : m_data(nullptr), m_size(0), m_capacity(0) {}

        explicit Vector(const size_t n) : Vector(n, T()) {}

        Vector(const size_t n, const T& value) : Vector() {
            resize(n);
            assign(0, n, value);
        }

        Vector(std::initializer_list<T> values) : Vector(values.begin(), values.end()) {}

        Vector(const Vector &other) : Vector(other.begin(), other.end()) {}

        Vector(Vector &&other) : m_data(other.m_data), m_size(other.m_size), m_capacity(other.m_capacity) {
            other.m_data = nullptr;
            other.m_size = other.m_capacity = 0;
        }

        template <typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0 >
        Vector(InputIt first, InputIt last) : Vector() {
            resize(last - first);
            assign(m_data, first, last);
        }

        Vector &operator=(const Vector &other) {
            if (this == &other) {
                return *this;
            }
            if (m_capacity < other.m_size) {
                free();
                reverse(other.m_size);
                for (size_t i = 0; i < other.m_size; ++i) {
                    ::new(m_data + i) T(other[i]);
                }
            }
            assign(m_data, other.begin(), other.end());
            m_size = other.m_size;
            return *this;
        }

        Vector &operator=(Vector &&other) noexcept {
            if (this == &other) { // 未定义行为，此处直接返回
                return *this;
            }
            free();
            m_data = other.m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;

            other.m_data = nullptr;
            other.m_size = other.m_capacity = 0;
            return *this;
        }

        ~Vector() {
            free();
        }

        T &operator[](size_t i) {
            return m_data[i];
        }

        const T &operator[](size_t i) const {
            return m_data[i];
        }

        // range: [0, count)
        void assign(size_t n, const T& value) {
            if (m_capacity < n) {
                free();
            }
            resize(n);
            assign(0, n, value);
        }

        // range: [first, last)
        template <typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0 >
        void assign(InputIt first, InputIt last) {
            const size_t n = last - first;
            if (m_capacity < n) {
                free();
            }
            resize(n);
            assign(begin(), first, last);
        }

        void assign(std::initializer_list<T> values) {
            assign(values.begin(), values.end());
        }

        T &at(size_t i) {
            return const_cast<T &>(static_cast<const Vector &>(*this).at(i));
        }

        const T &at(size_t i) const {
            if (i >= m_size) {
                throw std::out_of_range(out_of_range);
            }
            return m_data[i];
        }

        void reverse(size_t n) {
            if (m_capacity < n) {
                T* new_data = allocator().allocate(n);
                if (m_data) {
                    std::memcpy(new_data, m_data, sizeof(T) * m_size);
                    allocator().deallocate(m_data, m_capacity);
                }
                m_data = new_data;
                m_capacity = n;
            } else if (m_size > n) {
                assign(n, m_size, T());
            }
        }

        void resize(size_t n) {
            reverse(n);
            m_size = n;
        }

        void shrink_to_fit() {
            if (m_size == 0) {
                free();
            } else {
                T *new_data = allocator().allocate(m_size);
                std::memcpy(new_data, m_data, sizeof(T) * m_size);
                allocator().deallocate(m_data, m_capacity);
                m_data = new_data;
                m_capacity = m_size;
            }
        }

        void clear() {
            resize(0);
        }

        void push_back(const T &value) {
            if (m_capacity > m_size) {
                ::new(m_data + m_size) T(value);
                ++m_size;
                return;
            }
            reverse(increase(m_size + 1));
            ::new(m_data + m_size) T(value);
            ++m_size;
        }

        void push_back(T &&value) {
            if (m_capacity > m_size) {
                ::new (m_data + m_size) T(std::move(value));
                return;
            }
            reverse(increase(m_size + 1));
            ::new (m_data + m_size) T(std::move(value));
            ++m_size;
        }

        template<typename ... Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, int> = 0>
        T& emplace(const_iterator iter, Args &&... args) {
            if (iter > end()) {
                throw std::runtime_error("Iterator out of range");
            }
            const size_t pos = iter - m_data;
            move_data(iter, 1ULL, m_size == m_capacity);
            ++m_size;
            // placement new (定位 new 或 布置 new : 在指定内存地址出原地构造)
            ::new(m_data + pos) T(std::forward<Args>(args)...);
            return m_data[pos];
        }

        template<typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, int> = 0>
        T& emplace_back(Args &&... args) {
            return emplace(end(), args...);
        }

        void swap(Vector &other) {
            std::swap(m_data, other.m_data);
            std::swap(m_size, other.m_size);
            std::swap(m_capacity, other.m_capacity);
        }

        void pop_back() {
            if (m_size == 0) {
                throw std::runtime_error("vector is empty");
            }
            --m_size;
            ::new(m_data + m_size) T();
        }

        T &front() {
            return const_cast<T &>(static_cast<const Vector &>(*this).front());
        }

        T &back() {
            return const_cast<T &>(static_cast<const Vector &>(*this).back());
        }

        const T &front() const {
            if (m_size == 0) {
                throw std::range_error(out_of_range);
            }
            return m_data[0];
        }

        const T &back() const {
            if (m_size == 0) {
                throw std::range_error(out_of_range);
            }
            return m_data[m_size - 1];
        }

        T *data() {
            return m_data;
        }

        const T *data() const {
            return m_data;
        }

        void erase(const_iterator iter) {
            erase(iter, iter + 1);
        }

        void erase(const_iterator first, const_iterator last) {
            if (first == last) return;
            if (first > last) throw std::runtime_error("first is greater than last");
            if (first >= m_size) throw std::runtime_error("first out of range");
            if (last > m_size) throw std::runtime_error("last out of range");
            const size_t diff = last - first;
            std::memmove(const_cast<iterator>(first), last, sizeof(T) * (end() - last));
            assign(m_size - diff, m_size, T());
        }

        void insert(const_iterator iter, const T &value) {
            insert(iter, 1ULL, value);
        }

        void insert(const_iterator iter, T &&value) {
            if (iter > end()) {
                throw std::runtime_error("insert Iterator out of range");
            }
            const size_t pos = iter - m_data;
            move_data(iter, 1ULL, m_size == m_capacity);
            ::new(m_data + pos) T(std::move(value));
            ++m_size;
        }


        void insert(const_iterator iter, size_t n, const T &value) {
            if (iter > end()) {
                throw std::runtime_error("insert Iterator out of range");
            }
            size_t pos = iter - m_data;
            const bool new_memory = m_size == m_capacity;
            move_data(iter, n, new_memory);
            assign(pos, pos + n, value);
            m_size += n;
        }


        template <typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0>
        void insert(const_iterator iter, InputIt first, InputIt last) {
            if (iter > end()) {
                throw std::runtime_error("insert Iterator out of range");
            }
            size_t n = last - first, pos = iter - m_data;
            const bool new_memory = (m_capacity - m_size) < n;
            if constexpr (std::is_same_v<iterator, InputIt>) { // 相同类型
                if (first >= begin() && last <= end()) { // 特殊情况，单独处理
                    if (iter > first && iter < last) { // first < iter < last
                        size_t pre = first - m_data, left = iter - first;
                        move_data(iter, n, new_memory);
                        first = m_data + pre, last = m_data + pos + n + n - left;
                        assign(m_data + pos, first, first + left);
                        assign(m_data + pos + left, m_data + pos + n, last);
                    } else {
                        size_t pre = first - m_data;
                        move_data(iter, n, new_memory);
                        if (iter <= first) {
                            first = m_data + pre + n;
                        } else {
                            first = m_data + pre;
                        }
                        last = first + n;
                        assign(m_data + pos, first, last);
                    }
                }
            } else {
                move_data(iter, n, new_memory);
                assign(m_data + pos, first, last);
            }
            m_size += n;
        }

        void insert(const_iterator iter, std::initializer_list<T> values) {
            insert(iter, values.begin(), values.end());
        }

        iterator begin() {
            return m_data;
        }

        iterator end() {
            return m_data + m_size;
        }

        const_iterator begin() const {
            return m_data;
        }

        const_iterator end() const {
            return m_data + m_size;
        }

        size_t size() const {
            return m_size;
        }

        size_t capacity() const {
            return m_capacity;
        }

        bool empty() const {
            return m_size == 0;
        }

        constexpr size_t max_size() const {
            return std::numeric_limits<size_t>::max() / sizeof(T);
        }
    };

    template<typename T>
    bool operator==(const Vector<T>& a, const Vector<T>& b) {
        if (&a == &b) return true;
        if (a.size() != b.size()) return false;
        for (size_t i = 0, n = a.size(); i < n; ++i) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
}; // namespace stl

#endif //STL_VECTOR_HPP
