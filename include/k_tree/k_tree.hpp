#pragma once
#include <iterator>
#include <deque>
#include <cassert>
#include <queue>
#include <functional>

namespace k_tree{

namespace algo{
/**
 * Gives depth-distance between two iterators.
 * Counting from lhs to rhs, going up.
 * @param lhs iterator to compute depth-distance.
 * @param rhs target iterator.
 * @return depth-distance from lsh to rhs.
 *      Zero if iterators are not parent-related.
 */
template<class It>
static typename It::difference_type depth_from(const It &lhs, const It &rhs);//TODO:move to alg
/**
 * Checks if lhs is parent from rhs
 * Counting from lhs to rhs, going up.
 * @param lhs iterator to check parent-relation
 * @param rhs target iterator.
 * @return parent-relation. "true" if iterators are parent-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_parent_to(const It &lhs, const It &rhs);//TODO:move to alg
/**
 * Checks if lhs is left from rhs
 * Counting from lhs to rhs, going right.
 * @param lhs iterator to check left-relation
 * @param rhs target iterator.
 * @return left-relation. "true" if iterators are left-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_left_to(const It &lhs, const It &rhs);//TODO:move to alg
/**
 * Checks if lhs is right from rhs
 * Counting from lhs to rhs, going left.
 * @param lhs iterator to check right-relation
 * @param rhs target iterator.
 * @return right-relation. "true" if iterators are right-related,
 *      "false" otherwise.
 */
template<class It>
static bool is_right_to(const It &lhs, const It &rhs);//TODO:move to alg
};

template<class T>
class tree{
    /**
     * Node struct for k_tree
     * Contains pointers to parent, left and right neighbours,
     * begin and end of children
     */
    struct node{
        node* parent; /**< Parent of a node */
        node* left, /**< Left neighbour of a node */
            * right; /**< Right neighbour of a node */
        node* child_begin, /**< Pointer to childrens begin */
            *child_end; /**< Pointer to childrens end */
        T value; /**< Templated value of a node */
        /**
         * Default constructor
         * Initializes pointers to other nodes to nullptr
         */
        node();
    };
public:
    /**
     * Iterator base class
     */
    class iterator_base{
    protected:
        friend class tree;
        /**
         * Protected constructor
         * @param n node for an iterator
         */
        iterator_base(node* n);
    public:
        node* n; /**< Node of an iterator */
        typedef iterator_base self_type;
        typedef T value_type;
        typedef T& reference;
        typedef T* pointer;
        typedef size_t difference_type;
        typedef std::forward_iterator_tag iterator_category;

        /**
         * Copy constructor
         * @param rhs rvalue of a copying
         */
        iterator_base(const iterator_base &rhs);
        /**
         * Dereference operator
         * @return reference of a node value
         */
        auto& operator*();
        /**
         * Const-dereference operator
         * @return const-reference of a node value
         */
        const auto& operator*()const;
        /**
         * Equal operator
         * @param rhs rvalue to compare to
         */
        bool operator==(const iterator_base &rhs)const;
        /**
         * Non-equal operator
         * @param rhs rvalue to compare to
         */
        bool operator!=(const iterator_base &rhs)const;
    };

    /**
     * Depth-first iterator class
     * First, it iterates through node's children recursively from left to right.
     * Second, it iterates through node's neighbours recursively from left to right.
     */
    class depth_first_iterator:public iterator_base{
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        depth_first_iterator(node* n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        depth_first_iterator(const iterator_base &rhs);
        /**
         * Increment operator
         * @return reference to current iterator
         */
        auto& operator++();
        auto operator++(int){
            auto copy = *this;
            ++(*this);
            return copy;
        }
        /**
         * Decrement operator
         * @return reference to current iterator
         */
        auto& operator--();
        auto operator--(int){
            auto copy = *this;
            --(*this);
            return copy;
        }
    };

