#include <iostream>
#include <cassert>
#include "graph.hpp"

int main(){
    using graph = cxx_graph::graph<int>;
    graph gr;
    auto it_0 = gr.insert(0);
    auto it_1 = gr.add_adjacent(it_0, 1);
    assert(graph::is_adjacent(it_0, it_1));
    it_0++;
    assert(it_0 == it_1);
    return 0;
};