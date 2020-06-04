// graph.cpp (Implementation of Class Graph)

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <limits>
#include "graph.h"

const Graph::NodeId Graph::invalid_node = -1;
const double Graph::infinite_weight = std::numeric_limits<double>::max();


void Graph::add_nodes(NodeId num_new_nodes)
{
    _nodes.resize(num_nodes() + num_new_nodes);
}

Graph::Neighbor::Neighbor(Graph::NodeId n, double w): _id(n), _edge_weight(w) {}

Graph::Graph(NodeId num, DirType dtype): dirtype(dtype), _nodes(num) {}

void Graph::add_edge(NodeId tail, NodeId head, double weight)
{
    if (tail >= num_nodes() or tail < 0 or head >= num_nodes() or head < 0) {
        throw std::runtime_error("Edge cannot be added due to undefined endpoint.");
    }
    _nodes[tail].add_neighbor(head, weight);
    if (dirtype == Graph::undirected) {
        _nodes[head].add_neighbor(tail, weight);
    }
}

void Graph::delete_edge(NodeId tail, NodeId head)
{
    for (size_t i = 0; i < _nodes[tail].adjacent_nodes().size(); i++){
        if(_nodes[tail].adjacent_nodes()[i].id() == head) {
            _nodes[tail].adjacent_nodes().erase(_nodes[tail].adjacent_nodes().begin() + i);
        }
    }
}

void Graph::Node::add_neighbor(Graph::NodeId nodeid, double weight)
{
    _neighbors.push_back(Graph::Neighbor(nodeid, weight));
}

std::vector<Graph::Neighbor> & Graph::Node::adjacent_nodes()
{
    return _neighbors;
}

double Graph::Node::get_potential()
{
    return _feasible_potential;
}

void Graph::Node::set_potential(double new_feasible_potential)
{
    _feasible_potential = new_feasible_potential;
}

Graph::NodeId Graph::num_nodes() const
{
    return _nodes.size();
}

Graph::Node & Graph::get_node(NodeId node)
{
    if (node < 0 or node >= static_cast<int>(_nodes.size())) {
        throw std::runtime_error("Invalid nodeid in Graph::get_node.");
    }
    return _nodes[node];
}

Graph::NodeId Graph::Neighbor::id() const
{
    return _id;
}

double Graph::Neighbor::get_edge_weight() const
{
    return _edge_weight;
}

void Graph::Neighbor::set_edge_weight(double new_edge_weight)
{
    _edge_weight = new_edge_weight;
}

void Graph::print()
{
    if (dirtype == Graph::directed) {
        std::cout << "Digraph ";
    } else {
        std::cout << "Undirected graph ";
    }
    std::cout << "with " << num_nodes() << " vertices, numbered 0,...,"
              << num_nodes() - 1 << ".\n";

    for (auto nodeid = 0; nodeid < num_nodes(); ++nodeid) {
        std::cout << "The following edges are ";
        if (dirtype == Graph::directed) {
            std::cout << "leaving";
        } else {
            std::cout << "incident to";
        }
        std::cout << " vertex " << nodeid << ":\n";
        for (auto neighbor: _nodes[nodeid].adjacent_nodes()) {
            std::cout << nodeid << " - " << neighbor.id()
                      << " weight = " << neighbor.get_edge_weight() << "\n";
        }
    }
}

void Graph::print_matching(std::vector<Pupil> const &pupil_vector, std::vector<Student> const &student_vector, char const* filename_output) {
    int num_pupils = pupil_vector.size();
    int num_students = student_vector.size();
    std::ofstream myfile;
    myfile.open(filename_output);
    for (auto nodeid = num_pupils; nodeid < num_pupils + num_students; ++nodeid) {  //we iterate over the students since the students vector is usually smaller than the pupils vector
        for (auto neighbor: _nodes[nodeid].adjacent_nodes()) {
            myfile << pupil_vector[neighbor.id()]._id << "," << student_vector[nodeid - num_pupils]._id << "\n";
        }
    }
    myfile.close();
}

Graph::Graph(char const * filename, DirType dtype): dirtype(dtype)
{
    std::ifstream file(filename);                             // open file
    if (not file) {
        throw std::runtime_error("Cannot open file.");
    }

    Graph::NodeId num = 0;
    std::string line;
    std::getline(file, line);                 // get first line of file
    std::stringstream ss(line);               // convert line to a stringstream
    ss >> num;                                // for which we can use >>
    if (not ss) {
        throw std::runtime_error("Invalid file format.");
    }
    add_nodes(num);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        Graph::NodeId head, tail;
        ss >> tail >> head;
        if (not ss) {
            throw std::runtime_error("Invalid file format.");
        }
        double weight = 1.0;
        ss >> weight;
        if (tail != head) {
            add_edge(tail, head, weight);
        }
        else {
            throw std::runtime_error("Invalid file format: loops not allowed.");
        }
    }
}
