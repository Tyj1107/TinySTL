//
// Created by ASUS on 2023/11/26.
//

#ifndef STL_DISJOINTSET_HPP
#define STL_DISJOINTSET_HPP

#include <vector>
#include <unordered_map>

namespace stl {

    template<typename T>
    class DisjointSet {
    private:
        std::vector<size_t> m_parent; // 保存代表元素下标
        std::unordered_map<T, size_t> m_data; // 保存元素以及初始下标
        size_t m_size;

    public:
        DisjointSet() : m_size(0) {}

        ~DisjointSet() = default;

        // 查找几个元素所处集合
        size_t find(T a) {
            if (m_data.find(a) == m_data.end()) {
                return -1;
            }
            size_t root = m_data[a];
            while (root != m_parent[root]) {
                root = m_parent[root];
            }
            for (size_t i = m_data[a], temp; i != root; i = temp) {
                temp = m_parent[i];
                m_parent[i] = root;
            }
            return root;
        }

        // 插入集合元素
        void insert(T a) {
            if (m_data.find(a) != m_data.end()) {
                return;
            }
            m_parent.push_back(m_parent.size());
            m_data[a] = m_parent.back();
            ++m_size;
        }

        // 合并两个集合
        void merge(T a, T b) {
            size_t pa = find(a), pb = find(b);
            if (pa == -1 || pb == -1) return;
            if (pa != pb) {
                m_parent[pb] = pa;
                --m_size; // 合并两个集合
            }
        }

        // 将集合元素从所处集合中分离出来
        void detach(T a) {
            size_t pa = find(a);
            if (pa == -1) return;
            m_parent[m_data[a]] = m_data[a];
            ++m_size;
        }

        size_t size() const { return m_size; }
    };

} // namespace stl

#if 0
// 集合元素数量确定
class DisjointSet {
private:
    std::vector<size_t> m_parent; // 保存代表元素下标
    size_t m_size;

public:
    DisjointSet(size_t n) : m_size(n), m_parent(n) {
        for (size_t i = 0; i < n; ++i) {
            m_parent[i] = i;
        }
    }

    ~DisjointSet() = default;

    size_t find(size_t a) {
        if (a >= m_parent.size()) {
            return -1;
        }
        size_t root = a;
        while (root != m_parent[root]) {
            root = m_parent[root];
        }
        for (size_t i = a, temp; i != root; i = temp) {
            temp = m_parent[i];
            m_parent[i] = root;
        }
        return root;
    }

    void merge(size_t a, size_t b) {
        size_t pa = find(a), pb = find(b);
        if (pa == -1 || pb == -1) return;
        if (pa != pb) {
            m_parent[pb] = pa;
            --m_size; // 合并两个集合
        }
    }

    void detach(size_t a) {
        size_t pa = find(a);
        if (pa == -1) return;
        m_parent[a] = a;
        ++m_size;
    }

    size_t size() const { return m_size; }
};
#endif

#endif //STL_DISJOINTSET_HPP
