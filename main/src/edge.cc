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

    std::unordered_set<std::string> input_table = {   
        "A", "A1", "A2", "A3", "A4",
        "B", "B2",  "C", "C1", "C2",
        "D", "RESET_B", "CLK"
    }; // these two table should be improved
    std::unordered_set<std::string> output_table = {   
        "X", "Y", "Q",
        "?"
    };
    // (.find() != end) means pin_name can be found in input_table
    //      means pin_name -> input
    bool is_v2e = input_table.find(pin_name) != input_table.end();
    bool is_e2v = output_table.find(pin_name) != output_table.end();

    if (is_v2e){
        this -> adj_array_direction.push_back(v_to_edge);
    }
    else if(is_e2v){
        this -> adj_array_direction.push_back(edge_to_v);
    }
    else{ // can not find In/Out-put
        std::cerr << "New pin name: " << pin_name << "should be add in table." << std::endl;
    }


}
void Edge::ci_find_direction_mod(Name_type &pin_name, Edge_type e_type) {
 
    if (e_type == INPUT){
        // For submaodules, this edge is input, 
        // so for modules, the direction of this edge is from edge to vertex(sub_module)
        this -> adj_array_direction.push_back(edge_to_v);
    }
    else if (e_type == OUTPUT){
        // For submaodules, this edge is input, 
        // so for modules, the direction of this edge is from edge to vertex(sub_module)
        this -> adj_array_direction.push_back(v_to_edge);
    }
    else{
        std::cerr << "Submodule edge type made a mistake." << std::endl;
    }

};
