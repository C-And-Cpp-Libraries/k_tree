#include <iostream>
#include <cassert>
#include "k_tree.hpp"

static size_t alloc_counter = 0;
struct test_struct{
    int val;
    test_struct(){
        val = -1;
        alloc_counter++;
        std::cout<<"default constructed at "<<this<<" cnt:"<<alloc_counter<<"\n";
    }
    test_struct(test_struct &&str){
        std::cout<<"moved:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
    }
    test_struct(const test_struct &str){
        std::cout<<"copied:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
    }
    test_struct(int val){
        this->val = val;
        alloc_counter++;
        std::cout<<"constructed:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
    }
    ~test_struct(){
        alloc_counter--;
        std::cout<<"deleted:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
    }
    auto& operator=(const test_struct &rhs){
        this->val = rhs.val;
        std::cout<<"assigned:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
        return *this;
    }
    auto& operator=(test_struct &&rhs){
        this->val = rhs.val;
        std::cout<<"move-assigned:"<<val<<" at "<<this<<" cnt:"<<alloc_counter<<"\n";
        return *this;
    }
    friend bool operator==(const test_struct &lhs, const test_struct &rhs){
        return lhs.val == rhs.val;
    }
    friend bool operator!=(const test_struct &lhs, const test_struct &rhs){
        return !(lhs == rhs);
    }
    friend std::ostream& operator<<(std::ostream &str, const test_struct &s){
        str<<s.val;
        return str;
    }
};
using tree_ = k_tree::tree<test_struct>;
auto print_it(const tree_::iterator_base &it){
    std::cout<<"iter addr:"<<&it<<std::endl;
    std::cout<<"node addr:"<<it.n<<std::endl;
    if(it.n){
        std::cout<<"val:"<<*it<<std::endl;
    }else{
        std::cout<<"val:null"<<std::endl;
    }
    std::cout<<std::endl;
}

auto make_tree(){
    /*   0
        /|\
       1-2-5
         ^
        3-4
     depth-wise: 0 1 2 3 4 5
    */
    tree_ tree;
    auto it0 = tree.set_root(0);
    tree.append_child(it0, 1);
    auto it2 = tree.append_child(it0, 2);
    tree.append_child(it2, 3);
    tree.append_child(it2, 4);
    auto it5 = tree.append_child(it0, 5);
    return tree;
}

int main(){
    auto bak = alloc_counter;
    {
        auto tree = make_tree();
        auto it = tree.begin();
        auto end = tree.end();
        std::cout<<"begin"<<std::endl;
        print_it(it);
        std::cout<<"end"<<std::endl;
        print_it(end);
        while(it != end){
            std::cout<<"it #"<<std::distance(tree.begin(), it)<<std::endl;
            print_it(it);
            it++;
        }
    }
    assert(bak == alloc_counter);

    bak = alloc_counter;
    {
        auto tree = make_tree();
        tree_ copy = tree;
        auto move = std::move(copy);
    }
    assert(bak == alloc_counter);
}