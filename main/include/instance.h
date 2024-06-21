#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>

#include <string>
#include <map>


#define default_instance_weight 1.0
#define default_name "default"
using weight_type = float;
using Name_type = std::string;
using Edge_index_type = std::string;
using Instance_index_type  = std::string;

class Edge;
#ifndef VERTEX_H
#define VERTEX_H

// enum Ins_IO{
//     INPUT = 0x1,
//     OUTPUT =0x2
// };

class Instance{

public: // Initialization
    // Instance(){}; // equal to the next
    Instance(Instance_index_type index = default_name, weight_type instance_weight = default_instance_weight, bool is_clk = false){
        /*this -> instance_index    = index; // this data is in the instance_data*/ 
        this -> instance_weight   = instance_weight;
        // this -> connect_edge_list = NULL; 
        this -> instance_data     = {is_clk,index};
    };    
    
    Instance(Name_type instance_name, Instance_index_type index = default_name, weight_type instance_weight = default_instance_weight){
        this -> instance_weight   = instance_weight;
        // this -> connect_edge_list = NULL; 
        this -> instance_data = {is_clk(instance_name),index};
    };

    Instance(const Instance& other): instance_weight(other.instance_weight), 
        connect_edge_list(other.connect_edge_list), instance_data(other.instance_data){};

    Instance& operator=(const Instance& other) {
            if (this != &other){ //Prevent self assignment
                instance_weight = other.instance_weight;
                connect_edge_list = other.connect_edge_list; // deep copy vector
                instance_data = other.instance_data; // tuple can deep copy
            }
            return *this;
        }

    ~Instance(){};
public: // function
    void connect_edge(Edge& edge); // connect instance to the edge
    void connect_edge(Name_type& edge_name); // connect instance to the edge

public: // get_Function
    weight_type get_instance_weight() const { return instance_weight; }
    std::vector< Edge_index_type > get_connect_edge() const { return connect_edge_list; };
    std::tuple< bool, Instance_index_type > get_instance_data() const { return instance_data; };
protected:
private:
    bool is_clk(const Name_type& name); //Determine whether it is clk through instance_name
    // Name_type instance_name;  
    weight_type instance_weight;    // When multiple instance are synthesized, instance_weight may rise. (default 1.0)
    std::vector< Edge_index_type  > connect_edge_list; // what edge this instance connect
    std::tuple< bool, Instance_index_type > instance_data;

};

#endif
