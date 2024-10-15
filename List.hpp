//
// Created by ASUS on 2023/10/27.
//

#ifndef STL_LIST_HPP
#define STL_LIST_HPP

#include <cstdint>
#include <iterator>
#include <initializer_list>
#include "Utility.hpp"

namespace stl {

    template<typename T>
    class List {
    private:
        struct ListNode {
            T m_value;
            ListNode *m_prev;
            ListNode *m_next;

            ListNode() : m_value(T()), m_prev(nullptr), m_next(nullptr) {}

            explicit ListNode(const T &value) : m_value(value), m_prev(nullptr), m_next(nullptr) {}

            ListNode(T value, ListNode *prev, ListNode *next) : m_value(value), m_prev(prev), m_next(next) {}

            ListNode(const ListNode &other)
                    : m_value(other.m_value), m_prev(other.m_prev), m_next(other.m_next) {}

            ListNode(ListNode &&other) noexcept
                    : m_value(other.m_value), m_prev(other.m_prev), m_next(other.m_next) {
                other.m_value = T();
                other.m_prev = other.m_next = nullptr;
            }
        };

        struct const_iterator {
            friend List;

            using iterator_category = std::bidirectional_iterator_tag;
            using value_type = T;
            using difference_type = ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            const_iterator() = default;

            explicit const_iterator(ListNode *cur) : m_cur(cur) {}

            const_iterator &operator=(const const_iterator &other) {
                if (this == &other) {
                    return *this;
                }
                m_cur = other.m_cur;
                return *this;
            }

            const_iterator &operator++() {
                m_cur = m_cur->m_next;
                return *this;
            }

            const_iterator &operator--() {
                m_cur = m_cur->m_prev;
                return *this;
            }

            const const_iterator operator++(int) {
                const_iterator temp = *this;
                m_cur = m_cur->m_next;
                return temp;
            }

            const const_iterator operator--(int) {
                const_iterator temp = *this;
                m_cur = m_cur->m_prev;
                return temp;
            }

            bool operator==(const const_iterator &other) const {
                return m_cur == other.m_cur;
            }

            bool operator!=(const const_iterator &other) const {
                return m_cur != other.m_cur;
            }

            const T &operator*() const {
                return m_cur->m_value;
            }

            const T *operator->() const {
                return m_cur;
            }

        protected:
            ListNode *m_cur;
        };

        struct iterator : public const_iterator {
            friend List;

            iterator() = default;

            explicit iterator(ListNode *cur) : const_iterator(cur) {}

            iterator(const const_iterator &iter) : const_iterator(iter.m_cur) {}

            iterator &operator=(const const_iterator &iter) {
                this->m_cur->m_value = iter.m_cur;
                return *this;
            }

            T &operator*() {
                return this->m_cur->m_value;
            }

            T *operator->() {
                return this->m_cur;
            }
        };

        ListNode m_dummy; // 假头结点
        size_t m_size;


    public:
        using value_type = T;
        using pointer = ListNode *;
        using const_pointer = const ListNode *;
        using reference = T &;
        using const_reference = const T &;
        using iterator = iterator;
        using const_iterator = const_iterator;

        List() : m_dummy(T(), &m_dummy, &m_dummy), m_size(0) {}

        explicit List(size_t n) : List(n, T()) {}

        List(size_t n, const T &value) : List() {
            assign(n, value);
        }

        List(List &&other) : m_dummy(other.m_dummy), m_size(other.m_size) {
            m_dummy.m_next->m_prev = &m_dummy;
            m_dummy.m_prev->m_next = &m_dummy;
            other.m_dummy->m_prev = other.m_dummy->m_next = &other.m_dummy;
        }

        template<typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0>
        List(InputIt first, InputIt last) : List() {
            assign(first, last);
        }

        List(std::initializer_list<T> values) : List(values.begin(), values.end()) {}

        ~List() {
            clear();
        }

        void clear() {
            for (ListNode *cur = m_dummy.m_next; cur != &m_dummy; --m_size) {
                cur->m_value.~T();
                cur = cur->m_next;
                delete cur->m_prev;
            }
            m_dummy.m_prev = m_dummy.m_next = &m_dummy;
        }

        List &operator=(const List &other) {
            if (this == &other) {
                return *this;
            }
            assign(other.begin(), other.end());
            return *this;
        }

        List &operator=(List &&other) noexcept {
            if (this == &other) {
                return *this;
            }
            clear();
            m_size = other.m_size;
            m_dummy.m_next = other.m_dummy.m_next;
            m_dummy.m_prev = other.m_dummy.m_prev;
            m_dummy.m_next.m_prev = &m_dummy;
            m_dummy.m_prev.m_next = &m_dummy;
            other.m_dummy.m_prev = other.m_dummy.m_next = &other.m_dummy;
            other.m_size = 0;
            return *this;
        }

        List &operator=(std::initializer_list<T> values) {
            assign(values.begin(), values.end());
        }

        void assign(size_t n, const T &value) {
            iterator iter = begin();
            for (size_t i = 0; i < n; ++i, ++iter) {
                if (iter == end()) {
                    iter = emplace(iter, value);
                } else {
                    *iter = value;
                }
            }
            while (iter != end()) {
                iter = erase(iter);
            }
        }

        template<class InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0>
        void assign(InputIt first, InputIt last) {
            iterator iter = begin();
            for (; first != last; ++first, ++iter) {
                if (iter == end()) {
                    iter = emplace(iter, *first);
                } else {
                    *iter = *first;
                }
            }
            while (iter != end()) {
                iter = erase(iter);
            }
        }

