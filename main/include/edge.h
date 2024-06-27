#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>
#include <string>
#include <map>

#define default_name "default"
#define default_edge_weight 1.0

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

enum Edge_type { // INPUT_OUTPUT_NORMAL_Muiti用于记录这玩意儿是多口的边_
    INPUT = 0x1,    // equal 1
    OUTPUT = 0x2,   // equal 2
    NORMAL = 0x0,    // equal 0
    Multi = 0x3   
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
                this -> type = type;    // default = NORMAL
                // this -> offset_array = {0,-1};
                // this - > adjacency_array = NULL
                };
    
    Edge(Name_type edge_name = default_name, int low = -1, int high = -1, Edge_type type = NORMAL){
                this -> edge_name = edge_name;  // default = "default"  
                this -> range.low = low;
                this -> range.high = high;
                this -> type = type;    // default = NORMAL
                // this -> offset_array = {0,-1};
                // this - > adjacency_array = NULL
                };
    
    Edge(const Edge& other): edge_name(other.edge_name), range(other.range), 
        type(other.type), adjacency_array(other.adjacency_array){}

    Edge& operator=(const Edge& other) {
        if (this != &other) { // Prevent self assigmant
            edge_name = other.edge_name;
            range = other.range; // is this no bug?
            type = other.type;
            adjacency_array = other.adjacency_array;
        }
        return *this;
    }

    ~Edge(){};

public: // function
    void split_edge();//to be done, maybe it should be in graph
    //connect signal edge to the instance
    void connect_instance(Instance& instance);
    void connect_instance(Name_type& instance_name);

public: // get_function
    Edge_type get_type() const { return type; };
    Name_type get_name() const { return edge_name; };
    Range get_range() const { return range; }
    // std::array<Edge_offset_type,2> get_offset_array() const { return offset_array; }
    std::vector< Instance_index_type > get_adjacency_array() const { return adjacency_array; }
protected:
private:// data
    
    Edge_type type;    //input, output or normal
    Name_type edge_name; // pin/net's name + '_' + edge_index  // maybe assign -> (list name)
    Range range; // [range.low (and range.high) < 0] means edge is signal
    // std::array<Edge_offset_type,2> offset_array;
    std::vector< Instance_index_type > adjacency_array; //what instance it connect
};

#endif
