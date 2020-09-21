#pragma once
#include <vector>
#include <algorithm>

namespace cxx_graph{

template<class ValT>
class graph{
public:
    using value_type = ValT;
    struct node;
    struct edge;

    struct edge{
        node* m_first=nullptr, * m_second=nullptr;
    };
    struct node{
        using Edges = std::vector<edge*>;
        Edges m_edges={};
        value_type m_value;
    };

    class iterator_base{
    protected:
        friend class graph;
        node* m_node;
        iterator_base(node *n);
    public:
        iterator_base(const iterator_base &it);
        const value_type& operator*()const;
        value_type& operator*();
        friend bool operator==(const iterator_base& lhs, const iterator_base& rhs){
            return lhs.m_node == rhs.m_node;
        }
    };

    class bfs_iterator:public iterator_base{
        using Nodes = std::vector<node*>;
        Nodes m_nodes;
        size_t m_nodes_idx;
    public:
        bfs_iterator(node *n);
        bfs_iterator(const bfs_iterator &it);
        bfs_iterator(bfs_iterator &&it);
        bfs_iterator& operator++();
        bfs_iterator operator++(int);
    };

    class dfs_iterator:public iterator_base{
        std::vector<node*> m_nodes;
    public:
        dfs_iterator(node *n);
        dfs_iterator(const iterator_base &it);
        dfs_iterator& operator++();
        dfs_iterator operator++(int);
    };

    using default_it = bfs_iterator;
public:
    graph();
    graph(const graph &rhs);
    graph(graph &&rhs);
    ~graph();


    template<class Arg>
    default_it insert(Arg &&val);

    template<class Arg>
    default_it add_adjacent(iterator_base &it, Arg &&val);

    void erase(iterator_base &it);

    template<class NodeIt>
    static std::vector<NodeIt> get_adjacent(NodeIt node_it);

