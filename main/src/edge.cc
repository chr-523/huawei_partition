#include "edge.h"
#include "instance.h"

void Edge::split_edge(){
   //it is done in the Graph::add_adge and Graph::connecd_v_e 
}

void Edge::connect_instance(Instance& instance){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])        
    // get the instance's index and push into adjancy_array
    Name_type instance_name = std::get<1>(instance.get_instance_data());
    this -> connect_instance(instance_name);
}

void Edge::connect_instance(Name_type& instance_name){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])
    //  adj.pushback("T1")
    this -> adjacency_array.push_back(instance_name); 
}

void Edge::connect_instance_direction(Direction& direc){
    // instance => tu_test_ins, T1, A(n5->[-1: -1])
    //  adj.pushback("T1")
    this -> adjacency_array_direction.push_back(direc); 
}

