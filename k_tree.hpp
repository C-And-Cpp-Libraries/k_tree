#pragma once
#include <iterator>
#include <deque>
#include <functional>

namespace k_tree{

template<class T>
class tree{
    struct node{
        node* parent;
        node* left, *right;
        node* child_begin, *child_end;
        T value;
        node(){
            parent = nullptr;
            left = right = nullptr;
            child_begin = child_end = nullptr;
        }
    };
public:
    class iterator_base{
    protected:
        friend class tree;
        iterator_base(node* n) {
            this->n = n;
        }
    public:
        node* n;
        typedef iterator_base self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef size_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        iterator_base(const iterator_base &rhs) {
            this->n = rhs.n;
        }
        auto& operator*(){
            return n->value;
        }
        const auto& operator*()const{
            return n->value;
        }
        bool operator==(const iterator_base &rhs)const{
            return this->n == rhs.n;
        }
        bool operator!=(const iterator_base &rhs)const{
            return this->n != rhs.n;
        }
    };

    class depth_first_iterator:public iterator_base{
    protected:
    public:
        depth_first_iterator(node* n)
            :iterator_base(n)
        {}
        depth_first_iterator(const iterator_base &rhs)
            :iterator_base(rhs)
        {}
        auto& operator++(){
            if(this->n->child_begin){
                this->n = this->n->child_begin;
            }else{
                while(!this->n->right) {
                    this->n = this->n->parent;
                    if(!this->n){
                        return *this;
                    }
                }
                this->n = this->n->right;
            }
            return *this;
        }
        auto operator++(int){
            auto copy = *this;
            ++(*this);
            return copy;
        }
        auto& operator--(){
            if(this->n->left){
                this->n = this->n->left;
                while(this->n->child_end){
                    this->n = this->n->child_end;
                }
            }else{
                this->n = this->n->parent;
            }
            return *this;
        }
        auto operator--(int){
            auto copy = *this;
            --(*this);
            return copy;
        }
    };
    class depth_first_reverse_iterator:public depth_first_iterator{
    protected:
    public:
        depth_first_reverse_iterator(node* n)
            :depth_first_iterator(n)
        {}
        depth_first_reverse_iterator(const iterator_base &rhs)
            :depth_first_iterator(rhs)
        {}
        auto& operator++(){
            return depth_first_iterator::operator--();
        }
        auto operator++(int){
            auto copy = *this;
            ++(*this);
            return copy;
        }
        auto& operator--(){
            return depth_first_iterator::operator++();
        }
        auto operator--(int){
            auto copy = *this;
            --(*this);
            return copy;
        }
    };
private:

    node* root, *foot;
    void p_init(){
        root = new node();
        foot = root;
    }

    void p_erase_children(node *beg, node *end){
        for(auto n = beg; n!= end;){
            if(n->child_begin){
                auto nbeg = n->child_begin;
                auto nend = n->child_end;
                p_erase_children(nbeg, nend);
            }
            auto bak = n->right;
            delete n;
            n = bak;
        }
        delete end;
    }
    void p_transfer(const tree<T> &rhs){
        if(rhs.empty()){
            return;
        }
        auto it = depth_first_iterator(rhs.end());
        it--;
        auto prev_dist = depth_from(it, rhs.begin());
        std::deque<std::pair<node*, size_t>> queue;
        while(it != rhs.begin()){
            auto tmp = new node();
            tmp->value = it.n->value;
            auto it_dist = depth_from(it, rhs.begin());
            if(it_dist < prev_dist){ //we moved up
                tmp->child_begin = queue.back().first;
                auto bak = tmp->child_begin;
                while(!queue.empty() && queue.back().second != it_dist){
                    auto n = queue.back().first;
                    bak = n;
                    n->parent = tmp;
                    queue.pop_back();
                }
                tmp->child_end = bak;
                tmp->right = queue.back().first;
                queue.back().first->left = tmp;
            }else if(it_dist == prev_dist && !queue.empty()){
                tmp->right = queue.back().first;
                queue.back().first->left = tmp;
            }
            queue.emplace_back(tmp, it_dist);//NOTE:reverse order
            prev_dist = it_dist;
            it--;
        }
        set_root(it.n->value);
        root->child_begin = queue.back().first;
        auto bak = root->child_begin;
        while(!queue.empty()){
            auto n = queue.back().first;
            bak = n;
            n->parent = root;
            queue.pop_back();
        }
        root->child_end = bak;
    }
public:
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using const_pointer = const T*;
    using const_reference = const T&;
    using iterator = depth_first_iterator;
    using const_iterator = const depth_first_iterator;

    tree(const T& val)
        :tree()
    {
        set_root(val);
    }
    tree(T&& val)
        :tree()
    {
        set_root(val);
    }
    tree(const tree<T> &rhs)
        :tree()
    {
        p_transfer(rhs);
    }
    tree(tree<T> &&rhs) {
        this->root = rhs.root;
        this->foot = rhs.foot;
        rhs.root = nullptr;
        rhs.foot = nullptr;
    }
    tree(){
        p_init();
    }
    ~tree(){
        p_erase_children(root, foot);
    }

