#pragma once

#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>
#include <string>
#include <map>
#include <algorithm>

#define default_name "default"
#define default_edge_weight 1.0

using Weight_type = float;
using Name_type = std::string;
using Edge_index_type = std::string;
using Instance_index_type  = std::string;
using Edge_offset_type = int;

#ifndef EDGE_H
#define EDGE_H

struct Range { // low_high
    int low;
    int high;
    Range() {
        low = -1;
        high = -1;
    }
    Range(int l, int h) : low(l), high(h) {}
};
// Represents I/O PIN or internal normal wire within a module
// ----> INPUT, OUTPUT and NORMAL
enum Edge_type {
    INPUT = 0x1,    // equal 1
    OUTPUT = 0x2,   // equal 2
    NORMAL = 0x0    // equal 0
};

// Rrepresent the direction between vertex and edge
// ----> v_to_edge / edge_to_v
enum Direction {
    v_to_edge = 0x1,    // represents the direction from vertex to edge
    edge_to_v = 0x0     // represents the direction from vertex to edge
};

class Instance;

class Edge{
/*
    Edge Infromation:
        type : input output normal
        edge_name
        range
        edge_index
        adjacency_array : what instance it connect
    
    Function：

*/
public: // initialization
    // Edge(name,index,range,[type]) or Edge(name,index,low,high,[type])
    Edge(Name_type edge_name = default_name, Range range = Range(), Edge_type type = NORMAL){
                this -> edge_name = edge_name;  // default = "default"  
                this -> range.low = range.low;
                this -> range.high = range.high;    // default high = low = -1
                this -> edge_weight = default_edge_weight;
                this -> type = type;    // default = NORMAL
                // this -> offset_array = {0,-1};
                // this - > adjacency_array = NULL
                };
    
    Edge(Name_type edge_name = default_name, int low = -1, int high = -1, Edge_type type = NORMAL){
                this -> edge_name = edge_name;  // default = "default"  
                this -> range.low = low;
                this -> range.high = high;
                this -> edge_weight = default_edge_weight;
                this -> type = type;    // default = NORMAL
                // this -> offset_array = {0,-1};
                // this - > adjacency_array = NULL
                };
    
    Edge(const Edge& other): 
        edge_name(other.edge_name), 
        range(other.range), 
        edge_weight(other.edge_weight),
        type(other.type), 
        adjacency_array(other.adjacency_array),
        adj_array_direction(other.adj_array_direction){}

    Edge& operator=(const Edge& other) {
        if (this != &other) { // Prevent self assigmant
            edge_name = other.edge_name;
            range = other.range; // is this no bug?
            edge_weight = other.edge_weight;
            type = other.type;
            adjacency_array = other.adjacency_array;
            adj_array_direction = other.adj_array_direction;
        }
        return *this;
    }

    ~Edge(){};

public: // function
    void split_edge();//to be done, maybe it should be in graph
    //connect signal edge to the instance
    void connect_instance(Instance& instance);
    void connect_instance(Name_type& instance_name);
    // Determine the direction of the edge and store it (for instance)
    void ci_find_direction_ins(Name_type& pin_name);
    // Determine the direction of the edge and store it (for module)
    void ci_find_direction_mod(Name_type& pin_name, Edge_type e_type); 

public: // get_function
    void set_type(const Edge_type& s_type) { this->type=s_type; };
    Edge_type get_type() const { return type; };
    Name_type get_name() const { return edge_name; };
    Weight_type get_weight() const { return edge_weight; };
    Range get_range() const { return range; }
    std::vector< Instance_index_type > get_adjacency_array() const { return adjacency_array; }
    std::vector< Direction > get_adj_array_direction() const { return adj_array_direction; }

    void set_adjacency_array(std::vector< Instance_index_type >& adjacency_array) { 
        this -> adjacency_array = adjacency_array; 
        }

    void set_adj_array_direction(std::vector< Direction >&adj_array_direction) { 
        this -> adj_array_direction = adj_array_direction; 
    }
    // Because of direction, this has been abandoned    
    // void set_adj_byassign(std::vector< Instance_index_type >& v_2) {     
    //     std::copy(v_2.begin(), v_2.end(), std::back_inserter(this -> adjacency_array));
    // }

protected:
private:// data
    Direction find_direction_ins(Name_type& pin_name);
    Direction find_direction_mod(Name_type& pin_name, Edge_type e_type);
    Weight_type edge_weight;
    Edge_type type;    //input, output or normal
    Name_type edge_name; // pin/net's name + '_' + edge_index  // maybe assign -> (list name)
    Range range; // [range.low (and range.high) < 0] means edge is signal
    // std::array<Edge_offset_type,2> offset_array;
    std::vector< Instance_index_type > adjacency_array; //what instance it connect
    std::vector< Direction > adj_array_direction; //what instance it connect
};

#endif
