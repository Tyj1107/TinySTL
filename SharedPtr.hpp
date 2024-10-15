//
// Created by ASUS on 2024/8/25.
//

#ifndef STL_SHAREDPTR_HPP
#define STL_SHAREDPTR_HPP

#include <atomic>

namespace stl {
    struct RefCount {
        std::atomic_size_t m_count;
        RefCount(size_t count) : m_count(count) {}

        size_t incref() { return ++m_count; }
        size_t decref() { return --m_count; }
    };

    template<typename T>
    class SharedPtr {
    private:
        T *m_ptr;
        RefCount *m_refcount;

    public:
        SharedPtr(std::nullptr_t = nullptr)
            : m_ptr(nullptr), m_refcount(nullptr) {}

        explicit SharedPtr(T *ptr)
            : m_ptr(ptr), m_refcount(new RefCount(1)) {}

        SharedPtr(const SharedPtr& other)
            : m_ptr(other.m_ptr), m_refcount(other.m_refcount){
            m_refcount->incref();
        }

        ~SharedPtr() {
            if (m_ptr && m_refcount->decref() == 0) {
                delete m_ptr;
                delete m_refcount;
                m_ptr = nullptr;
                m_refcount = nullptr;
            }
        }

        T* get() const { return m_ptr; }

        size_t use_count() const { return m_refcount->m_count; }

        T operator*() { return *m_ptr; }

        T* operator->() { return m_ptr; }
    };

    template <typename T, typename ...Args>
    SharedPtr<T> makeSharedPtr(Args&& ...args) {
        return SharedPtr<T>(new T(std::forward<Args>(args)...));
    }
} // namespace stl
#endif //STL_SHAREDPTR_HPP