        void assign(std::initializer_list<T> values) {
            assign(values.begin(), values.end());
        }

        void push_front(const T &value) {
            emplace(begin(), value);
        }

        void push_front(T &&value) {
            emplace(begin(), std::move(value));
        }

        template<typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, int> = 0>
        void emplace_front(Args &&... args) {
            emplace(begin(), args...);
        }

        template<typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, int> = 0>
        void emplace_back(Args &&... args) {
            emplace(end(), args...);
        }

        template<typename ...Args, std::enable_if_t<std::is_constructible_v<T, Args&&...>, int> = 0>
        iterator emplace(const_iterator iter, Args&&... args) {
            auto prev = iter.m_cur->m_prev, next = iter.m_cur;
            auto p = new ListNode(T(std::forward<Args>(args)...), prev, next);
            prev->m_next = p;
            next->m_prev = p;
            ++m_size;
            return iterator(p);
        }

        void push_back(const T &value) {
            emplace(end(), value);
        }

        void push_back(T &&value) {
            emplace(end(), std::move(value));
        }

        void pop_front() {
            erase(begin());
        }

        void pop_back() {
            erase(std::prev(end()));
        }

        void resize(size_t n) {
            resize(n, T());
        }

        void resize(size_t n, const T &value) {
            if (n == m_size) return;
            if (n > m_size) {
                for (size_t i = m_size; i < n; ++i) {
                    emplace(end(), value);
                }
            } else {
                while (n < m_size) {
                    erase(std::prev(end()));
                }
            }
        }

        void swap(List &other) {
            if (this == &other) return;
            std::swap(m_dummy.m_next->m_prev, other.m_dummy.m_next->m_prev);
            std::swap(m_dummy.m_next, other.m_dummy.m_next);
            std::swap(m_dummy.m_prev->m_next, other.m_dummy.m_prev->m_next);
            std::swap(m_dummy.m_prev, other.m_dummy.m_prev);
            std::swap(m_size, other.m_size);
        }

        iterator begin() {
            return iterator(m_dummy.m_next);
        }

        iterator end() {
            return iterator(&m_dummy);
        }

        const_iterator begin() const {
            return const_iterator(m_dummy.m_next);
        }

        const_iterator end() const {
            return const_iterator(const_cast<ListNode*>(&m_dummy));
        }

        iterator insert(const_iterator iter, const T &value) {
            return emplace(iter, value);
        }

        iterator insert(const_iterator iter, T &&value) {
            return emplace(iter, std::move(value));
        }

        iterator insert(const_iterator iter, size_t n, const T &value) {
            iterator cur = iter;
            for (size_t i = 0; i < n; ++i) {
                cur = emplace(cur, value);
            }
            return cur;
        }

        template<typename InputIt, std::enable_if_t<is_iterator_v<InputIt>, int> = 0>
        iterator insert(const_iterator iter, InputIt first, InputIt last) {
            List other(first, last);
            iterator prev = std::prev(iter), next = iter;
            prev.m_cur->m_next = other.m_dummy.m_next;
            other.m_dummy.m_next->m_prev = prev.m_cur;
            next.m_cur->m_prev = other.m_dummy.m_prev;
            other.m_dummy.m_prev->m_next = next.m_cur;
            other.m_dummy.m_prev = other.m_dummy.m_next = &other.m_dummy;
            m_size += other.m_size;
            return std::next(prev);
        }

        iterator insert(const_iterator iter, std::initializer_list<T> values) {
            return insert(iter, values.begin(), values.end());
        }

        iterator erase(const_iterator iter) {
            if (iter.m_cur == &m_dummy) {
                return end();
            }
            ListNode *cur = iter.m_cur;
            iterator next(cur->m_next);
            cur->m_prev->m_next = cur->m_next;
            cur->m_next->m_prev = cur->m_prev;
            cur->m_value.~T();
            delete cur;
            --m_size;
            return next;
        }

        iterator erase(const_iterator first, const_iterator last) {
            iterator cur = first;
            while (cur != last) {
                cur = erase(cur);
            }
            return cur;
        }

        size_t remove(const T &value) {
            return remove_if([&](const T &cur) {
                return cur == value;
            });
        }

        template<typename UnaryPredicate>
        size_t remove_if(UnaryPredicate &&p) {
            size_t count = 0;
            for (iterator first = begin(), last = end(); first != last;) {
                if (p(*first)) {
                    first = erase(first);
                    ++count;
                } else {
                    ++first;
                }
            }
            return count;
        }

        T &front() {
            return const_cast<T &>(static_cast<const List &>(*this).front());
        }

        const T &front() const {
            if (empty()) {
                return m_dummy.m_value;
            }
            return m_dummy.m_next->m_value;
        }

        T &back() {
            return const_cast<T &>(static_cast<const List &>(*this).back());
        }

        const T &back() const {
            if (empty()) {
                return m_dummy.m_value;
            }
            return m_dummy.m_prev->m_value;
        }

        bool empty() const {
            return m_size == 0;
        }

        size_t size() const {
            return m_size;
        }

        constexpr size_t max_size() {
            return std::numeric_limits<size_t>::max() / sizeof(ListNode);
        }
    };

    template<typename T>
    bool operator==(const List<T>& a, const List<T>& b) {
        if (&a == &b) return true;
        if (a.size() != b.size()) return false;
        for (auto p = a.begin(), q = b.begin(), end = a.end(); p != end; ++p, ++q) {
            if (*p != *q) {
                return false;
            }
        }
        return true;
    }

}; // namespaece stl

#endif //STL_LIST_HPP
