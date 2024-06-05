#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <tuple> // to be choosen
#include <unordered_set>

#include <string>
#include <map>

#define default_edge_weight 1.0
#define default_vertex_weight 1.0
#define default_module_weight 1.0

#define weight_type float
// Afterwards, these can be used int8_t, int16_t, ...
using Module_index_type  = std::string;
using Vertex_index_type  = std::string;
using Edge_index_type = std::string;
#define default_name "default"
#define Edge_offset_type int
// unuseful
#define Wire_index_type int

using Name_type = std::string;

/* name explain
Replace circuit terminology with graph theory terminology.
    circuit/module -> graph
    net -> edge
        pin and assigment -> net
    gate/instance -> vertex
*/

/* From verilog         to output(.txt). (limbo)
    'module' -> (module_name + \n +pin_name)
        'pin' -> pin_name + 1/2 (in or out)
        'net' -> net_name
        'assigment' -> vertex1 vertex2 (need to add net)
        'instance' -> type name {PIN}
            {PIN} = pin(net)pin(net)...pin(net)
                or
            {PIN} = pin({group:(net)(net)...})pin(net)...
                where net can be:
                net = net -> signal edge
                    = net -> multi-edge -> n signal edges
                    = net[a] -> signal edge
                    = net[a,b] -> multi-edge -> n signal edges
*/

/* From output(.txt)    to my database. 

    module module_name pin_name
        -->> Graph module_name
        
    pin_name + range + 1 or 2
            -->> for each pin
        {
        module_name.add_edge(pin_name,range.low,range.high,type);//   pin name [123] 1/2  
            -> Edge(name,index,range,adj,offset,type)
        // module_name.split_multi_edge(); (to be done)
        }     

    net_name + rang e-> Edge
            -->> for each pin
        {
            module_name.add_edge(net_name,range.low,range.high);//   net name [123]
                -> Edge(name,index,range,adj,offset)
            // module_name.split_multi_edge(); (to be done)
        }     

    assign vertex1 vertex2 
    (tobedone after split edge)(instance's pin or wire ) (only signal edge)
        -> combine multiple edges into one?
        -> ? 6.7

    instance_type(gate) name [pin(net)pin(net)...] -> vertex
            -->> for each instance(if type == gate)
        {
            Vertex

            module_name.add_vertex(vv_before)

        }

        data <-:
            type -> is_clk (now)  (or pin->is_clk?)
            index -> index
        for each pin and net in [~~~] 
            vertex.connectedge -> edge_list.pushback(net)
            edge.connectvertex

    instance_type(module) -> sub_graph (to be done)
        -> connect the edge should be reconfiguration
        -> ?? 6.7 
        this three situations should be ...
        instance => tu_test_ins, T2, B(n6->[-1:3])
        instance => tu_test_ins, T3, C(GROUP_NETS {(n1)(n2)(n5->[15:0])})
        instance => tu_test_ins, T4, D(n7->[7:0])

*/

struct Range { // low_high
    int low;
    int high;
    Range() {low = high = -1;}
    Range(int l, int h) : low(l), high(h) {}
};

enum Edge_type { // INPUT_OUTPUT_NORMAL
    INPUT = 0x1,    // equal 1
    OUTPUT = 0x2,   // equal 2
    NORMAL = 0x0    // equal 0
};

class Edge; // advance declaration
class Vertex; // advance declaration

