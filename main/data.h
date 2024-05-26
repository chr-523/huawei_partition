#include<vector>
#include<list>
#include<deque> // to be choosen
#include<queue>
#include<tuple>

#include<map>
#include<string>

#define default_edge_weight 1.0
#define default_vertex_weight 1.0
#define default_module_weight 1.0

#define weight_type float
// Afterwards, these can be used int8_t, int16_t, ...
#define module_index_type int
#define edge_offset_type int
#define vertex_index_type int   
// unuseful
#define wire_index_type int

/*
Replace circuit terminology with graph theory terminology.
    circuit/module -> graph
    net -> edge
        pin and assigment -> net
    gate/instance -> vertex

From verilog to output.txt. (limbo)
    'module' -> (module_name + \n +pin_name)
        'pin' -> pin_name + 1/2 (in or out)
        'net' -> net_name
        'assigment' -> point1 point2 (need to add net)
        instance -> type name {PIN}
            {PIN} = pin(net)pin(net)...pin(net)
                or
            {PIN} = pin({group:(net)(net)...})pin(net)...
        
From output to my database. (Function)

    instance (type = gate)
    -> name  -> is_clk + index
    -> pin(net) -> connect net
    
*/

class Graph{
public:
    Graph(){};
    ~Graph(){};
    module_index_type get_module_index() const { return module_index; };
    weight_type get_module_weight() const { return module_weight; };
    std::tuple<bool,vertex_index_type> get_module_data() const { return module_data; }
    std::list<Vertex> get_vertex() const { return vertex; }
    std::list<Graph*> get_subgraph() const { return subgraph; }
    std::list<EdgeList> get_edge_list() const { return edge_list; }
protected:
private:
    module_index_type module_index;
    weight_type module_weight;    // depend on how many and what gate it remain
    std::tuple<bool,vertex_index_type> module_data;   // verse vertex_data type:= <isclk?,index>
    std::list<Vertex> vertex; // vertex_list -> index, is_clk, weight
    std::list<Graph*> subgraph; // subgraph_list -> index, weight
    std::list<EdgeList> edge_list; // -> offset + adj
};

class Vertex{
public: // Initialization
    // Direct initialization
    Vertex():   vertex_index(-1), vertex_weight(default_vertex_weight), vertex_data(false,-1){};
                /*vertex_data(LogicGate().get_is_clk_gate(), LogicGate().get_gate_index())*/
    Vertex(vertex_index_type index, weight_type vertex_weight, bool is_clk){
        this->vertex_index=index;
        this->vertex_weight=vertex_weight;
        this->vertex_data={is_clk,index};
    };
    //
    Vertex(){};
    
    // Initialize with LogicGate (no use)
    Vertex( const vertex_index_type& vertex_index, const LogicGate& vertex_data):
            vertex_index(vertex_index), 
            vertex_weight(vertex_data.get_weight_gate()), 
            vertex_data( vertex_data.get_is_clk_gate() , vertex_data.get_gate_index() ){};
    ~Vertex(){};
public: // Function

    vertex_index_type get_vertex_index() const { return vertex_index; };
    std::tuple<bool,vertex_index_type> get_vertex_data() const { return vertex_data; };
protected:
private:
    vertex_index_type vertex_index;
    weight_type vertex_weight;    // When multiple vertex are synthesized, vertex_weight may rise.(default 1)
    std::tuple<bool,vertex_index_type> vertex_data;   // := <isclk?,index>
    //which net it connevt?
    //input/output graph? Is this need?
};

class EdgeList{
/*
    Edge_list Infromation:
        adjacency_array
        offset_array
    Function：

    eg.
    |-e0-\     /-e1-|		0--\       /--4
    | 0	   \ /    4 |		2---+--1--+---5
    | 2    |1|      |		3--/      
    |_3____/ \____5_|	
        adjacency_array：[0,1,2,3,1,4,5]
        offset_array:[(0,4),(4,3),(6,0)]    
*/
public: // Initialization
    EdgeList(): offset_array(0,0){};
    ~EdgeList(){};
public: // Function
    std::tuple<int,int> get_offset_array() const { return offset_array; }
    vertex_index_type get_adjacency_array() const { return adjacency_array; }

    void add_edge(){};
protected:
private:
    vertex_index_type adjacency_array;
    std::tuple<edge_offset_type,edge_offset_type> offset_array;
};

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

class LogicGate{
/*
    Previously used as a parser, may now be abandoned.
*/
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
    /* Related to the competition question,*/
    bool is_clk;    // Determine if it is the starting/ending point of the timing path.
    vertex_index_type gate_index;
    /*The remaining information is is just information from Verilog*/
    weight_type weight_gate;
    std::string name_gate;
    std::string type_gate;
    std::vector<vertex_index_type> input_gate_list;
    std::vector<vertex_index_type> output_gate_list;
};

class Wire{
/*  Store net information
wire    n1, ... n199, ... n142, ... n222,...
    nand2_1 U519 ( .A(n119), .B(n142), .Y(n222) );
    nand2_1 U524 ( .A(n119), .B(n142), .Y(n225) ); */

    // std::tuple<std::string, wire_index_type, std::tuple<int ,int>> wire;
    // maybe this can be used instead of class
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

