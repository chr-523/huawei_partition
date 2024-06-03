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
//Determine if it is clk based on the entered name
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

bool Graph::is_instance_type(Name_type& instance_name){
    Name_type instance_prefix = "sky130_fd_sc_hd__";
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

void Graph::add_edge(){

}

void Graph::add_instance(){
    
}