class Edge{
/*
    Edge Infromation:
        type : input output normal
        edge_name
        range
        edge_index
        adjacency_array : what vertex it connect
    
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
    
    Edge(Name_type edge_name = default_name, int low, int high, Edge_type type = NORMAL){
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
    void split_edge(){};//to be done, maybe it should be in graph
    //connect signal edge to the vertex
    void connect_vertex(Vertex& vertex){};
    void connect_vertex(Name_type& vertex_name){};

public: // get_function
    Edge_type get_type() const { return type; };
    Name_type get_name() const { return edge_name; };
    // std::array<Edge_offset_type,2> get_offset_array() const { return offset_array; }
    std::vector< Vertex_index_type > get_adjacency_array() const { return adjacency_array; }
protected:
private:// data
    Edge_type type;    //input, output or normal
    Name_type edge_name; // pin/net's name + '_' + edge_index  // maybe assign -> (list name)
    Range range; // [range.low (and range.high) < 0] means edge is signal
    // std::array<Edge_offset_type,2> offset_array;
    std::vector< Vertex_index_type > adjacency_array;//what vertex it connect
    
};

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
    void connect_edge(Edge& edge){}; // connect vertex to the edge
    void connect_edge(Name_type& edge_name){}; // connect vertex to the edge

public: // get_Function
    weight_type get_vertex_weight() const { return vertex_weight; }
    std::vector< Edge_index_type > get_connect_edge() const { return connect_edge_list; };
    std::tuple< bool, Vertex_index_type > get_vertex_data() const { return vertex_data; };
protected:
private:
    bool is_clk(const Name_type& name){}; //Determine whether it is clk through vertex_name
    // Name_type vertex_name;  
    weight_type vertex_weight;    // When multiple vertex are synthesized, vertex_weight may rise. (default 1.0)
    std::vector< Edge_index_type > connect_edge_list; // what edge this vertex connect
    std::tuple< bool, Vertex_index_type > vertex_data;   //  = <isclk?,index>
};

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
    void add_edge(Name_type& name, int& low, int& high, Edge_type& type){}; // add net/pin
    void add_edge(Name_type& name, Range& range,        Edge_type& type){}; // add net/pin
    void add_instance(Name_type& type_name, Vertex_index_type& name){}; // add instance no pin
    void connect_ins_edge(Vertex_index_type& name, std::queue< Name_type >& edge_name_queue, std::queue< Range >& range_queue); // connect instance and edge
    void add_module(){}; // add module...to be done
    void connect_mod_edge(Vertex_index_type& name, std::queue< Name_type >& edge_name_queue, std::queue< Range >& range_queue); // connect module and edge...to be done
public://getfunction
    Name_type get_module_name() const { return module_name; };
    weight_type get_module_weight() const { return module_weight; };
    // std::list<Name_type> get_multi_edge_list() const { return multi_edge_list; };
    // std::tuple<bool,Vertex_index_type> get_module_data() const { return module_data; }
    std::vector< Vertex > get_vertex_list() const { return vertex; }
    std::vector< Graph* > get_subgraph_list() const { return subgraph; }
    std::vector< Edge > get_internal_edge_list() const { return internal_edge_list; }
    std::vector< Edge_index_type > get_connect_edge_list() const { return connect_edge_list; }
protected:
private:
    bool is_instance_type(Name_type& instance_name);
    Name_type module_name; // module_name 
    weight_type module_weight;    // maybe depend on how many and what gate it remain?
    // std::list<Name_type> multi_edge_list;
    // std::tuple<bool,Vertex_index_type> module_data;   // verse vertex_data type:= <isclk?,index>
    std::vector< Vertex > vertex; // vertex_list -> data, weight (within data = <is_clk,index>)
    std::vector< Graph* > subgraph; // subgraph_list -> index, weight
    std::vector< Edge > internal_edge_list; // what edge this module contain (edge_list -> edge's adj)
    std::vector< Edge_index_type > connect_edge_list; // what edge this module connect

};



//else is old



/* CLASS VERTEX_OLD
class Vertex_old{
public:
    Vertex_old():vertex_index(0),vertex_data( LogicGate() ){};
    Vertex_old(const Vertex_index_type& vertex_index, const LogicGate& vertex_data):
           vertex_index(vertex_index),          vertex_data(vertex_data){};
    ~Vertex_old(){};
public:
    Vertex_index_type get_vertex_index() const { return vertex_index; };
    LogicGate get_vertex_data() const { return vertex_data; };
protected:
private:
    Vertex_index_type vertex_index;
    LogicGate vertex_data;
};
*/

/* CLASS LOGICGATE
class LogicGate{

    Previously used as a parser, may now be abandoned.

public:
    LogicGate():is_clk(false), gate_index(-1), 
                weight_gate(1.0), name_gate("Unknown"), type_gate("Unknown"), 
                input_gate_list{-1}, output_gate_list{-1}{};
    LogicGate(  const bool& is_clk,                         const Vertex_index_type& gate_index,
                //The initialization of weight_gate has not been completed yet.
                const std::string& type_gate,               const std::string& name_gate,
                const std::vector<int> input_gate_list,     const std::vector<int> output_gate_list):  
                is_clk(is_clk),                             gate_index(gate_index),
                name_gate(name_gate),                       type_gate(type_gate),
                input_gate_list(input_gate_list),           output_gate_list(output_gate_list){};
    // Paser?
    //LogicGate(const std::string vlg_dot_v="sky130_fd_sc_hd__nand2_1 U569 ( .A(ff1_1[4]), .B(n395), .Y(n397) );"){
    // };
    ~LogicGate(){};
public:
    bool get_is_clk_gate() const { return is_clk; };
    Vertex_index_type get_gate_index() const { return gate_index; };
    float get_weight_gate() const { return weight_gate; };
    std::string get_name_gate() const { return name_gate; };
    std::string get_type_gate() const { return type_gate; };
    std::vector<Vertex_index_type> get_input_gate_list() const { return input_gate_list; };
    std::vector<Vertex_index_type> get_output_gate_list() const { return output_gate_list; };
protected:
private:
    // Related to the competition question,
    bool is_clk;    // Determine if it is the starting/ending point of the timing path.
    Vertex_index_type gate_index;
    //The remaining information is is just information from Verilog
    weight_type weight_gate;
    std::string name_gate;
    std::string type_gate;
    std::vector<Vertex_index_type> input_gate_list;
    std::vector<Vertex_index_type> output_gate_list;
};
*/

/* CLASS WIRE
class Wire{
//   Store net information
// wire    n1, ... n199, ... n142, ... n222,...
//     nand2_1 U519 ( .A(n119), .B(n142), .Y(n222) );
//     nand2_1 U524 ( .A(n119), .B(n142), .Y(n225) ); 

//     // std::tuple<std::string, Wire_index_type, std::tuple<int ,int>> wire;
//     // maybe this can be used instead of class (or struct)
public:
    Wire(): name("n142"), wire_index(142), pin2pin{519,524}{};
    Wire(const std::string& name, const Wire_index_type& wire_index, const std::vector<Vertex_index_type>& pin2pin):
        name(name), wire_index(wire_index),pin2pin(pin2pin){};
    ~Wire(){};
public:
    std::string get_name() const { return name; };
    Wire_index_type get_wire_index() const { return wire_index; };
    std::vector<Vertex_index_type> get_pin2pin() const { return pin2pin; };
protected:
private:
    std::string name;
    Wire_index_type wire_index;
    std::vector<Vertex_index_type> pin2pin;
};
*/
