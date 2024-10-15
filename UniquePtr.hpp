//
// Created by ASUS on 2024/8/25.
//

#ifndef STL_UNIQUEPTR_HPP
#define STL_UNIQUEPTR_HPP

#include <cstddef>
#include <algorithm>

namespace stl {

    template <typename T>
    class UniquePtr {
    private:
        T* m_ptr;

    public:
        UniquePtr(std::nullptr_t = nullptr) : m_ptr(nullptr) {}

        explicit UniquePtr(T* ptr) : m_ptr(ptr) {}

        UniquePtr(UniquePtr&& other) { m_ptr = other.release(); }

        template <typename U>
        UniquePtr(UniquePtr<U>&& ptr) {
            m_ptr = ptr.release();
        }

        template <typename U>
        UniquePtr& operator=(UniquePtr<U>&& ptr) {
            reset(ptr.release());
            return *this;
        }

        UniquePtr& operator=(std::nullptr_t) { reset(nullptr); }

        UniquePtr(const UniquePtr& other) = delete;

        UniquePtr& operator=(const UniquePtr& other) = delete;

        UniquePtr& operator=(UniquePtr&& other) {
            m_ptr = other.release();
            return *this;
        }

        ~UniquePtr() {
            delete m_ptr;
            m_ptr = nullptr;
        }

        T* get() const { return m_ptr; }

        T* operator->() const { return m_ptr; }

        T& operator*() const { return *m_ptr; }

        void reset(T* ptr = nullptr) {
            if (m_ptr) delete m_ptr;
            m_ptr = ptr;
        }

        T* release() {
            T* ptr = m_ptr;
            if (m_ptr) {
                delete m_ptr;
                m_ptr = nullptr;
            }
            return ptr;
        }

        void swap(UniquePtr& other) {
            std::swap(m_ptr, other.m_ptr);
        }

        explicit operator bool() const { return m_ptr != nullptr; }
    };
}
#endif //STL_UNIQUEPTR_HPP