    /**
     * Depth-first reverse iterator class
     * First, it iterates through node's children recursively from right to left.
     * Second, it iterates through node's neighbours recursively from right to left.
     */
    class depth_first_reverse_iterator:public depth_first_iterator{
    protected:
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        depth_first_reverse_iterator(node* n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        depth_first_reverse_iterator(const iterator_base &rhs);
        /**
         * Increment operator
         * @return reference to current iterator
         */
        auto& operator++();
        auto operator++(int){
            auto copy = *this;
            ++(*this);
            return copy;
        }
        /**
         * Decrement operator
         * @return reference to current iterator
         */
        auto& operator--();
        auto operator--(int){
            auto copy = *this;
            --(*this);
            return copy;
        }
    };

    /**
     * Breadth-first iterator
     * Iterates through tree nodes from left to right.
     * Classic queue approach.
     */ 
    class breadth_first_iterator:public iterator_base {
        node* end;
    public:
        /**
         * Constructor
         * @param n node for an iterator
         */
        breadth_first_iterator(node* n);
        /**
         * Copy Constructor
         * @param rhs rvalue of a copying
         */
        breadth_first_iterator(const iterator_base &rhs);
        /**
         * Increment operator
         * @return reference to current iterator
         */
        auto& operator++();
        auto  operator++(int){
            auto copy = *this;
            ++(*this);
            return copy;
        }
        auto& operator+=(unsigned int n){
            while(n > 0) {
                ++(*this);
                --n;
            }
            return *this;
        }
    private:
        std::queue<node *> q;
    };
private:

