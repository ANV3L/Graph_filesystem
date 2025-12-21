#pragma once

namespace sstd {
    // ________________ HELP FUNCTIONS ________________

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::NodePtr table<Key, Value, Compare>::clone_node(
        const Node<Key, Value, Compare> *cur, Node<Key, Value, Compare> *parent) {
        if (!cur) return nullptr;

        auto cloned = std::make_shared<Node<Key, Value, Compare> >(
            cur->data_.first,
            cur->data_.second,
            parent);

        cloned->left_ = clone_node(cur->left_.get(), cloned.get());
        cloned->right_ = clone_node(cur->right_.get(), cloned.get());
        cloned->bf_ = cur->bf_;

        return cloned;
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<Node<Key, Value, Compare> *, bool>
    table<Key, Value, Compare>::find_node_(const Key &key) const {
        Compare cmp{};

        Node<Key, Value, Compare> *current = root_.get();
        Node<Key, Value, Compare> *parent = nullptr;

        while (current) {
            parent = current;

            if (!cmp(current->data_.first, key) && !cmp(key, current->data_.first)) {
                return {current, true};
            }

            if (cmp(key, current->data_.first)) {
                current = current->left_.get();
            } else {
                current = current->right_.get();
            }
        }

        return {parent, false};
    }


    // _______________ BALANCE ________________


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::NodePtr
    table<Key, Value, Compare>::rotate_right(NodePtr node) {
        if (!node || !node->left_) return node;

        NodePtr a = node->left_;

        node->left_ = a->right_;
        if (node->left_) node->left_->parent_ = node.get();

        a->parent_ = node->parent_;
        a->right_ = node;
        a->right_->parent_ = a.get();

        a->right_->update_balance();
        a->update_balance();

        return a;
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::NodePtr
    table<Key, Value, Compare>::rotate_left(NodePtr node) {
        if (!node || !node->right_) return node;

        NodePtr a = node->right_;

        node->right_ = a->left_;
        if (node->right_) node->right_->parent_ = node.get();

        a->parent_ = node->parent_;
        a->left_ = node;
        a->left_->parent_ = a.get();

        a->left_->update_balance();
        a->update_balance();

        return a;
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::NodePtr
    table<Key, Value, Compare>::balance(NodePtr node) {
        if (!node) return nullptr;

        node->update_balance();
        const int bf = node->bf_;

        if (bf < -1 && node->left_ && node->left_->bf_ <= 0) {
            return rotate_right(node);
        }

        if (bf > 1 && node->right_ && node->right_->bf_ >= 0) {
            return rotate_left(node);
        }

        if (bf < -1 && node->left_ && node->left_->bf_ > 0) {
            node->left_ = rotate_left(node->left_);
            if (node->left_) node->left_->parent_ = node.get();
            return rotate_right(node);
        }

        if (bf > 1 && node->right_ && node->right_->bf_ < 0) {
            node->right_ = rotate_right(node->right_);
            if (node->right_) node->right_->parent_ = node.get();
            return rotate_left(node);
        }

        return node;
    }

    template<typename Key, typename Value, typename Compare>
    void table<Key, Value, Compare>::rebalance_from(Node<Key, Value, Compare> *node, bool to_root) {
        Node<Key, Value, Compare> *current = node;

        while (current) {
            current->update_balance();
            int bf = current->bf_;

            if (bf > 1 || bf < -1) {
                Node<Key, Value, Compare> *parent = current->parent_;

                if (!parent) {
                    root_ = balance(root_);
                    if (root_) root_->parent_ = nullptr;
                    return;
                }

                if (parent->left_.get() == current) {
                    NodePtr old_left = parent->left_;
                    parent->left_ = balance(parent->left_);
                    if (parent->left_) {
                        parent->left_->parent_ = parent;
                        current = parent->left_.get();
                    } else {
                        current = nullptr;
                    }
                } else {
                    NodePtr old_right = parent->right_;
                    parent->right_ = balance(parent->right_);
                    if (parent->right_) {
                        parent->right_->parent_ = parent;
                        current = parent->right_.get();
                    } else {
                        current = nullptr;
                    }
                }
            }

            if (current) {
                current = current->parent_;
            } else {
                break;
            }
        }
    }


    // __________________ ERASE HELP ____________________________

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::NodePtr
    table<Key, Value, Compare>::erase_node(NodePtr node) {
        if (!node) return nullptr;

        if (!node->left_ && !node->right_) {
            return nullptr;
        }

        if (!node->left_) {
            node->right_->parent_ = node->parent_;
            return node->right_;
        }

        if (!node->right_) {
            node->left_->parent_ = node->parent_;
            return node->left_;
        }

        NodePtr *max_link = &node->left_;
        Node<Key, Value, Compare> *max_parent = node.get();

        while ((*max_link)->right_) {
            max_parent = max_link->get();
            max_link = &((*max_link)->right_);
        }

        NodePtr max_ptr = *max_link;

        const_cast<Key &>(node->data_.first) = max_ptr->data_.first;
        node->data_.second = max_ptr->data_.second;

        if (max_ptr->left_) {
            (*max_link) = max_ptr->left_;
            (*max_link)->parent_ = max_parent;
        } else {
            (*max_link) = nullptr;
        }

        return node;
    }


    // ___________________ CONSTRUCTORS ______________________

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::table(const table &other) : size_(other.size_) {
        if (other.empty()) return;
        root_ = clone_node(other.root_.get(), nullptr);
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare> &table<Key, Value, Compare>::operator=(const table &other) & {
        if (this == &other) return *this;
        table temp(other);
        this->swap(temp);
        return *this;
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::table(std::initializer_list<value_type> init, const Compare &comp ) noexcept {
        try {
            for (auto &v: init) {
                insert(std::move(v));
            }
        } catch (...) {
            clear();
        }
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare> &table<Key, Value, Compare>::operator=(std::initializer_list<value_type> init) & noexcept {
        table t(std::move(init));
        swap(t);
        return *this;
    }

    // ________________________ ELEMENT ACCESS ______________________

    template<typename Key, typename Value, typename Compare>
    Value &table<Key, Value, Compare>::operator[](const Key &key) & {
        return try_emplace(key).first->second;
    }

    template<typename Key, typename Value, typename Compare>
    Value &table<Key, Value, Compare>::at(const Key &key) & {
        auto res = find(key);

        if (res == end())
            throw std::out_of_range("Key not found in table");

        return res->second;
    }

    template<typename Key, typename Value, typename Compare>
    const Value &table<Key, Value, Compare>::at(const Key &key) const & {
        auto res = find(key);

        if (res == end())
            throw std::out_of_range("Key not found in table");

        return res->second;
    }

    // _______________________ ITERATORS __________________________

    template<typename Key, typename Value, typename Compare>
    Node<Key, Value, Compare> *table<Key, Value, Compare>::find_node_for_forward_iterator() const {
        auto *node = this->root_.get();
        if (!node) return nullptr;
        while (node->left_) node = node->left_.get();
        return node;
    }


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::begin() {
        return iterator{find_node_for_forward_iterator()};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::begin() const {
        return const_iterator{find_node_for_forward_iterator()};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::cbegin() const {
        return const_iterator{find_node_for_forward_iterator()};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::end() {
        return iterator{nullptr};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::end() const {
        return const_iterator{nullptr};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::cend() const {
        return const_iterator{nullptr};
    }


    // _______________________ MODIFIERS ________________________

    template<typename Key, typename Value, typename Compare>
    void table<Key, Value, Compare>::clear() noexcept {
        root_.reset();
        size_ = 0;
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::insert(const value_type &value) {
        return insert(value_type{value});
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::insert(value_type &&value) {
        if (!root_) {
            root_ = std::make_shared<Node<Key, Value, Compare> >(
                value.first,
                std::move(value.second),
                nullptr
            );
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, logic] = find_node_(value.first);

        if (logic) {
            return {iterator{node}, false};
        }

        Compare cmp{};

        bool insert_left = cmp(value.first, node->data_.first);


        auto new_node = std::make_shared<Node<Key, Value, Compare> >(
            value.first,
            std::move(value.second),
            node
        );

        Node<Key, Value, Compare> *new_node_ptr = new_node.get();

        if (insert_left) {
            node->left_ = new_node;
        } else {
            node->right_ = new_node;
        }

        ++size_;
        rebalance_from(node);

        return {iterator{new_node_ptr}, true};
    }


    template<typename Key, typename Value, typename Compare>
    template<typename I>
    void table<Key, Value, Compare>::insert(I first, I last) {
        std::vector<Key> inserted;
        inserted.reserve(std::distance(first, last));

        try {
            for (auto it = first; it != last; ++it) {
                auto [i, f] = insert(*it);
                if (f) {
                    inserted.push_back(i->first);
                }
            }
        } catch (...) {
            for (const auto &key : inserted)
                erase(key);
            throw;
        }

    }

    template<typename Key, typename Value, typename Compare>
    template<typename I>
    void table<Key, Value, Compare>::assign(I first, I last) {
        table t;
        for (auto it = first; it != last; ++it)
            t.insert(*it);
        this->swap(t);
    }

    template<typename Key, typename Value, typename Compare>
    void table<Key, Value, Compare>::assign(std::initializer_list<value_type> init) {
        table t(init);
        this->swap(t);
    }


    template<typename Key, typename Value, typename Compare>
    void table<Key, Value, Compare>::insert(std::initializer_list<value_type> init) {
        for (const auto &elem: init) {
            insert(elem);
        }
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::insert_or_assign(const value_type &value) {
        if (!root_) {
            root_ = std::make_shared<Node<Key, Value, Compare> >(value.first, value.second, nullptr);
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, logic] = find_node_(value.first);

        if (logic) {
            node->data_.second = value.second;
            return {iterator{node}, false};
        }

        Compare cmp{};

        auto new_node = std::make_shared<Node<Key, Value, Compare> >(value.first, value.second, node);
        Node<Key, Value, Compare> *new_node_ptr = new_node.get();

        if (cmp(value.first, node->data_.first))
            node->left_ = (new_node);
        else
            node->right_ = (new_node);

        ++size_;
        rebalance_from(node);

        return {iterator{new_node_ptr}, true};
    }


    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::insert_or_assign(value_type &&value) {
        if (!root_) {
            root_ = std::make_shared<Node<Key, Value, Compare> >(
                value.first,
                std::move(value.second),
                nullptr
            );
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, found] = find_node_(value.first);

        if (found) {
            node->data_.second = std::move(value.second);
            return {iterator{node}, false};
        }

        Compare cmp{};

        bool insert_left = cmp(value.first, node->data_.first);

        auto new_node = std::make_shared<Node<Key, Value, Compare> >(
            value.first,
            std::move(value.second),
            node
        );
        Node<Key, Value, Compare> *new_node_ptr = new_node.get();

        if (insert_left) {
            node->left_ = new_node;
        } else {
            node->right_ = new_node;
        }

        ++size_;
        rebalance_from(node);

        return {iterator{new_node_ptr}, true};
    }


    template<typename Key, typename Value, typename Compare>
    template<class... Args>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::emplace(Args &&... args) {
        std::shared_ptr<Node<Key, Value, Compare>> _node_ = std::make_shared<Node<Key, Value, Compare>>(std::forward<Args>(args)...);

        if (!root_) {
            root_ = _node_;
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, logic] = find_node_(_node_->data_.first);
        if (logic)
            return {iterator{node}, false};

        Compare cmp{};

        _node_->parent_ = node;

        if (cmp(_node_.get()->data_.first, node->data_.first))
            node->left_ = _node_;
        else
            node->right_ = _node_;
        ++size_;
        rebalance_from(node);
        return {iterator{_node_.get()}, true};
    }


    template<typename Key, typename Value, typename Compare>
    template<class... Args>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::emplace_hint(const_iterator pos, Args &&... args) {
        return emplace(std::forward<Args>(args)...).first;
    }

    template<typename Key, typename Value, typename Compare>
    template<class... Args>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::try_emplace(const Key &k, Args &&... args) {
        if (!root_) {
            root_ = std::make_shared<Node<Key, Value, Compare> >(
                k,
                Value(std::forward<Args>(args)...),
                nullptr
            );
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, logic] = find_node_(k);

        if (logic)
            return {iterator{node}, false};

        Compare cmp{};

        auto new_node = std::make_shared<Node<Key, Value, Compare> >(k, Value(std::forward<Args>(args)...), node);
        Node<Key, Value, Compare> *new_node_ptr = new_node.get();

        if (cmp(k, node->data_.first))
            node->left_ = (new_node);
        else
            node->right_ = (new_node);

        ++size_;
        rebalance_from(node);

        return {iterator{new_node_ptr}, true};
    }


    template<typename Key, typename Value, typename Compare>
    template<class... Args>
    std::pair<typename table<Key, Value, Compare>::iterator, bool>
    table<Key, Value, Compare>::try_emplace(Key &&k, Args &&... args) {
        if (!root_) {
            root_ = std::make_shared<Node<Key, Value, Compare> >(
                std::move(k),
                Value(std::forward<Args>(args)...),
                nullptr
            );
            size_ = 1;
            return {iterator{root_.get()}, true};
        }

        auto [node, logic] = find_node_(k);

        if (logic)
            return {iterator{node}, false};

        Compare cmp{};

        auto new_node = std::make_shared<Node<Key, Value, Compare> >(std::move(k), Value(std::forward<Args>(args)...),
                                                                     node);
        Node<Key, Value, Compare> *new_node_ptr = new_node.get();

        if (cmp(new_node_ptr->data_.first, node->data_.first))
            node->left_ = (new_node);
        else
            node->right_ = (new_node);

        ++size_;
        rebalance_from(node);

        return {iterator{new_node_ptr}, true};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::erase(iterator pos) noexcept {
        return erase(const_iterator(pos));
    }


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::erase(const_iterator pos) noexcept {
        if (pos == end() || empty()) return end();

        Node<Key, Value, Compare> *del = pos.node_;

        Key next_key;
        bool has_next = false;

        const_iterator next = pos;
        ++next;
        if (next != end()) {
            next_key = next->first;
            has_next = true;
        }

        Node<Key, Value, Compare> *parent = del->parent_;

        if (!parent) {
            root_ = erase_node(std::move(root_));
            if (root_) {
                root_->parent_ = nullptr;
                rebalance_from(root_.get(), true);
            }
        } else {
            if (parent->left_.get() == del) {
                parent->left_ = erase_node(std::move(parent->left_));
                if (parent->left_) parent->left_->parent_ = parent;
            } else {
                parent->right_ = erase_node(std::move(parent->right_));
                if (parent->right_) parent->right_->parent_ = parent;
            }

            rebalance_from(parent, true);
        }

        --size_;

        if (has_next) {
            return iterator{find_needed_node(next_key)};
        }

        return end();
    }


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::erase(const_iterator first, const_iterator last) noexcept {
        while (first != last) {
            first = erase(first);
        }
        return iterator{last.node_};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::size_type
    table<Key, Value, Compare>::erase(const Key &key) noexcept {
        table_iterator<Key, Value, Compare, false> it = find(key);
        if (it == end()) return 0;
        erase(it);
        return 1;
    }

    template<typename Key, typename Value, typename Compare>
    void table<Key, Value, Compare>::swap(table &other) noexcept {
        std::swap(root_, other.root_);
        std::swap(size_, other.size_);
    }

    // _______________________ LOOKUP ____________________

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::size_type
    table<Key, Value, Compare>::count(const Key &key) const noexcept {
        return static_cast<size_type>(contains(key));
    }

    template<typename Key, typename Value, typename Compare>
    Node<Key, Value, Compare> *table<Key, Value, Compare>::find_needed_node(const Key &key) const {
        auto *current = this->root_.get();
        Compare cmp{};

        while (current) {
            if (!cmp(current->data_.first, key) && !cmp(key, current->data_.first)) {
                return current;
            }
            if (cmp(key, current->data_.first)) {
                current = current->left_.get();
            } else {
                current = current->right_.get();
            }
        }

        return nullptr;
    }


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::find(const Key &key) {
        if (!root_) return iterator{nullptr};
        return iterator{find_needed_node(key)};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::find(const Key &key) const {
        if (!root_) return const_iterator{nullptr};
        return const_iterator{find_needed_node(key)};
    }

    template<typename Key, typename Value, typename Compare>
    bool table<Key, Value, Compare>::contains(const Key &key) const noexcept {
        return find(key) != end();
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::iterator, typename table<Key, Value, Compare>::iterator>
    table<Key, Value, Compare>::equal_range(const Key &key) {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Value, typename Compare>
    std::pair<typename table<Key, Value, Compare>::const_iterator, typename table<Key, Value, Compare>::const_iterator>
    table<Key, Value, Compare>::equal_range(const Key &key) const {
        return {lower_bound(key), upper_bound(key)};
    }

    template<typename Key, typename Value, typename Compare>
    Node<Key, Value, Compare> *table<Key, Value, Compare>::ul_find_needed_node(const Key &key, bool isUpper) const {
        Node<Key, Value, Compare> *cur = root_.get();
        Node<Key, Value, Compare> *candidate = nullptr;
        Compare cmp{};

        while (cur) {
            if (isUpper) {
                if (cmp(key, cur->data_.first)) {
                    candidate = cur;
                    cur = cur->left_.get();
                } else cur = cur->right_.get();
            } else {
                if (!cmp(cur->data_.first, key)) {
                    candidate = cur;
                    cur = cur->left_.get();
                } else cur = cur->right_.get();
            }
        }
        return candidate;
    }


    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::lower_bound(const Key &key) {
        return iterator{ul_find_needed_node(key, false)};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::lower_bound(const Key &key) const {
        return const_iterator{ul_find_needed_node(key, false)};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::iterator
    table<Key, Value, Compare>::upper_bound(const Key &key) {
        return iterator{ul_find_needed_node(key, true)};
    }

    template<typename Key, typename Value, typename Compare>
    table<Key, Value, Compare>::const_iterator
    table<Key, Value, Compare>::upper_bound(const Key &key) const {
        return const_iterator{ul_find_needed_node(key, true)};
    }

    // _______________ ELSE METODS ___________________

    template<typename Key, typename Value, typename Compare>
    bool operator==(const table<Key, Value, Compare> &lhs,
                    const table<Key, Value, Compare> &rhs) {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template<typename Key, typename Value, typename Compare>
    auto operator<=>(const table<Key, Value, Compare> &lhs,
                     const table<Key, Value, Compare> &rhs) {
        auto it1 = lhs.begin();
        auto it2 = rhs.begin();
        auto end1 = lhs.end();
        auto end2 = rhs.end();

        for (; it1 != end1 && it2 != end2; ++it1, ++it2) {
            const auto &[k1, v1] = *it1;
            const auto &[k2, v2] = *it2;

            if (auto cmp = (k1 <=> k2); cmp != 0) {
                return cmp;
            }
            if (auto cmp = (v1 <=> v2); cmp != 0) {
                return cmp;
            }
        }

        if (it1 == end1 && it2 == end2)
            return std::strong_ordering::equal;
        if (it1 == end1)
            return std::strong_ordering::less;
        return std::strong_ordering::greater;
    }


    template<typename Key, typename Value, typename Compare>
    void swap(table<Key, Value, Compare> &lhs, table<Key, Value, Compare> &rhs) noexcept {
        lhs.swap(rhs);
    }

    template<typename Key, typename Value, typename Compare, typename Pred>
    table<Key, Value, Compare>::size_type
    erase_if(table<Key, Value, Compare> &c, Pred pred) {
        auto old_size = c.size();

        std::vector<Key> to_erase;
        for (const auto it: c) {
            if (pred(it))
                to_erase.push_back(std::ref(it.first));
        }


        for (const auto &key: to_erase) {
            c.erase(key);
        }

        return old_size - c.size();
    }
};