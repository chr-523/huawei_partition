#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>
#include <string>
#include <map>
// #include <copy>

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
class Module;
using Sub_Module_type = 
    std::tuple<
        Module_index_type, 
        std::vector<std::tuple<Edge_index_type,Edge_index_type> >, 
        Module*>;

#ifndef GRAPH_H
#define GRAPH_H

class Module{
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
    Module(Name_type module_name = default_name, weight_type module_weight = default_module_weight):
          module_name(module_name), module_weight(module_weight){};

    Module(const Module& other): module_name(other.module_name),
          module_weight(other.module_weight), vertex(other.vertex), submodule(other.submodule),
          internal_edge_list(other.internal_edge_list){};
    // operator = 
    Module& operator=(const Module& other){
        if (this != &other) { // Prevent self assigment
            module_name = other.module_name;
            module_weight = other.module_weight;
            vertex = other.vertex;
            submodule = other.submodule;
            internal_edge_list = other.internal_edge_list;
        }
        return *this;
    }

    ~Module(){};
public: //function
    void add_edge(Name_type& name, int& low, int& high, Edge_type& type); // add net/pin
    void add_edge(Name_type& name, Range& range,        Edge_type& type); // add net/pin
    void add_instance(Name_type& type_name, Vertex_index_type& name); // add instance without connection 
    // connect instance and edge
    friend void connect_ins_edge(
        Module& gra, Vertex_index_type& name, 
        std::queue< Name_type >& edge_name_queue, 
        std::queue< Range >& range_queue);
    // connect module and edge...to be done
    friend void connect_mod_edge(
        Module& gra, Module_index_type& name, 
        std::queue< Name_type >& edge_name_queue,        
        std::queue< Name_type >& pin_name_queue, 
        std::queue< Range >& range_queue); 

    // add module without initialization
    void add_module();
    // should find G_1 before use it add module without connection 
    void add_module( 
        std::vector< std::tuple<Edge_index_type, Edge_index_type >> connect_edge_list, 
        Module_index_type module_index, Module* G_1); 
    // used in connecting edge except group net
   

        // same as vertex
    void connect_edge(Edge& edge){ /**/ }; // connect vertex to the edge
    void connect_edge(Name_type& edge_name){ /**/ }; // connect vertex to the edge
public: //function about the data
    void clear(){
        this -> module_name = default_name;
        this -> module_weight = default_module_weight;        
        this -> vertex.clear();       
        this -> submodule.clear();        
        this -> internal_edge_list.clear();
    }
    void set_module_name(const Name_type& new_module_name){ this -> module_name = new_module_name; }
    Name_type get_module_name() const { return module_name; };
    weight_type get_module_weight() const { return module_weight; };
    std::vector< Vertex > get_vertex_list() const { return vertex; };
    std::vector< Sub_Module_type > get_submodule_list() const { return submodule; };
    std::vector< Edge > get_internal_edge_list() const { return internal_edge_list; }protected:
private: 
    // module_name (It's the module name, not the initialized name)
    Name_type module_name;
    // same as vertex's weight (maybe depend on how many and what gate it remain?)
    weight_type module_weight;
    // Vertex => connect_edge_list, data, weight (data = <is_clk, index>)
    // _c_e_l -> the vector of index
    // _____index -> which edge the vertex connect
    // _data -> <is_clk, index> index's eg. U1
    // _weight
    std::vector< Vertex > vertex;
    // Sub_Module_type = <index, connect_edge_list, module*>
    // _index -> the name of module eg. M1
    // _c_e_l -> the vector of <index_1,index_2>
    // _____index_1 -> which edge the module connect
    // _____index_2 -> which pin the index_1 edge connect
    // _module* -> name, weight, internal_vertex, internal_edge
    std::vector< Sub_Module_type > submodule; 
    // what edge this module contain (edge_list -> edge's adj)
    std::vector< Edge > internal_edge_list; 
};

#endif
