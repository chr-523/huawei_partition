#include "data.h"

void Edge::split_edge(){
    
}

void Edge::connect_vertex(Vertex& vertex){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])        
    // get the vertex's index and push into adjancy_array
    this -> adjacency_array.push_back(std::get<1>(vertex.get_vertex_data()));
}


void Vertex::connect_edge(Edge& edge){
    // instance => tu_test_ins, T1, A(n5->[-1:-1])
    // get the edge's index and push into edgelist
    this -> connect_edgelist.push_back(edge.get_edge_index());
}

//Determine whether it is clk through vertex_name
bool Vertex::is_clk(const Name_type& name){    
    std::unordered_set<std::string> clk_table = 
        {   "sky130_fd_sc_hd__dfrtp_1", 
            "sky130_fd_sc_hd__dfstp_1", 
            "sky130_fd_sc_hd__dfrbp_1", 
            "sky130_fd_sc_hd__dfbbp_1", 
            "sky130_fd_sc_hd__edfxtp_1", 
            "sky130_fd_sc_hd__dfxtp_2", 
            "sky130_fd_sc_hd__edfxbp_1", 
            "sky130_fd_sc_hd__dfrtp_2", 
            "sky130_fd_sc_hd__dfsbp_1", 
            "sky130_fd_sc_hd__dfxbp_1", 
            "sky130_fd_sc_hd__dfrtp_4", 
            "sky130_fd_sc_hd__dfxbp_2", 
            "sky130_fd_sc_hd__dfxtp_1", 
            "sky130_fd_sc_hd__dfstp_2", 
            "sky130_fd_sc_hd__dfxtp_4" };
    return clk_table.find(name) != clk_table.end();
};

//Determine whether it is instance or module through instance_name
bool Graph::is_instance_type(Name_type& instance_name){
    Name_type instance_prefix = "sky130_fd_sc_hd__";
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

void Graph::add_edge(Name_type& name, int& low, int& high, Edge_type& type){

    if (low > high) std::swap(low, high); // just in case

    if(low < 0 && high < 0 ){ // no range means signal edge
        Edge edge_(name, -1, type); // is index need?
        this -> edge_list.push_back(edge_); //range = [-1, -1]
    }
    else{//only in cases where high low is greater than 0
        for (size_t counter = 0; counter <= high ; counter++) // split the multi edge: to instantiation high + 1 Edges
        {   //the counter is from 0 to high
            Name_type se_name = name + '_' + std::to_string(counter) ; // se = signal edge 
            Edge edge_(se_name, -1, type); // name -> name_counter
            this -> edge_list.push_back(edge_);
        }
    }
}

void Graph::add_edge(Name_type& name, Range& range, Edge_type& type){
    this -> add_edge(name, range.low, range.high, type);
}

void Graph::add_instance(Name_type& name){
    Vertex v_1(name);
    this -> vertex.push_back(v_1);
}