    node* root, /** Begin of a tree, has value */
        *foot; /** End of a tree, hasn't value */
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
        auto prev_dist = algo::depth_from(it, rhs.begin());
        std::deque<std::pair<node*, size_t>> queue;
        while(it != rhs.begin()){
            auto tmp = new node();
            tmp->value = it.n->value;
            auto it_dist = algo::depth_from(it, rhs.begin());
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

    /**
     * Copy/move constructor for a value
     */
    tree(T&& val);
    /**
     * Copy constructor, copies tree structire and values
     */
    tree(const tree<T> &rhs);
    /**
     * Move constructor, moves entire tree
     * NOTE: move constructor is far more optimized
     */
    tree(tree<T> &&rhs);
    /**
     * Default constructor
     */
    tree();
    /**
     * Destructor
     */
    ~tree();
    /**
     * Assign copy operator, clears current tree,
     * copies rhs structure and values
     */
    auto& operator=(const tree<T> &rhs);
    /**
     * Assign move operator, clears current tree,
     * copies rhs structure and values.
     * NOTE: move assigment is far more optimized
     */
    auto& operator=(tree<T> &&rhs);
    /**
     * Checks if root's address equals foot's address, hence tree is empty.
     */
    bool empty()const;
    /**
     * Clears current tree, resets it's structure
     */
    void clear();
    /**
     * Erases given iterator of any iterator-based class
     * @param it iterator to erase
     * @return next iterator of given iterator
     */
    template<class It>
    auto erase(const It &it);
    /**
     * Sets root value, copy or move.
     * If tree is empty, inserts root node.
     */
    template<class X>
    auto set_root(X&& val);
    /**
     * Returns depth-first iterator to root of a tree
     * @return depth-first iterator to root of a tree
     */
    auto begin()const;
    /**
     * Returns depth-first iterator to foot of a tree
     * @return depth-first iterator to foot of a tree
     */
    auto end()const;
    /**
     * Inserts value left from given iterator (left neighbour)
     * @param it iterator for relative left insert
     * @param val rhs of inserting, copy or move
     */
    template<class X>
    auto insert_left(iterator_base& it, X&& val);
    /**
     * Inserts value right from given iterator (right neighbour)
     * @param it iterator for relative right insert
     * @param val rhs of inserting, copy or move
     */
    template<class X>
    auto insert_right(iterator_base& it, X&& val);
    /**
     * Appends child with value to a given iterator (right-most child)
     * @param it iterator for child append
     * @param val rhs for appending, copy or move
     * @return depth-first iterator of resulting child
     */
    template<class X>
    auto append_child(const iterator_base& it, X&& val);
    /**
     * Prepends child with value to a given iterator (left-most child)
     * @param it iterator for child prepend
     * @param val rhs for prepending, copy or move
     * @return depth-first iterator of resulting child
     */
    template<class X>
    auto prepend_child(const iterator_base& it, X&& val);
    /**
     * Equals operator
     * Checks if rhs structure and values are equeal to current tree.
     * @param rhs tree to check equality
     * @return Equality. "true" if trees are equal. "false" otherwise.
     */
    bool operator==(const tree<T> &rhs)const;
    /**
     * Non-equals operator
     * Checks if rhs structure and values are not equeal to current tree.
     * @param rhs tree to check non-equality
     * @return Non-equality. "true" if trees are non-equal.
     *      "false" otherwise.
     */
    bool operator!=(const tree<T> &rhs)const;
};

//*** node ***
template<class T>
tree<T>::node::node() {
    parent = nullptr;
    left = right = nullptr;
    child_begin = child_end = nullptr;
}

//*** iterator_base ***
template<class T>
tree<T>::iterator_base::iterator_base(node* n) {
    this->n = n;
}

template<class T>
tree<T>::iterator_base::iterator_base(const iterator_base &rhs) {
    this->n = rhs.n;
}

template<class T>
auto& tree<T>::iterator_base::operator*(){
    return n->value;
}

template<class T>
const auto& tree<T>::iterator_base::operator*()const{
    return n->value;
}

template<class T>
bool tree<T>::iterator_base::operator==(const iterator_base &rhs)const{
    return this->n == rhs.n;
}

template<class T>
bool tree<T>::iterator_base::operator!=(const iterator_base &rhs)const{
    return this->n != rhs.n;
}

//*** depth_first_iterator ***
template<class T>
tree<T>::depth_first_iterator::depth_first_iterator(node* n)
    :iterator_base(n)
{}

template<class T>
tree<T>::depth_first_iterator::depth_first_iterator(const iterator_base &rhs)
    :iterator_base(rhs)
{}

template<class T>
auto& tree<T>::depth_first_iterator::operator++(){
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

template<class T>
auto& tree<T>::depth_first_iterator::operator--(){
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

template<class T>
tree<T>::depth_first_reverse_iterator::
    depth_first_reverse_iterator(node* n)
    :depth_first_iterator(n)
{}

template<class T>
tree<T>::depth_first_reverse_iterator::
    depth_first_reverse_iterator(const iterator_base &rhs)
    :depth_first_iterator(rhs)
{}

template<class T>
auto& tree<T>::depth_first_reverse_iterator::operator++(){
    return depth_first_iterator::operator--();
}

template<class T>
auto& tree<T>::depth_first_reverse_iterator::operator--(){
    return depth_first_iterator::operator--();
}

template<class T>
tree<T>::breadth_first_iterator::breadth_first_iterator(node* n)
    :iterator_base(n)
{
    q.emplace(n);
}

template<class T>
tree<T>::breadth_first_iterator::
    breadth_first_iterator(const iterator_base &rhs)
    :iterator_base(rhs)
{
    q.emplace(rhs.n);
}

template<class T>
auto& tree<T>::breadth_first_iterator::operator++(){
    if(this->n->right){
        if(this->n->parent && this->n->right){//it's not foot
            this->n = this->n->right;
            q.emplace(this->n);
            return *this;
        }else{
            this->end = this->n->right; //save foot
        }
    }
    //find first node with children in queue
    node* top;
    do{
        top = q.front();
        q.pop();
    }while(!top->child_begin && !q.empty());
    if(!top->child_begin){
        this->n = end;
    }else{
        this->n = top->child_begin;
        q.emplace(this->n);
    }
    return *this;
}

template<class T>
tree<T>::tree(T&& val)
    :tree()
{
    set_root(std::forward<T>(val));
}

template<class T>
tree<T>::tree(const tree<T> &rhs)
    :tree()
{
    p_transfer(rhs);
}

template<class T>
tree<T>::tree(tree<T> &&rhs) {
    this->root = rhs.root;
    this->foot = rhs.foot;
    rhs.root = nullptr;
    rhs.foot = nullptr;
}

template<class T>
tree<T>::tree(){
    p_init();
}

template<class T>
tree<T>::~tree(){
    p_erase_children(root, foot);
}

template<class T>
auto& tree<T>::operator=(const tree<T> &rhs){
    clear();
    p_transfer(rhs);
    return *this;
}

template<class T>
auto& tree<T>::operator=(tree<T> &&rhs){
    p_erase_children(root, foot);
    this->root = rhs.root;
    this->foot = rhs.foot;
    rhs.root = nullptr;
    rhs.foot = nullptr;
    return *this;
}

template<class T>
bool tree<T>::empty()const{
    return this->root == this->foot;
}

template<class T>
void tree<T>::clear(){
    if(root == foot){
        return;
    }
    erase(depth_first_iterator(root));
}

template<class T> template<class It>
auto tree<T>::erase(const It &it){
    assert(it.n != foot);
    if(it.n->child_begin){
        p_erase_children(it.n->child_begin, it.n->child_end);
    }
    It bak = (it.n->right)?
        It(it.n->right):
        It(it.n->parent);
    if(it.n->left){
        it.n->left->right = it.n->right;
    }
    if(it.n->right){
        it.n->right->left = it.n->left;
    }
    if(it.n->parent){
        if(it.n->parent->child_begin == it.n){
            it.n->parent->child_begin = it.n->right;
        }
        if(it.n->parent->child_end == it.n){
            it.n->parent->child_end = it.n->left;
        }
    }
    if(it.n == root){
        root = foot;
    }
    delete it.n;
    return bak;
}

template<class T> template<class X>
auto tree<T>::set_root(X&& val){
    if(root == foot){
        foot = new node();
        root->right = foot;
        foot->left = root;
    }
    this->root->value = std::forward<X>(val);
    return depth_first_iterator(this->root);
}

template<class T>
auto tree<T>::begin()const{
    return depth_first_iterator(this->root);
}

template<class T>
auto tree<T>::end()const{
    return depth_first_iterator(this->foot);
}

template<class T> template<class X>
auto tree<T>::insert_left(iterator_base& it, X&& val){
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

template<class T> template<class X>
auto tree<T>::insert_right(iterator_base& it, X&& val){
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

template<class T> template<class X>
auto tree<T>::append_child(const iterator_base& it, X&& val){
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

template<class T> template<class X>
auto tree<T>::prepend_child(const iterator_base& it, X&& val){
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

template<class T>
bool tree<T>::operator==(const tree<T> &rhs)const{
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
        if(algo::is_parent_to(it, it_bak) != algo::is_parent_to(this_it, this_it_bak) ||
            algo::is_left_to(it, it_bak) != algo::is_left_to(this_it, this_it_bak) ||
            algo::is_right_to(it, it_bak) != algo::is_right_to(this_it, this_it_bak))
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

template<class T>
bool tree<T>::operator!=(const tree<T> &rhs)const{
    return !(*this == rhs);
}

template<class It>
typename It::difference_type algo::depth_from(const It &lhs, const It &rhs){
    typename It::difference_type i = 0;
    auto tmp = lhs.n;
    while(tmp->parent){
        tmp = tmp->parent;
        i++;
    }
    if(tmp != rhs.n)
        return 0;
    return i;
}

template<class It>
bool algo::is_parent_to(const It &lhs, const It &rhs){
    return algo::depth_from(lhs, rhs) != 0;
}

template<class It>
bool algo::is_left_to(const It &lhs, const It &rhs){
    return rhs.n && lhs.n->left == rhs.n;
}

template<class It>
bool algo::is_right_to(const It &lhs, const It &rhs){
    return rhs.n && lhs.n->right == rhs.n;
}

};
