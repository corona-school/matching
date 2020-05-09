#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "graph.h"
#include "heap.h"
#include <algorithm>

struct HeapItem
{
    HeapItem(Graph::NodeId nodeid, double key): _nodeid(nodeid), _key(key) {}
    Graph::NodeId _nodeid;
    double _key;
};

bool operator<(const HeapItem & a, const HeapItem & b)
{
    return (a._key < b._key);
}


class NodeHeap : public Heap<HeapItem> {
public:
    NodeHeap(int num_nodes): _heap_node(num_nodes, not_in_heap)
    {   // creates a heap with all nodes having key = inifinte weight
        for(auto i = 0; i < num_nodes; ++i) {
            insert(i, Graph::infinite_weight);
        }
    }

    bool is_member(Graph::NodeId nodeid) const
    {
        ensure_is_valid_nodeid(nodeid);
        return _heap_node[nodeid] != not_in_heap;
    }

    double get_key(Graph::NodeId nodeid)
    {
        return get_object(_heap_node[nodeid])._key;
    }

    Graph::NodeId extract_min()
    {
        Graph::NodeId result = Heap<HeapItem>::extract_min()._nodeid;
        _heap_node[result] = not_in_heap;
        return result;
    }

    void insert(Graph::NodeId nodeid, double key)
    {
          ensure_is_valid_nodeid(nodeid);
          HeapItem item(nodeid, key);
          _heap_node[nodeid] = Heap<HeapItem>::insert(item);
    }

    void decrease_key(Graph::NodeId nodeid, double new_key)
    {
        ensure_is_valid_nodeid(nodeid);
        get_object(_heap_node[nodeid])._key = new_key;
        Heap<HeapItem>::decrease_key(_heap_node[nodeid]);
    }

    void remove(Graph::NodeId nodeid)
    {
        ensure_is_valid_nodeid(nodeid);
        Heap<HeapItem>::remove(_heap_node[nodeid]);
        _heap_node[nodeid] = not_in_heap;
    }

private:

    void ensure_is_valid_nodeid(Graph::NodeId nodeid) const
    {
        if (nodeid < 0 or nodeid >= static_cast<int>(_heap_node.size()))
            throw std::runtime_error("invalid nodeid in NodeHeap");
    }

    void swap(HeapItem & a, HeapItem & b)
    {
        std::swap(a,b);
        std::swap(_heap_node[a._nodeid],_heap_node[b._nodeid]);
    }

    static const int not_in_heap;
    std::vector<int> _heap_node;
};

int const NodeHeap::not_in_heap = -1;


struct PrevData {
    Graph::NodeId id;
    double weight;
};

std::vector<Graph::NodeId> dijkstra(Graph & g, Graph::NodeId start_nodeid, Graph::NodeId end_nodeid)
{   // Dijkstra's Algorithm. The graph g can be directed or undirected.
    std::vector<Graph::NodeId> shortest_path = {};
    NodeHeap heap(g.num_nodes());
    std::vector<PrevData> prev(g.num_nodes(), {Graph::invalid_node, 0.0});
    int flag = 0;

    heap.decrease_key(start_nodeid, 0);

    while (not heap.is_empty()) {
        double key = heap.find_min()._key;
        if (key == Graph::infinite_weight) {
            break;
        }
        Graph::NodeId nodeid = heap.extract_min();
        for (auto neighbor: g.get_node(nodeid).adjacent_nodes()) {
            if (heap.is_member(neighbor.id()) and
                (key + neighbor.get_edge_weight() < heap.get_key(neighbor.id())))
            {
                prev[neighbor.id()] = {nodeid, neighbor.get_edge_weight()};
                heap.decrease_key(neighbor.id(), key + neighbor.get_edge_weight());
                g.get_node(neighbor.id()).set_potential(key + neighbor.get_edge_weight());
                if(neighbor.id() == end_nodeid) {
                    flag = 1;
                }
            }
        }
    }

    if(flag == 1){
        Graph::NodeId nodeid = end_nodeid;
        shortest_path.push_back(nodeid);

        while(nodeid != start_nodeid){
            nodeid = prev[nodeid].id;
            shortest_path.push_back(nodeid);
        }

        std::reverse(shortest_path.begin(), shortest_path.end());
    }

    return shortest_path;
}

#endif // GRAPH_ALGORITHMS_H
