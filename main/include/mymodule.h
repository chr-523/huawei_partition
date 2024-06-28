#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>
#include <string>
#include <map>
// #include <copy>

#include "instance.h"
#include "edge.h"
#include "../paser/VerilogDataBase.h"

#define default_name "default"
#define default_module_weight 1.0
using weight_type = int;
using Name_type = std::string;
using Module_index_type  = std::string;
using Instance_index_type  = std::string;
using Edge_index_type = std::string;
class Module;
using Internal_Instance_type = Instance;

using Sub_Module_type = std::tuple< Module_index_type, Module*>;

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
          module_weight(other.module_weight), internal_instance(other.internal_instance), submodule(other.submodule),
          internal_edge_list(other.internal_edge_list),
          assign_edge_list(other.assign_edge_list),
          submodule_pin_edge(other.submodule_pin_edge),
          IO_map(other.IO_map),E_map(other.E_map),
          Ins_map(other.Ins_map),Submodule_map(other.Submodule_map){};
    // operator = 
    Module& operator=(const Module& other){
        if (this != &other) { // Prevent self assigment
            module_name = other.module_name;
            module_weight = other.module_weight;
            internal_instance = other.internal_instance;
            submodule = other.submodule;
            internal_edge_list = other.internal_edge_list;
            assign_edge_list = other.assign_edge_list;
            submodule_pin_edge = other.submodule_pin_edge;
            IO_map = other.IO_map;
            E_map = other.E_map;
            Ins_map = other.Ins_map;
            Submodule_map = other.Submodule_map;
        }
        return *this;
    }

    ~Module(){};
public: //function
    void add_edge(Name_type& name, int& low, int& high, Edge_type& type); // add net/pin
    void add_edge(Name_type& name, Range& range,        Edge_type& type); // add net/pin
    void add_instance(Name_type& type_name, Instance_index_type& name); // add instance without connection 
    // connect instance and edge

    friend void connect_ins_edge(
        Module& gra, Instance_index_type& name, 
        std::queue< Name_type >& edge_name_queue, 
        std::queue< Range >& range_queue);
    friend void connect_ins_edge(
        Module& gra,
        Name_type& pin_name,
        Instance_index_type& instance_name, 
        Name_type& edge_name, 
        Range& range);

    friend int connect_mod_edge(
        Module& gra,//返回值用来调整 a{a}+ wire[3:0]a的情况
        Instance_index_type& mode_name, 
        Name_type& edge_name, 
        Range& range,
        Name_type& pin_name,
        size_t& subfix_counter,
        Direction& e_m_type);
    friend int connect_mod_edge(
        Module& gra,//返回值用来调整 a{a}+ wire[3:0]a的情况
        Instance_index_type& mode_name, 
        Name_type& edge_name, 
        Range& range,
        Direction& e_m_type);

    void add_back_ins_edge(const Name_type& e_){
        // Internal_Instance_type = Instance;
        this -> internal_instance.back().connect_edge(e_);
        // connect_edge(e_);
    }

    // should find G_1 before use it add module without connection 
    void add_module( Module_index_type module_index, Module* G_1); 
    void add_submodule_pin_edge(const Name_type& pin_name){/*或者是const Name_type& edge_name*/
        this -> submodule_pin_edge.back().push_back(pin_name);}

public: //function about the data
    void clear(){
        this -> module_name = default_name;
        this -> module_weight = default_module_weight;        
        this -> internal_instance.clear();       
        this -> submodule.clear();        
        this -> internal_edge_list.clear();
        this -> assign_edge_list.clear();
        this -> submodule_pin_edge.clear();
        this -> IO_map.clear();
        this -> E_map.clear();
        this -> Ins_map.clear();
        this -> Submodule_map.clear();
    }
    void set_module_name(const Name_type& new_module_name){ this -> module_name = new_module_name; }
    void assign_two_edge(Name_type& e_1, Name_type& e_2);
    Name_type get_module_name() const { return module_name; };
    weight_type get_module_weight() const { return module_weight; };
    std::vector< Internal_Instance_type > get_instance_list() const { return internal_instance; };
    std::vector< Sub_Module_type > get_submodule_list() const { return submodule; };
    std::vector< std::vector<Edge_index_type>> 
        get_submodule_pin_edge_list() const { return submodule_pin_edge; };
    std::vector< Edge > get_internal_edge_list() const { return internal_edge_list; };
    std::vector<std::pair<Edge_index_type,Edge_index_type>> assign_edge_list;
    std::unordered_map<Edge_index_type,std::pair<Edge_type,Range>> IO_map;
    std::unordered_map< Instance_index_type, size_t > Ins_map; 
    std::unordered_map< Module_index_type, size_t > Submodule_map; 
    std::unordered_map< Edge_index_type, size_t > E_map;
    
protected:
private: 

    // module_name (It's the module name, not the initialized name)
    Name_type module_name;
    // same as instance's weight (maybe depend on how many and what gate it remain?)
    weight_type module_weight;

    std::vector< Internal_Instance_type > internal_instance;
    // Sub_Module_type = std::tuple<Module_index_type, Module *>
    std::vector< Sub_Module_type > submodule; 
    std::vector<std::vector< Edge_index_type>> submodule_pin_edge;
    std::vector< Edge > internal_edge_list; 
};


struct Graph_data{
    Module module;
    std::unordered_map<Name_type, Module*> submodule_map;
    Graph_data(
        Module gra, std::unordered_map<Name_type, Module*> s_m):
            module(gra), submodule_map(s_m) {}
};

Direction fins_ins_pin_direction(Name_type &pin_name);
#endif
