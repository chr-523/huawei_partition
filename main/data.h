#include<vector>
#include<list>
#include<deque>
#include<queue>
#include<tuple> // to be choosen

#include<map>
#include<string>

#define default_edge_weight 1.0
#define default_vertex_weight 1.0
#define default_module_weight 1.0

#define weight_type float
// Afterwards, these can be used int8_t, int16_t, ...
#define module_index_type int
#define vertex_index_type int   
#define edge_index_type int
#define edge_offset_type int
#define name_type std::string
// unuseful
#define wire_index_type int

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
        module_name -> graph_name
        
    pin_name + range + 1 or 2 -> Edge(name,index,range,adj,offset)
    net_name + range-> Edge
        name(+1/2) -> edge_name
        range -> range
        index -> index
        none -> adj
        {0,-1} -> offset

    split the edge(to be done)

    instance_type(gate) name [pin(net)pin(net)...] -> vertex(weight,edge_list,data(isclk,index))
        data <-:
            type -> is_clk (now)  (or pin->is_clk?)
            index -> index
        for each pin and net in [~~~] 
            vertex.connectedge -> edge_list.pushback(net)
            edge.connectvertex

    assign(tobedone after split edge)
        -> combine multiple edges into one?

    instance_type(module) -> sub_graph (to be done)
        ->
        -> ? 6.??        
        instance => tu_test_ins, T2, B(n6->[-1:3])
        instance => tu_test_ins, T3, C(GROUP_NETS {(n1)(n2)(n5->[15:0])})
        instance => tu_test_ins, T4, D(n7->[7:0])

    else
    assigment vertex1 vertex2 (instance's pin or wire )(only signal edge)
        ->
        -> ? 5.31


    
*/

struct Range 
{
    int low;
    int high;
    Range() {low = high = -1;}
    Range(int l, int h) : low(l), high(h) {}
};

class Edge{
/*
    Edge Infromation:
        edge_name
        range
        edge_index
        adjacency_array
        offset_array
    
    Function：


    eg. to be done
    |-e0-\     /-e1-|		 input--0--\       /--4--output
PI<-+-0	   \ /    4-+->PO	        2---+--1--+---5
    | 2    |1|      |		        3--/      
    |_3____/ \____5_|	    
name:e_0    index: 0                    name:e_1    index: 1
        adjacency_array：[0,1,2,3]      adjacency_array：[1,4,5]
        offset_array:(0,4)              offset_array:(0,3) 
name:input_PI    index: 3               name:output_PO    index: 4
        adjacency_array：[0]            adjacency_array：[4]
        offset_array:(0,1)              offset_array:(0,1) 
*/
public: // initialization
    Edge():edge_index(-1), /*adjacency_array(NULL), */offset_array({0,-1}){};
    //signal edge initialization
    Edge(name_type edge_name, edge_index_type edge_index):
            edge_name(edge_name),
            edge_index(edge_index), range(), offset_array({0,-1}){};
    //multi-edge initialization
    Edge(name_type edge_name, edge_index_type edge_index, Range range):
            edge_name(edge_name),
            edge_index(edge_index),offset_array({0,-1}){
                this -> range.high = range.high;
                this -> range.low = range.low;
                };
    ~Edge(){};
public: // function
    void split_edge(){};//to be done

    void connect_vertex(Vertex vertex){};//only connect signal edge
    void connect_vertex_old(Vertex vertex){};

public: // get_function
    std::string get_name() const { return edge_name; };
    edge_index_type get_edge_index() const { return edge_index; } 
    std::array<edge_offset_type,2> get_offset_array() const { return offset_array; }
    std::vector<vertex_index_type> get_adjacency_array() const { return adjacency_array; }
protected:
private:// data
    name_type edge_name; // input_...or output_...
    Range range; // [range.low (and range.high) < 0] means edge is signal
    edge_index_type edge_index;
    std::vector<vertex_index_type> adjacency_array;//what vertex it connect
    std::array<edge_offset_type,2> offset_array;
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
    v_0 index: 0            v_1 index: 1
        data: <true,0>          data: <false,1>
    
*/
public: // Initialization

    // Direct initialization
    Vertex(): vertex_weight(default_vertex_weight), vertex_data(false,-1){};

    Vertex(vertex_index_type index = -1, weight_type vertex_weight = default_vertex_weight, bool is_clk = false){
        /*this -> vertex_index    = index;*/
        this -> vertex_weight   = vertex_weight;
        this -> vertex_data     = {is_clk,index};
    };    
    
    Vertex(name_type vertex_name, vertex_index_type index, weight_type vertex_weight = default_vertex_weight){
        this -> vertex_weight   = vertex_weight;
        this -> vertex_data = {is_clk(vertex_name),index};
    };
    
    ~Vertex(){};
public: // function
    void connect_edge(Edge edge){};;

public: // get_Function
    weight_type get_vertex_weight() const { return vertex_weight; }
    std::vector<edge_index_type> get_connect_edge() const { return connect_edgelist; };
    std::tuple<bool,vertex_index_type> get_vertex_data() const { return vertex_data; };
protected:
private:
    bool is_clk(name_type name){};
    // name_type vertex_name;  
    weight_type vertex_weight;    // When multiple vertex are synthesized, vertex_weight may rise.(default 1)
    std::vector<edge_index_type> connect_edgelist;
    std::tuple<bool,vertex_index_type> vertex_data;   // := <isclk?,index>

};

class Graph{
/*
big graph
    |-e0-\     /-e1-|
    | 1	   \ /   (1)|
    |(0)  |0,3|  (2)|
    |_2____/ \____4_|
small = (0)(1)(2)
    |-e0-\     /-e1-|		0--\       /--4
    | 0	   \ /    4 |		2---+--1--+---5
    | 2    |1|      |		3--/      
    |_3____/ \____5_|
*/
public:
    Graph(){};
    ~Graph(){};
public:
public:
    name_type get_module_name() const { return module_name; };
    module_index_type get_module_index() const { return module_index; };
    weight_type get_module_weight() const { return module_weight; };
    // std::list<name_type> get_multi_edge_list() const { return multi_edge_list; };
    std::tuple<bool,vertex_index_type> get_module_data() const { return module_data; }
    std::list<Vertex> get_vertex() const { return vertex; }
    std::list<Graph*> get_subgraph() const { return subgraph; }
    std::list<Edge> get_edge_list() const { return edge_list; }
protected:
private:
    name_type module_name;
    module_index_type module_index;
    weight_type module_weight;    // depend on how many and what gate it remain
    // std::list<name_type> multi_edge_list; //std::unordered_set?
    std::tuple<bool,vertex_index_type> module_data;   // verse vertex_data type:= <isclk?,index>
    std::list<Vertex> vertex; // vertex_list -> index, is_clk, weight
    std::list<Graph*> subgraph; // subgraph_list -> index, weight
    std::list<Edge> edge_list; // -> offset + adj
};

/* CLASS VERTEX_OLD
class Vertex_old{
public:
    Vertex_old():vertex_index(0),vertex_data( LogicGate() ){};
    Vertex_old(const vertex_index_type& vertex_index, const LogicGate& vertex_data):
           vertex_index(vertex_index),          vertex_data(vertex_data){};
    ~Vertex_old(){};
public:
    vertex_index_type get_vertex_index() const { return vertex_index; };
    LogicGate get_vertex_data() const { return vertex_data; };
protected:
private:
    vertex_index_type vertex_index;
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
    LogicGate(  const bool& is_clk,                         const vertex_index_type& gate_index,
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
    vertex_index_type get_gate_index() const { return gate_index; };
    float get_weight_gate() const { return weight_gate; };
    std::string get_name_gate() const { return name_gate; };
    std::string get_type_gate() const { return type_gate; };
    std::vector<vertex_index_type> get_input_gate_list() const { return input_gate_list; };
    std::vector<vertex_index_type> get_output_gate_list() const { return output_gate_list; };
protected:
private:
    // Related to the competition question,
    bool is_clk;    // Determine if it is the starting/ending point of the timing path.
    vertex_index_type gate_index;
    //The remaining information is is just information from Verilog
    weight_type weight_gate;
    std::string name_gate;
    std::string type_gate;
    std::vector<vertex_index_type> input_gate_list;
    std::vector<vertex_index_type> output_gate_list;
};
*/

/* CLASS WIRE
class Wire{
//   Store net information
// wire    n1, ... n199, ... n142, ... n222,...
//     nand2_1 U519 ( .A(n119), .B(n142), .Y(n222) );
//     nand2_1 U524 ( .A(n119), .B(n142), .Y(n225) ); 

//     // std::tuple<std::string, wire_index_type, std::tuple<int ,int>> wire;
//     // maybe this can be used instead of class (or struct)
public:
    Wire(): name("n142"), wire_index(142), pin2pin{519,524}{};
    Wire(const std::string& name, const wire_index_type& wire_index, const std::vector<vertex_index_type>& pin2pin):
        name(name), wire_index(wire_index),pin2pin(pin2pin){};
    ~Wire(){};
public:
    std::string get_name() const { return name; };
    wire_index_type get_wire_index() const { return wire_index; };
    std::vector<vertex_index_type> get_pin2pin() const { return pin2pin; };
protected:
private:
    std::string name;
    wire_index_type wire_index;
    std::vector<vertex_index_type> pin2pin;
};
*/
