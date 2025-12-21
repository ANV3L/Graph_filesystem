#pragma once

namespace sstd {
    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst>::operator table_iterator<Key, Value, Compare, true>() const {
        return table_iterator<Key, Value, Compare, true>(node_);
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst>::reference
    table_iterator<Key, Value, Compare, IsConst>::operator*() const {
        return node_->data_;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst>::pointer
    table_iterator<Key, Value, Compare, IsConst>::operator->() const {
        return &(node_->data_);
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst> &
    table_iterator<Key, Value, Compare, IsConst>::operator++() noexcept {
        if (!node_) return *this;
        if (node_->right_) {
            node_ = node_->right_.get();
            while (node_->left_) node_ = node_->left_.get();
        } else {
            NodeT *parent = node_->parent_;
            while (parent && node_ == parent->right_.get()) {
                node_ = parent;
                parent = parent->parent_;
            }
            node_ = parent;
        }
        return *this;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst>
    table_iterator<Key, Value, Compare, IsConst>::operator++(int) noexcept{
        table_iterator temp = *this;
        ++(*this);
        return temp;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst> &
    table_iterator<Key, Value, Compare, IsConst>::operator--() noexcept {
        if (!node_) return *this;
        if (node_->left_) {
            node_ = node_->left_.get();
            while (node_->right_) node_ = node_->right_.get();
        } else {
            NodeT *parent = node_->parent_;
            while (parent && node_ == parent->left_.get()) {
                node_ = parent;
                parent = parent->parent_;
            }
            node_ = parent;
        }
        return *this;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
    table_iterator<Key, Value, Compare, IsConst>
    table_iterator<Key, Value, Compare, IsConst>::operator--(int) noexcept {
        table_iterator temp = *this;
        --(*this);
        return temp;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
        template<bool OtherConst>
        bool table_iterator<Key, Value, Compare, IsConst>::operator==(
            const table_iterator<Key, Value, Compare, OtherConst>& other) const {
        return node_ == other.node_;
    }

    template<typename Key, typename Value, typename Compare, bool IsConst>
        template<bool OtherConst>
        bool table_iterator<Key, Value, Compare, IsConst>::operator!=(
            const table_iterator<Key, Value, Compare, OtherConst>& other) const {
        return node_ != other.node_;
    }
}