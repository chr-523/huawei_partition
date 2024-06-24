#include "edge.h"
#include "instance.h"
#include <cassert>
#include <iostream>

void Edge::split_edge(){
   //it is done in the Graph::add_adge and Graph::connecd_v_e 
   int a = 1;
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
    
void Edge::ci_find_direction_ins(Name_type& pin_name){
    this -> adj_array_direction.push_back(find_direction_ins(pin_name));
}

Direction Edge::find_direction_ins(Name_type &pin_name)
{
    std::unordered_set<std::string> input_table = {   
        "A", "A1", "A2", "A3", "A4",
        "B", "B2",  "C", "C1", "C2",
        "D", "RESET_B", "CLK"
    }; // these two table should be improved
    std::unordered_set<std::string> output_table = {   
        "X", "Y", "Q",//补一下
        "?",
    };
    // (.find() != end) means pin_name can be found in input_table
    //      means pin_name -> input
    // for ins, pin_name is input means edge to v
    bool is_e2v = input_table.find(pin_name) != input_table.end();
    bool is_v2e = output_table.find(pin_name) != output_table.end();

    if (is_v2e){
        return v_to_edge;
    }
    else if(is_e2v){
        return edge_to_v;
    }
    else{ // can not find In/Out-put
        std::cerr << "New pin name: " << pin_name << " should be add in table." << std::endl;
    }
    std::cerr << "Something wrong with the name:" << pin_name << "." << std::endl;

    return Direction();
}

void Edge::ci_find_direction_mod(Name_type &pin_name, Edge_type e_type) {
    this -> adj_array_direction.push_back(find_direction_mod(pin_name, e_type));
}

Direction Edge::find_direction_mod(Name_type &pin_name, Edge_type e_type)
{
    if (e_type == INPUT){
        // For submaodules, this edge is input, 
        // so for modules, the direction of this edge is from edge to vertex(sub_module)
        return edge_to_v;
    }
    else if (e_type == OUTPUT){
        // For submaodules, this edge is onput, 
        // so for modules, the direction of this edge is from vertex(sub_module) tp edge
        return v_to_edge;
    }
    else{
        std::cerr << "Submodule edge type made a mistake." << std::endl;
    }

    return Direction();
};
