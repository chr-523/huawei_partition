#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>
#include <string>
#include <map>
#include "vertex.h"
#include "edge.h"
#include "../paser/VerilogDataBase.h"

#define default_name "default"
#define default_module_weight 1.0
using weight_type = float;
using Name_type = std::string;
using Module_index_type  = std::string;
using Vertex_index_type  = std::string;
using Edge_index_type = std::string;

#ifndef GRAPH_H
#define GRAPH_H

class Graph{
/* eg.
// big graph
//
//      |-e0--\   /--e1-|
//      | 1	   \ /   (1)|
//      |(0)  |0,3|  (2)|
//input-+-2    / \    4-+-output
//      |_____/   \___6_|
//
// sub_graph = (0)(1)(2)
//
//      |-e0--\   /--e1-|
//      | 0	   \ /    4-+-output
//input-+-2    |1|      |  
//      |_3____/ \____5_|
*/
/*

*/
public:
    Graph(Name_type module_name = default_name, weight_type module_weight = default_module_weight):
          module_name(module_name), module_weight(module_weight){};

    Graph(const Graph& other): module_name(other.module_name),
          module_weight(other.module_weight), vertex(other.vertex), subgraph(other.subgraph),
          internal_edge_list(other.internal_edge_list){};
    // 赋值运算符
    Graph& operator=(const Graph& other){
        if (this != &other) { // Prevent self assigment
            module_name = other.module_name;
            module_weight = other.module_weight;
            vertex = other.vertex;
            subgraph = other.subgraph;
            internal_edge_list = other.internal_edge_list;
        }
        return *this;
    }

    ~Graph(){};
public: //function
    void add_edge(Name_type& name, int& low, int& high, Edge_type& type); // add net/pin
    void add_edge(Name_type& name, Range& range,        Edge_type& type); // add net/pin
    void add_instance(Name_type& type_name, Vertex_index_type& name); // add instance without connection
    void connect_ins_edge(Vertex_index_type& name, std::queue< Name_type >& edge_name_queue, std::queue< Range >& range_queue); // connect instance and edge
    // add module without connection
    void add_module(Module_index_type& module_name);
    // should find G_1 before use it add module without connection 
    void add_module(Graph* G_1); 
    // used in connecting edge except group net
    void connect_mod_edge(Vertex_index_type& name, std::queue< Name_type >& edge_name_queue, std::queue< Range >& range_queue); // connect module and edge...to be done
    // used in group net connection
    void connect_mod_edge(Module_index_type& module_name, std::vector< VerilogParser::GeneralName >& group_edge);
    
        // same as vertex
    void connect_edge(Edge& edge){ /**/ }; // connect vertex to the edge
    void connect_edge(Name_type& edge_name){ /**/ }; // connect vertex to the edge
public: //get_function
    Name_type get_module_name() const { return module_name; };  // same as vertex's index in data<1>
    weight_type get_module_weight() const { return module_weight; }; // same as vertex's weight
    std::vector< Vertex > get_vertex_list() const { return vertex; }; // internal vertex
    std::vector< Graph* > get_subgraph_list() const { return subgraph; }; // internal sub_graph
    std::vector< Edge > get_internal_edge_list() const { return internal_edge_list; }
    std::vector< Edge_index_type > get_connect_edge_list() const { return connect_edge_list; } // same as vertex's edgelist
protected:
private:
    bool is_instance_type(Name_type& instance_name);
    Name_type module_name; // module_name (same as vertex's index in data<1>)
    weight_type module_weight;    // (same as vertex's weight) maybe depend on how many and what gate it remain?
    std::vector< Vertex > vertex; // internal vertex -> _list -> data, weight (within data = <is_clk,index>)
    std::vector< Graph* > subgraph; // internal subgraph -> _list -> index, weight
    std::vector< Edge > internal_edge_list; // what edge this module contain (edge_list -> edge's adj)
    std::vector< Edge_index_type > connect_edge_list; // what edge this module connect (same as vertex's edgelist)
};

#endif
