#pragma once
#include <functional>
#include <memory>
#include <utility>
#include <limits>

#include "iterator.hpp"

namespace sstd {
    //student's std

    template<typename Key, typename Value, typename Compare = std::less<Key> >
    class table;

    template<typename Key, typename Value, typename Compare = std::less<Key> >
    struct Node {
        Node(Key k, Value v, Node *parent = nullptr) : data_(std::move(k), std::move(v)), parent_(parent) {
        }

        Node() = delete;

        template<class... Args>
        explicit Node(Args &&... args, Node* parent = nullptr) : data_(std::forward<Args>(args)...), parent_(parent) {}

        std::pair<const Key, Value> data_;
        int bf_{0};
        Node *parent_ = nullptr;
        std::shared_ptr<Node> left_ = nullptr;
        std::shared_ptr<Node> right_ = nullptr;

        [[nodiscard]] int get_height() const {
            int left_h  = left_  ? left_->get_height()  : 0;
            int right_h = right_ ? right_->get_height() : 0;
            return std::max(left_h, right_h) + 1;
        }

        void update_balance() {
            int left_h  = left_  ? left_->get_height()  : 0;
            int right_h = right_ ? right_->get_height() : 0;
            bf_ = right_h - left_h;
        }
    };


    /**
     * @brief assotiative container table
     *
     * AVL-tree with single keys
     * C++20 standart
     *
     * @tparam Key key_type
     * @tparam Value value_type
     * @tparam Compare = std::less
     *
     * @note bidirectional iterator
     */
    template<typename Key, typename Value, typename Compare>
    class table {
        size_t size_{0};

        std::shared_ptr<Node<Key, Value, Compare> > root_ = nullptr;

        using NodePtr = std::shared_ptr<Node<Key, Value, Compare> >;

        static NodePtr clone_node(const Node<Key, Value, Compare> *cur, Node<Key, Value, Compare> *parent);

        [[nodiscard]] std::pair<Node<Key, Value, Compare> *, bool> find_node_(const Key &key) const;

        [[nodiscard]] Node<Key, Value, Compare> *find_needed_node(const Key &key) const;

        Node<Key, Value, Compare> *find_node_for_forward_iterator() const;

        [[nodiscard]] Node<Key, Value, Compare> *ul_find_needed_node(const Key &key, bool isUpper) const;

        NodePtr rotate_right(NodePtr node);

        NodePtr rotate_left(NodePtr node);

        NodePtr balance(NodePtr node);

        void rebalance_from(Node<Key, Value, Compare> *node, bool to_root = false);

        static NodePtr erase_node(NodePtr node);

    public:
        using key_type = Key;
        using mapped_type = Value;
        using value_type = std::pair<const Key, Value>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using key_compare = Compare;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = value_type *;
        using const_pointer = const value_type *;
        using iterator = table_iterator<Key, Value, Compare, false>;
        using const_iterator = table_iterator<Key, Value, Compare, true>;
        using reverse_iterator = std::reverse_iterator<iterator>;
        using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        class value_compare {
            friend class table;

        protected:
            Compare comp;

            explicit value_compare(Compare c) : comp(c) {
            }

        public:
            using result_type = bool;
            using first_argument_type = value_type;
            using second_argument_type = value_type;

            bool operator()(const value_type &lhs, const value_type &rhs) const {
                return comp(lhs.first, rhs.first);
            }
        };

        // ______________ CONSTRUCTORS _______________

        explicit table(const Compare &comp = Compare()) {
        } ///< default constructor

        table(const table &); ///< copy constructor

        table &operator=(const table &) &; ///< copy assignment

        table(table &&other) noexcept
    : size_(other.size_), root_(std::move(other.root_)) {
            other.size_ = 0;
        } ///< move constructor

        table &operator=(table &&other) & noexcept {
            if (this != &other) {
                root_ = std::move(other.root_);
                size_ = other.size_;
                other.size_ = 0;
            }
            return *this;
        } ///< move assignment

        table(std::initializer_list<value_type> init, const Compare &comp = Compare()) noexcept; ///< initializer_list constructor

        table &operator=(std::initializer_list<value_type> init) & noexcept; ///< initializer_list assignment

        ~table() = default;

        // ____________ ELEMENT ACCESS ______________________

        /**
         * @brief Access with insert empty elem if key not in table
         * @return reference to value
         */
        Value &operator[](const Key &key) &;

        /**
         * @brief Access if elem exist
         * @return reference to value
         * @throws std::out_of_range("Key not found in table")
         */
        Value &at(const Key &key) &;

        /**
         * @brief Access if elem exist
         * @return reference to value
         * @throws std::out_of_range("Key not found in table")
         */
        [[nodiscard]] const Value &at(const Key &key) const &;

        // ________________ ITERATORS _______________________

        /**
         * @return iterator to begin
         */
        iterator begin();

        /**
         * @return const_iterator to begin
         */
        [[nodiscard]] const_iterator begin() const;

        /**
        * @return const_iterator to begin
        */
        [[nodiscard]] const_iterator cbegin() const;

        /**
         * @return iterator to end
         */
        iterator end();

        /**
         * @return const_iterator to end
         */
        [[nodiscard]] const_iterator end() const;

        /**
         * @return const_iterator to end
         */
        [[nodiscard]] const_iterator cend() const;

        // _______________ CAPACITY ________________

