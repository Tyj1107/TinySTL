//
// Created by ASUS on 2023/10/18.
//

#ifndef STL_ARRAY_HPP
#define STL_ARRAY_HPP

#include <cstdio>
#include <cstdint>
#include <stdexcept>

namespace stl {

    template<typename T, size_t N>
    class Array {
    public:
        T m_data[N];
        using value_type = T;
        using size_type = size_t;
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_pointer = const T *;
        using const_reference = const T &;
        using const_iterator = const T *;

    public:
        Array() = default;

        ~Array() = default;

        void fill(const T &value) {
            for (size_t i = 0; i < N; ++i) {
                m_data[i] = value;
            }
        }

        void swap(Array &other) {
            std::swap(*this, other);
        }

        T &operator[](const size_t i) {
            return m_data[i];
        }

        T &at(const size_t i) {
            return const_cast<T &>(static_cast<const Array &>(*this).at(i));
        }

        const T &at(const size_t i) const {
            if (i >= N) {
                throw std::out_of_range("array subscript out of range");
            }
            return m_data[i];
        }

        T &front() {
            return m_data[0];
        }

        T &back() {
            return m_data[N - 1];
        }

        const T &operator[](size_t i) const {
            return m_data[i];
        }

        const T &front() const {
            return m_data[0];
        }

        const T &back() const {
            return m_data[N - 1];
        }

        T *data() {
            return m_data;
        }

        const T *data() const {
            return m_data;
        }

        iterator begin() {
            return m_data;
        }

        iterator end() {
            return m_data + N;
        }

        const_iterator begin() const {
            return m_data;
        }

        const_iterator end() const {
            return m_data + N;
        }

        static constexpr size_t size() {
            return N;
        }

        static constexpr size_t max_size() {
            return N;
        }

        static constexpr bool empty() {
            return false;
        }
    };

    template<typename T>
    class Array<T, 0> {
    public:
        using value_type = T;
        using size_type = size_t;
        using pointer = T *;
        using reference = T &;
        using iterator = T *;
        using const_pointer = const T *;
        using const_reference = const T &;
        using const_iterator = const T *;

    public:
        void fill(const T &value) {
            throw std::out_of_range("array subscript out of range");
        }

        void swap(Array &other) {}

        T &operator[](const size_t i) {
            throw std::out_of_range("array subscript out of range");
        }

        T &at(const size_t i) {
            throw std::out_of_range("array subscript out of range");
        }

        const T &at(const size_t i) const {
            throw std::out_of_range("array subscript out of range");
        }

        T &front() {
            throw std::out_of_range("array subscript out of range");
        }

        T &back() {
            throw std::out_of_range("array subscript out of range");
        }

        const T &operator[](size_t i) const {
            throw std::out_of_range("array subscript out of range");
        }

        const T &front() const {
            throw std::out_of_range("array subscript out of range");
        }

        const T &back() const {
            throw std::out_of_range("array subscript out of range");
        }

        T *data() {
            return nullptr;
        }

        const T *data() const {
            return nullptr;
        }

        iterator begin() {
            return nullptr;
        }

        iterator end() {
            return nullptr;
        }

        const_iterator begin() const {
            return nullptr;
        }

        const_iterator end() const {
            return nullptr;
        }

        static constexpr size_t size() {
            return 0;
        }

        static constexpr size_t max_size() {
            return 0;
        }

        static constexpr bool empty() {
            return true;
        }
    };
} // namespace stl

#endif //STL_ARRAY_HPP
