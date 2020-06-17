#include <random>
#include <iostream>
#include "k_tree.hpp"

using tree_ = k_tree::tree<int>;

int main(){
    const int size = 100;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 4);
    tree_ t;
    t.set_root(0);
    int resulting_size=1;
    for(int i=0; i < size; i++){
        auto num = dist(gen);
        std::uniform_int_distribution<int> node_dist(0, t.size()-1);
        auto node_num = node_dist(gen);
        std::cout<<"selected node:"<<node_num<<"\t";
        auto it = std::next(t.begin(), node_num);
        resulting_size++;
        if(num == 0){
            std::cout<<"inserting:";
            *it = i;
            resulting_size--;
        }else if(num == 1){
            std::cout<<"inserting left:";
            t.insert_left(it, i);
        }else if(num == 2){
            std::cout<<"inserting right:";
            t.insert_right(it, i);
        }else if(num == 3){
            std::cout<<"appending child:";
            t.append_child(it, i);
        }else if(num == 4){
            std::cout<<"prepending child:";
            t.prepend_child(it, i);
        }
        std::cout<<i<<std::endl;
    }
    std::cout<<"tree size:"<<t.size()<<"\twanted nodes num:"<<resulting_size<<std::endl;
    assert(t.size() == resulting_size);
    return 0;
}