        /**
         * @brief check is container empty
         */
        [[nodiscard]] bool empty() const noexcept { return !size_; }
        /**
         * @return quantity of elem in container
         */
        [[nodiscard]] size_type size() const noexcept { return size_; }
        /**
         * @return max able size of container
         */
        [[nodiscard]] size_type max_size() const noexcept { return std::numeric_limits<size_type>::max(); }

        // _________________ MODIFIERS _________________

        /**
         * @brief clear container
         */
        void clear() noexcept;

        /**
         * @brief insert element (No overwrite)
         * @return iterator and insert flag
         */
        std::pair<iterator, bool> insert(const value_type &value);

        /**
         * @brief insert element (No overwrite)
         * @return iterator and insert flag
         */
        std::pair<iterator, bool> insert(value_type &&value);

        /**
         * @brief insert elements from range [first, last)
         * @param first diapazone start
         * @param last diapazone end
         */
        template<typename I>
        void insert(I first, I last);

        /**
         * @brief refresh container with new elements
         * @param first diapazone start
         * @param last diapazone end
         */
        template<typename I>
        void assign(I first, I last);

        /**
        * @brief refresh container with new elements
        */
        void assign(std::initializer_list<value_type> init);


        /**
         * @brief insert elements (No overwrite) from initializer_list
         */
        void insert(std::initializer_list<value_type> init);

        /**
         * @brief insert element (With overwrite)
         * @return iterator and insert flag
         */
        std::pair<iterator, bool> insert_or_assign(const value_type &value);

        /**
         * @brief insert element (With overwrite)
         * @return iterator and insert flag
         */
        std::pair<iterator, bool> insert_or_assign(value_type &&value);

        /**
         * @brief insert with perfect_forwarding
         * @return iterator and insert flag
         */
        template<class... Args>
        std::pair<iterator, bool> emplace(Args &&... args);

        /**
         * @brief emplace with pos of possible place of insert
         * @param pos position of insert
         * @param args arguments for forwarding
         * @return iterator to inserted elem
         */
        template<class... Args>
        iterator emplace_hint(const_iterator pos, Args &&... args);

        /**
         * @brief try emplace by key
         * @return iterator and insert flag
         */
        template<class... Args>
        std::pair<iterator, bool> try_emplace(const Key &k, Args &&... args);

        /**
         * @brief try emplace with moved key
         * @return iterator and insert flag
         */
        template<class... Args>
        std::pair<iterator, bool> try_emplace(Key &&k, Args &&... args);

        /**
         * @brief delete elem by key
         * @return iterator to next elem
         */
        iterator erase(iterator pos) noexcept;

        /**
         * @brief delete elem by key
         * @return iterator to next elem
         */
        iterator erase(const_iterator pos) noexcept;

        /**
         * @brief delete elem by key
         * @return iterator to next elem
         */
        iterator erase(const_iterator first, const_iterator last) noexcept;

        /**
         * @brief delete elem by key
         * @return quantity of deleted elems (0 or 1)
         */
        size_type erase(const Key &key) noexcept;

        /**
         * @brief swaps data with other table
         */
        void swap(table &other) noexcept;

        // _____________ LOOKUP ______________

        /**
         * @return quantity of elems with this key (0 or 1)
         */
        [[nodiscard]] size_type count(const Key &key) const noexcept;

        /**
         * @brief find by key
         * @return iterator to elem
         */
        iterator find(const Key &key);

        /**
        * @brief find by key
        * @return const_iterator to elem
        */
        [[nodiscard]] const_iterator find(const Key &key) const;

        /**
        * @brief check is key in table
        */
        [[nodiscard]] bool contains(const Key &key) const noexcept;

        /**
        * @return iterators range with elems
        */
        std::pair<iterator, iterator> equal_range(const Key &key);

        /**
        * @return const_iterators range with elems
        */
        [[nodiscard]] std::pair<const_iterator, const_iterator> equal_range(const Key &key) const;

        /**
        * @return iterator to first elem >= key
        */
        iterator lower_bound(const Key &key);

        /**
        * @return const_iterator to first elem >= key
        */
        [[nodiscard]] const_iterator lower_bound(const Key &key) const;

        /**
        * @return iterator to first elem > key
        */
        iterator upper_bound(const Key &key);

        /**
        * @return const_iterator to first elem > key
        */
        [[nodiscard]] const_iterator upper_bound(const Key &key) const;

        // __________ OBSERVERS _______________

        /**
        * @return key comparator
        */
        static key_compare key_comp() { return Compare{}; }
        /**
        * @return value comparator
        */
        [[nodiscard]] value_compare value_comp() const { return value_compare(Compare{}); }
    };

    template<typename Key, typename Value, typename Compare>
    bool operator==(const table<Key, Value, Compare> &lhs,
                    const table<Key, Value, Compare> &rhs);

    template<typename Key, typename Value, typename Compare>
    auto operator <=>(const table<Key, Value, Compare> &lhs,
                      const table<Key, Value, Compare> &rhs);

    template<typename Key, typename Value, typename Compare>
    void swap(table<Key, Value, Compare> &lhs,
              table<Key, Value, Compare> &rhs) noexcept;

    template<typename Key, typename Value, typename Compare, typename Pred>
    table<Key, Value, Compare>::size_type erase_if(table<Key, Value, Compare> &c, Pred pred);
} // ---------------- END OF NAMESPACE SSTD ---------------------

static_assert(std::bidirectional_iterator<sstd::table<int, int>::iterator>);
static_assert(std::ranges::range<sstd::table<int, int> >);

#include "table.tpp"
