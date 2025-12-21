#pragma once
#include <iterator>
#include <utility>
#include <cstddef>
#include <type_traits>

namespace sstd {

    template<typename Key, typename Value, typename Compare>
    struct Node;

    template<typename Key, typename Value, typename Compare>
    class table;

    template<typename Key, typename Value, typename Compare, bool IsConst>
    class table_iterator {
        using NodeT = Node<Key, Value, Compare>;

        NodeT *node_;

    public:
        using difference_type = std::ptrdiff_t;
        using value_type = std::pair<const Key, Value>;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_t = std::conditional_t<IsConst, const Value&, Value&>;

        using pointer = std::conditional_t<IsConst, const value_type*, value_type*>;;
        using reference = std::conditional_t<IsConst, const value_type&, value_type&>;;

        explicit table_iterator(NodeT *node = nullptr) : node_(node) {}

        operator table_iterator<Key, Value, Compare, true>() const;

        reference operator*() const; // UB if arg == nullptr
        pointer operator->() const;

        table_iterator& operator++() noexcept;
        table_iterator operator++(int) noexcept;

        table_iterator& operator--() noexcept;
        table_iterator operator--(int) noexcept;

        template<bool OtherConst>
        bool operator==(const table_iterator<Key, Value, Compare, OtherConst>& other) const;

        template<bool OtherConst>
        bool operator!=(const table_iterator<Key, Value, Compare, OtherConst>& other) const;

        template<typename K, typename V, typename C, bool IsConst_>
        friend class table_iterator;

        friend table<Key, Value, Compare>;
    };
}

#include "iterator.tpp"