    template<class NodeIt>
    static bool is_adjacent(NodeIt node_one_it, NodeIt node_two_it);
private:
    std::vector<node*> m_nodes;
};

template<class ValT>
graph<ValT>::iterator_base::iterator_base(node *n){
    this->m_node = n;
}

template<class ValT>
graph<ValT>::iterator_base::iterator_base(const iterator_base &it)
    :iterator_base(it.m_node)
{}

template<class ValT>
auto graph<ValT>::iterator_base::operator*()const
    ->const typename graph<ValT>::value_type& 
{
    return m_node->m_value;
}

template<class ValT>
auto graph<ValT>::iterator_base::operator*()
    ->typename graph<ValT>::value_type& 
{
    return m_node->m_value;
}

template<class ValT>
graph<ValT>::bfs_iterator::bfs_iterator(node *n)
    :iterator_base(n)
{
    this->m_nodes.emplace_back(n);
    this->m_nodes_idx = 0;
}
template<class ValT>
graph<ValT>::bfs_iterator::bfs_iterator(const bfs_iterator &it)
    :iterator_base(it.m_node)
{
    this->m_nodes = it.m_nodes;
    this->m_nodes_idx = it.m_nodes_idx;
}
template<class ValT>
graph<ValT>::bfs_iterator::bfs_iterator(bfs_iterator &&it)
    :iterator_base(it)
{
    this->m_nodes = std::move(it.m_nodes);
    this->m_nodes_idx = std::move(it.m_nodes_idx);
}

template<class ValT>
auto graph<ValT>::bfs_iterator::operator++()
    ->typename graph<ValT>::bfs_iterator& 
{
    const node* node = *(m_nodes.begin()+m_nodes_idx);
    const auto& edges = node->m_edges;
    auto other_nodes_it = get_adjacent(*this);
    std::vector<struct node*> other_nodes_ptr;
    other_nodes_ptr.reserve(other_nodes_it.size());
    std::transform(other_nodes_it.begin(), other_nodes_it.end(), std::back_inserter(other_nodes_ptr), 
        [](auto &&node_it){ return node_it.m_node; });
    m_nodes.reserve(m_nodes.size()+other_nodes_ptr.size());
    std::copy_if(other_nodes_ptr.begin(), other_nodes_ptr.end(), std::back_inserter(m_nodes),
        [this](auto &&node_ptr){
            return std::find(this->m_nodes.begin(), this->m_nodes.end(), node_ptr) == m_nodes.end();
        });
    this->m_nodes_idx++;
    this->m_node = *(m_nodes.begin()+m_nodes_idx);
    return *this;
}

template<class ValT>
auto graph<ValT>::bfs_iterator::operator++(int)
    ->typename graph<ValT>::bfs_iterator
{
    auto copy = *this;
    ++(*this);
    return copy;
}

template<class ValT>
graph<ValT>::graph(){
}

template<class ValT>
graph<ValT>::graph(const graph &rhs){
}

template<class ValT>
graph<ValT>::graph(graph &&rhs){
}

template<class ValT>
graph<ValT>::~graph(){
    auto node_it = m_nodes.begin();
    while (node_it != m_nodes.end()) {
        auto& node = *node_it;
        auto node_it_graph = iterator_base(node);
        erase(node_it_graph);
        node_it++;
    }
}

template<class ValT>
template<class Arg>
auto graph<ValT>::insert(Arg &&val)
    ->typename graph<ValT>::default_it
{
    auto new_node = new struct node();
    new_node->m_value = std::forward<Arg>(std::move(val));
    m_nodes.emplace_back(new_node);
    return typename graph<ValT>::default_it(new_node);
}

template<class ValT>
template<class Arg>
auto graph<ValT>::add_adjacent(iterator_base &it, Arg &&val)
    ->typename graph<ValT>::default_it
{
    auto& node = it.m_node;
    auto new_node = new struct node();
    new_node->m_value = std::forward<Arg>(std::move(val));
    auto edge = new struct edge();
    edge->m_first = node;
    edge->m_second = new_node;
    new_node->m_edges.emplace_back(edge);
    node->m_edges.emplace_back(edge);
    return typename graph<ValT>::default_it(new_node);
}

template<class ValT>
void graph<ValT>::erase(iterator_base &it){
    auto& node = it.m_node;
    auto& edges = node->m_edges;
    auto edges_it = edges.begin();
    while(edges_it != edges.end()){
        auto& edge = *edges_it;
        auto& other_end = (edge->m_first == node)?
            edge->m_second : edge->m_first;
        auto& other_end_edges = other_end->m_edges;
        auto other_end_edge_it = other_end_edges.begin();
        while(other_end_edge_it != other_end_edges.end()){
            auto& other_end_edge = *other_end_edge_it;
            if(other_end_edge == edge){
                other_end_edge_it = other_end_edges.erase(other_end_edge_it);
                break;
            }
        }
        delete edge;
        edges_it = edges.erase(edges_it);
    }
    delete node;
}

template<class ValT>
template<class NodeIt>
auto graph<ValT>::get_adjacent(NodeIt node_it)
    ->std::vector<NodeIt>
{
    const auto& node = node_it.m_node;
    const auto& edges = node->m_edges;
    std::vector<NodeIt> result;
    result.reserve(edges.size());
    std::transform(edges.begin(), edges.end(), std::back_inserter(result),
        [&node](auto &&edge){
            return (edge->m_first == node)? edge->m_second : edge->m_first;
    });
    return result;
}

template<class ValT>
template<class NodeIt>
auto graph<ValT>::is_adjacent(NodeIt node_one_it, NodeIt node_two_it)
    ->bool
{
    auto& edges_one = node_one_it.m_node->m_edges;
    auto& edges_two = node_two_it.m_node->m_edges;
    return std::find_first_of(edges_one.begin(), edges_one.end(),
        edges_two.begin(), edges_two.end()) != edges_one.end();
}

};