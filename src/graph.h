// graph.h (Declaration of Class Graph)
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include "pupil.h"
#include "student.h"

class Graph {
public:
    using NodeId = int;  // vertices are numbered 0,...,num_nodes()-1

    class Neighbor {
    public:
        Neighbor(Graph::NodeId n, double w);
        double get_edge_weight() const;
        void set_edge_weight(double new_edge_weight);
        Graph::NodeId id() const;
    private:
        Graph::NodeId _id;
        double _edge_weight;
    };

    class Node {
    public:
        void add_neighbor(Graph::NodeId nodeid, double weight);
        std::vector<Neighbor> & adjacent_nodes();
        double get_potential();
        void set_potential(double new_feasible_potential);
    private:
        std::vector<Neighbor> _neighbors;
        double _feasible_potential = 0;
    };

    enum DirType {directed, undirected};  // enum defines a type with possible values
    Graph(NodeId num_nodes, DirType dirtype);
    Graph(char const* filename, DirType dirtype);

    void add_nodes(NodeId num_new_nodes);
    void add_edge(NodeId tail, NodeId head, double weight = 1.0);
    void delete_edge(NodeId tail, NodeId head); //to call the delete_edge function we need that we have a simple graph (no edges join same vertices)

    NodeId num_nodes() const;
    Node & get_node(NodeId);
    void print();
    void print_matching(std::vector<Pupil> const &pupil_vector, std::vector<Student> const &student_vector, char const* filename_output);

    const DirType dirtype;
    static const NodeId invalid_node;
    static const double infinite_weight;

private:
    std::vector<Node> _nodes;
};

#endif // GRAPH_H

