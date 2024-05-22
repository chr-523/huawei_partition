#include<vector>
#include<list>
#include<deque> // 后面看看用哪个
#include<queue>
#include<tuple>

#include<map>
#include<string>

#define default_edge_weight 1.0
#define default_vertex_weight 1.0
// #define weight_type float
#define gate_index_type int
#define wire_index_type int

/*

*/

class CircuitGraph{/* example
             0, 1,   2, 3, 4, 5,    
Vertex:		[0][0,1][0][0][1][1]
    一个[0]或者[0,1]表示一个对应点,连接的所有边
Edge:	E+A	0↓          1↓
		E:	[0,↓      4][4,↓   3] (else+)[6,0]  //offset + connect_number
		A:	[0, 1, 2, 3, 1, 4, 5]
|-e0-\     /-e1-|		0--\       /--4
| 0	   \ /    4 |		2---+--1--+---5
| 2    |1|      |		3--/      
|_3____/ \____5_|	
*/
public:
    CircuitGraph(){};
    ~CircuitGraph(){};

    std::list<Vertex> get_vertex() const { return vertex; };
    std::list<EdgeList> get_edge_list() const { return edge_list; };
protected:
private:

    std::list<Vertex> vertex; // vertex:index, is_clk, weight
    std::list<Module> module;
    std::list<EdgeList> edge_list; // E+A
};

class Module{
public:
     Module(/* args */){};
     ~Module(){};
protected:
private:
    

};

class Vertex{
public:
    Vertex():vertex_index(-1), vertex_weight(default_vertex_weight), vertex_data(LogicGate().get_is_clk_gate(), LogicGate().get_gate_index()){};
    Vertex( const gate_index_type& vertex_index, const LogicGate& vertex_data):
            vertex_index(vertex_index), 
            vertex_weight(vertex_data.get_weight_gate()), 
            vertex_data( vertex_data.get_is_clk_gate() , vertex_data.get_gate_index() ){};
    ~Vertex(){};
public:

    gate_index_type get_vertex_index() const { return vertex_index; };
    std::tuple<bool,gate_index_type> get_vertex_data() const { return vertex_data; };
protected:
private:
    gate_index_type vertex_index;
    float vertex_weight;
    std::tuple<bool,gate_index_type> vertex_data;
    //连接了哪些边
    //input output
};

class EdgeList{
public:
    EdgeList(): offset_array(0,0){};
    ~EdgeList(){};
    std::tuple<int,int> get_offset_array() const { return offset_array; }
    gate_index_type get_adjacency_array() const { return adjacency_array; }
    void add_edge(){};
protected:
private:
    gate_index_type adjacency_array;
    std::tuple<int,int> offset_array;
};

class Vertex_old{
public:
    Vertex_old():vertex_index(0),vertex_data( LogicGate() ){};
    Vertex_old(const gate_index_type& vertex_index, const LogicGate& vertex_data):
           vertex_index(vertex_index),          vertex_data(vertex_data){};
    ~Vertex_old(){};
    gate_index_type get_vertex_index() const { return vertex_index; };
    LogicGate get_vertex_data() const { return vertex_data; };
protected:
private:
    gate_index_type vertex_index;
    LogicGate vertex_data;
};

class LogicGate{
public:
    LogicGate():is_clk(false), gate_index(-1), 
                weight_gate(1.0), name_gate("Unknown"), type_gate("Unknown"), 
                input_gate_list{-1}, output_gate_list{-1}{};
    LogicGate(  const bool& is_clk,                         const gate_index_type& gate_index,
                //上面两个赛题相关 没写weight的初始化
                const std::string& type_gate,               const std::string& name_gate,
                const std::vector<int> input_gate_list,     const std::vector<int> output_gate_list):  
                is_clk(is_clk),                             gate_index(gate_index),
                name_gate(name_gate),                       type_gate(type_gate),
                input_gate_list(input_gate_list),           output_gate_list(output_gate_list){};
    // 类解析器
    //LogicGate(const std::string vlg_dot_v="sky130_fd_sc_hd__nand2_1 U569 ( .A(ff1_1[4]), .B(n395), .Y(n397) );"){
    // };
    ~LogicGate(){};
    bool get_is_clk_gate() const { return is_clk; };
    gate_index_type get_gate_index() const { return gate_index; };
        //
    float get_weight_gate() const { return weight_gate; };
    std::string get_name_gate() const { return name_gate; };
    std::string get_type_gate() const { return type_gate; };
    std::vector<gate_index_type> get_input_gate_list() const { return input_gate_list; };
    std::vector<gate_index_type> get_output_gate_list() const { return output_gate_list; };
protected:
private:
    bool is_clk;
    gate_index_type gate_index;
        //赛题相关，判断是否是timing-path起点\终点
        //其他都只是verilog里面的信息
    float weight_gate;
    std::string name_gate;
    std::string type_gate;
    std::vector<gate_index_type> input_gate_list;
    std::vector<gate_index_type> output_gate_list;
};

class Wire{
/*
wire    n1, ... n199, ... n142, ... n222,...
    nand2_1 U519 ( .A(n119), .B(n142), .Y(n222) );
    nand2_1 U524 ( .A(n119), .B(n142), .Y(n225) ); */

    // std::tuple<std::string, wire_index_type, std::tuple<int ,int>> wire;
    //好像可以直接用这个代替不用calss了
public:
    Wire(): name("n142"), wire_index(142), pin2pin{519,524}{};
    Wire(const std::string& name, const wire_index_type& wire_index, const std::vector<gate_index_type>& pin2pin):
        name(name), wire_index(wire_index),pin2pin(pin2pin){};
    ~Wire(){};
    std::string get_name() const { return name; };
    wire_index_type get_wire_index() const { return wire_index; };
    std::vector<gate_index_type> get_pin2pin() const { return pin2pin; };
protected:
private:
    std::string name;
    wire_index_type wire_index;
    std::vector<gate_index_type> pin2pin;
};

