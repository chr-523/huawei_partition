#include "edge.h"
#include "instance.h"

void Instance::connect_edge(Edge& edge){
    // instance => tu_test_ins, T1, A(n5->[-1:-1])
    // get the edge's index(= name) and push into edgelist
    Name_type edge_name = edge.get_name();    
    this -> connect_edge(edge_name);
}

void Instance::connect_edge(const Name_type& edge_name){
    // instance => tu_test_ins, T1, A(n5->[-1:-1])
    // connect_edge_list.push_back("n5")
    this -> connect_edge_list.push_back(edge_name);
}

//Determine whether it is clk through instance_name's prefix
bool Instance::is_clk(const Name_type& name){
    std::unordered_set<std::string> clk_table = {   
        "sky130_fd_sc_hd__dfrtp_1", 
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
        "sky130_fd_sc_hd__dfxtp_4" 
    };

    return clk_table.find(name) != clk_table.end();
};