    auto& operator=(const tree<T> &rhs){
        clear();
        p_transfer(rhs);
        return *this;
    }

    auto& operator=(tree<T> &&rhs){
        p_erase_children(root, foot);
        this->root = rhs.root;
        this->foot = rhs.foot;
        rhs.root = nullptr;
        rhs.foot = nullptr;
        return *this;
    }

    bool empty()const{
        return this->root == this->foot;
    }

    void clear(){
        if(root == foot){
            return;
        }
        erase(depth_first_iterator(root));
    }

    template<class It>
    auto erase(const It &it){
        assert(it.n != foot);
        if(it.n->child_begin){
            p_erase_children(it.n->child_begin, it.n->child_end);
        }
        It bak = (it.n->right)?
            It(it.n->right):
            It(it.n->parent);
        if(it.n->left && it.n->right){
            it.n->left->right = it.n->right;
            it.n->right->left = it.n->left;
        }
        if(it.n == root){
            root = foot;
        }
        delete it.n;
        return bak;
    }

    template<class X>
    auto set_root(X&& val){
        if(root == foot){
            foot = new node();
            root->right = foot;
            foot->left = root;
        }
        this->root->value = std::forward<X>(val);
        return depth_first_iterator(this->root);
    }

    auto begin()const{
        return depth_first_iterator(this->root);
    }

    auto end()const{
        return depth_first_iterator(this->foot);
    }

    template<class X>
    auto insert_left(iterator_base& it, X&& val){
        auto tmp = new node();
        if(it.n->left){
            tmp->left = it.n->left;
            tmp->right = it.n;
            it.n->left->right = tmp;
            it.n->left = tmp;
        }else{
            it.n->left = tmp;
            tmp->right = it.n;
        }
        tmp->parent = it.n->parent;
        if(it.n->parent){
            if(it.n->parent->child_begin == it.n){
                it.n->parent->child_begin = tmp;
            }
        }
        tmp->value = std::forward<X>(val);
    }

    template<class X>
    auto insert_right(iterator_base& it, X&& val){
        auto tmp = new node();
        if(it.n->right){
            tmp->right = it.n->right;
            tmp->left = it.n;
            it.n->right->left = tmp;
            it.n->right = tmp;
        }else{
            it.n->right = tmp;
            tmp->left = it.n;
        }
        tmp->parent = it.n->parent;
        if(it.n->parent){
            if(it.n->parent->child_end == it.n){
                it.n->parent->child_end = tmp;
            }
        }
        tmp->value = std::forward<X>(val);
    }

    template<class X>
    auto append_child(iterator_base& it, X&& val){
        if(!it.n->child_end){ //iterator has no children
            return prepend_child(it, std::forward<X>(val));
        }
        auto tmp = new node();
        tmp->parent = it.n;
        tmp->left = it.n->child_end;
        it.n->child_end->right = tmp;
        it.n->child_end = tmp;
        tmp->value = std::forward<X>(val);
        return depth_first_iterator(tmp);
    }

    template<class X>
    auto prepend_child(iterator_base& it, X&& val){
        auto tmp = new node();
        tmp->parent = it.n;
        if(!it.n->child_begin){
            it.n->child_begin = tmp;
            it.n->child_end = tmp;
        }else{
            it.n->child_begin->left = tmp;
            tmp->right = it.n->child_begin;
            it.n->child_begin = tmp;
        }
        tmp->value = std::forward<X>(val);
        return depth_first_iterator(tmp);
    }

    auto depth_from(const iterator_base &lhs, const iterator_base &rhs)const{
        typename iterator_base::difference_type i = 0;
        auto tmp = lhs.n;
        while(tmp->parent){
            tmp = tmp->parent;
            i++;
        }
        assert(tmp == rhs.n); 
        return i;
    }

    bool is_parent_to(iterator_base &lhs, iterator_base &rhs)const{
        return lhs.n && rhs.n->parent && rhs.n->parent == lhs.n;
    }
    bool is_left_to(iterator_base &lhs, iterator_base &rhs)const{
        return rhs.n && lhs.n->left == rhs.n;
    }
    bool is_right_to(iterator_base &lhs, iterator_base &rhs)const{
        return rhs.n && lhs.n->right == rhs.n;
    }
    bool operator==(const tree<T> &rhs)const{
        auto this_it = begin();
        auto this_it_bak = this_it;
        auto it = rhs.begin();
        auto it_bak = it;
        while(it != rhs.end()){
            if(this_it == end()){
                return false;
            }
            if(*it != *this_it){
                return false;
            }
            if(is_parent_to(it, it_bak) != is_parent_to(this_it, this_it_bak) ||
                is_left_to(it, it_bak) != is_left_to(this_it, this_it_bak) ||
                is_right_to(it, it_bak) != is_right_to(this_it, this_it_bak))
            {
                return false;
            }
            it_bak = it;
            this_it_bak = this_it;
            it++;
            this_it++;
        }
        if(this_it != end()){
            return false;
        }
        return true;
    }
    bool operator!=(const tree<T> &rhs)const{
        return !(*this == rhs);
    }
};

};