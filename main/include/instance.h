#include "i_e_all.h"


#define default_instance_weight 1.0
#define default_name "default"
using weight_type = int;
using Name_type = std::string;
using Edge_index_type = std::string;
using Instance_index_type  = std::string;

class Edge;
#ifndef VERTEX_H
#define VERTEX_H

class Instance{
/*    
    Instance Infromation:
        <is_clk,index>
    Function：

    |-e0-\     /-e1-|		0--\       /--4
    | 0	   \ /    4 |		2---+--1--+---5
    | 2    |1|      |		3--/      
    |_3____/ \____5_|
    v_0 index: v_0          v_1 index: v_1     ...  v_5 index: v_5
        data: <true,v_0>        data: <false,v_1>       data: <true,v_5>
        e_l: [0]                e_l: [1,0]              e_l: [1]
*/
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

    Instance(const Instance& other): 
        instance_weight(other.instance_weight), 
        connect_edge_list(other.connect_edge_list), 
        instance_data(other.instance_data),
        connect_edge_direction(other.connect_edge_direction){};

    Instance& operator=(const Instance& other) {
            if (this != &other){ //Prevent self assignment
                instance_weight = other.instance_weight;
                connect_edge_list = other.connect_edge_list; // deep copy vector
                instance_data = other.instance_data; // tuple can deep copy
                connect_edge_direction = other.connect_edge_direction;
            }
            return *this;
        }

    ~Instance(){};
public: // function
    void connect_edge(Edge& edge); // connect instance to the edge
    void connect_edge(const Name_type& edge_name); // connect instance to the edge
    void add_edge_direction(Direction& direc);
public: // get_Function
    weight_type get_instance_weight() const { return instance_weight; }
    std::vector< Edge_index_type  > get_connect_edge() const { return connect_edge_list; };
    std::tuple< bool, Instance_index_type > get_instance_data() const { return instance_data; };
    std::vector< Direction > get_connect_edge_direction() const {
        return connect_edge_direction;
    }; // what edge this instance connect
protected:
private:
    bool is_clk(const Name_type& name); //Determine whether it is clk through instance_name
    // Name_type instance_name;  
    weight_type instance_weight;    // When multiple instance are synthesized, instance_weight may rise. (default 1.0)
    std::vector< Edge_index_type  > connect_edge_list; // what edge this instance connect
    std::vector< Direction > connect_edge_direction; // what edge this instance connect
    std::tuple< bool, Instance_index_type > instance_data;

};

#endif
