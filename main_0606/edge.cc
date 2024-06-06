#include "edge.h"
#include "vertex.h"

void Edge::split_edge(){
   //it is done in the Graph::add_adge and Graph::connecd_v_e 
}

void Edge::connect_vertex(Vertex& vertex){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])        
    // get the vertex's index and push into adjancy_array
    Name_type vertex_name = std::get<1>(vertex.get_vertex_data());
    this -> connect_vertex(vertex_name);
}

void Edge::connect_vertex(Name_type& vertex_name){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])
    //  adj.pushback("T1")
    this -> adjacency_array.push_back(vertex_name); 
}
