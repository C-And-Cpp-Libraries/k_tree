#include <iostream>
#include <cassert>
#include "k_tree.hpp"
using tree_ = k_tree::tree<int>;

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

int main(){
    /* 0
       |
       1-2-5-7
         |
       6-3-4
       depth-wise: 0 1 2 6 3 4 5 7
    */
    tree_ tree;
    auto it0 = tree.set_root(0);
    tree.append_child(it0, 1);
    auto it2 = tree.append_child(it0, 2);
    auto it3 = tree.append_child(it2, 3);
    tree.append_child(it2, 4);
    auto it5 = tree.append_child(it0, 5);
    tree.insert_left(it3, 6);
    tree.insert_right(it5, 7);

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

    tree_ copy = tree;
    it = copy.begin();
    end = copy.end();
    std::cout<<"begin"<<std::endl;
    print_it(it);
    std::cout<<"end"<<std::endl;
    print_it(end);
    while(it != end){
        std::cout<<"it #"<<std::distance(copy.begin(), it)<<std::endl;
        print_it(it);
        it++;
    }

    assert(copy == tree);
    auto rvalue = std::move(copy);
    assert(rvalue == tree);
}