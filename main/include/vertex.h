#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>

#include <string>
#include <map>


#define default_vertex_weight 1.0
#define default_name "default"
using weight_type = float;
using Name_type = std::string;
using Edge_index_type = std::string;
using Vertex_index_type  = std::string;

class Edge;
#ifndef VERTEX_H
#define VERTEX_H


class Vertex{
/*    
    Vertex Infromation:
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
    // Vertex(){}; // equal to the next
    Vertex(Vertex_index_type index = default_name, weight_type vertex_weight = default_vertex_weight, bool is_clk = false){
        /*this -> vertex_index    = index; // this data is in the vertex_data*/ 
        this -> vertex_weight   = vertex_weight;
        // this -> connect_edge_list = NULL; 
        this -> vertex_data     = {is_clk,index};
    };    
    
    Vertex(Name_type vertex_name, Vertex_index_type index = default_name, weight_type vertex_weight = default_vertex_weight){
        this -> vertex_weight   = vertex_weight;
        // this -> connect_edge_list = NULL; 
        this -> vertex_data = {is_clk(vertex_name),index};
    };

    Vertex(const Vertex& other): vertex_weight(other.vertex_weight), 
        connect_edge_list(other.connect_edge_list), vertex_data(other.vertex_data){};

    Vertex& operator=(const Vertex& other) {
            if (this != &other){ //Prevent self assignment
                vertex_weight = other.vertex_weight;
                connect_edge_list = other.connect_edge_list; // deep copy vector
                vertex_data = other.vertex_data; // tuple can deep copy
            }
            return *this;
        }

    ~Vertex(){};
public: // function
    void connect_edge(Edge& edge); // connect vertex to the edge
    void connect_edge(Name_type& edge_name); // connect vertex to the edge

public: // get_Function
    weight_type get_vertex_weight() const { return vertex_weight; }
    std::vector< Edge_index_type > get_connect_edge() const { return connect_edge_list; };
    std::tuple< bool, Vertex_index_type > get_vertex_data() const { return vertex_data; };
protected:
private:
    bool is_clk(const Name_type& name); //Determine whether it is clk through vertex_name
    // Name_type vertex_name;  
    weight_type vertex_weight;    // When multiple vertex are synthesized, vertex_weight may rise. (default 1.0)
    std::vector< Edge_index_type > connect_edge_list; // what edge this vertex connect
    std::tuple< bool, Vertex_index_type > vertex_data;   //  = <isclk?,index>
};

#endif
