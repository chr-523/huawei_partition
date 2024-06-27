// #include "../include/module.h"
#include "mymodule.h"
#include <cassert>
#include <algorithm>
#include <unordered_set>


//add internal edge -> pin and net
//((Name_type& name, int& low, int& high, Edge_type& type))
void Module::add_edge(Name_type& name, int& low, int& high, Edge_type& type){

    if (low > high) std::swap(low, high); // just in case

    if(low==0 and high == 0){
        name +="_0";
        Edge edge_(name,-1,-1,type);
        this -> internal_edge_list.push_back(edge_);
    }
    else if(low < 0 && high < 0 ){ // no range means signal edge
        // pin => a->[-1:-1] =>1    ->  add_edge(a, -1, -1, INPUT)
        // net => n1->[-1:-1]       ->  add_edge(n1, -1, -1, NORMAL)    
        Edge edge_(name, -1, -1, type); // name is index
        this -> internal_edge_list.push_back(edge_); //range = [-1, -1]
    }
    else{   // only in cases where high low is greater than 0
            // pin => dd->[3:0] =>2    =>  add_edge(dd, 15,  0, OUTPUT)
        Edge e_(name, low, high, Multi);
        this ->internal_edge_list.push_back(e_);
        for (size_t counter = 0; counter <= high ; counter++) // split the multi edge: to instantiation high + 1 Edges
        {   //the counter is from 0 to the high    
            //     -> add_adge(dd_0, -1,  0, OUTPUT)
            //     -> add_adge(dd_1, -1,  1, OUTPUT)
            //     -> add_adge(dd_2, -1,  2, OUTPUT)
            //     -> add_adge(dd_3, -1,  3, OUTPUT)
            Name_type se_name = name + '_' + std::to_string(counter) ; // se_name means signal_edge_name 
            Edge edge_(se_name,-1, counter, type); // name -> name_'counter'
            this -> internal_edge_list.push_back(edge_);
        }
    }
}

    //  same as add_edge(...,low,high,...)
void Module::add_edge(Name_type& name, Range& range, Edge_type& type){
    // eg. ↓
    // pin => a->[-1:-1] =>1    ->  add_edge(a, -1, -1, INPUT)
    // pin => dd->[3:0] =>2    =>  add_edge(dd, 15,  0, OUTPUT)
    //     -> add_adge(dd_0, -1, -1, OUTPUT)
    //     -> add_adge(dd_1, -1, -1, OUTPUT)
    //     -> add_adge(dd_2, -1, -1, OUTPUT)
    //     -> add_adge(dd_3, -1, -1, OUTPUT)
    // net => n1->[-1:-1]       ->  add_edge(n1, -1, -1, NORMAL)
    this -> add_edge(name, range.low, range.high, type);
}

void Module::add_instance(Name_type& type_name, Instance_index_type& name){
    // eg. ↓ 
    // instance => tu_test_ins, T1, A(n1)
    //          -> instance(tu_test_ins, T1)
    //              then push_back it
    Instance v_1(type_name, name); // type_name is only used to determine is_clk during initialization
    this -> internal_instance.push_back(v_1);
}

void connect_ins_edge(
    Module& gra, 
    Instance_index_type& instance_name, 
    Name_type& edge_name,
    Range& e_range){
    Name_type e_name;
    std::unordered_map< std::string, Edge* > edge_map;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edge_map[e.get_name()] = &e; 
    }

    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    auto it = edge_map.find(e_name);

    it -> second -> connect_instance(instance_name); // connect T1/T2 to n1/n2_3
}

size_t connect_mod_edge(
    Module& gra, 
    Instance_index_type& mod_name, 
    Name_type& edge_name,
    Range& e_range,
    Name_type& pin_name,
    size_t& subfix_counter){

    size_t is_normal =0;
    Name_type e_name;
    std::unordered_map< std::string, Edge* > edge_map;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edge_map[e.get_name()] = &e; 
    }

    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    auto it = edge_map.find(e_name);
    
    if(it->second->get_type()!=Multi){
        it -> second -> connect_instance(mod_name); // connect T1/T2 to n1/n2_3
    }
    else{
        int o_high= it->second->get_range().high;
        int o_low=it->second->get_range().low;
        for(int i = o_low; i <= o_high; ++i) {
            e_name = edge_name + '_' + std::to_string(i);
            Name_type p_name = pin_name + '_' + std::to_string(i+subfix_counter);
            auto it_ = edge_map.find(e_name);
            it -> second -> connect_instance(mod_name);
            gra.add_submodule_pin_edge(p_name);
            gra.add_submodule_pin_edge(e_name);
        }
        is_normal = o_high-o_low;
    }
    
    return is_normal;

}


size_t connect_mod_edge(
    Module& gra, 
    Instance_index_type& mod_name, 
    Name_type& edge_name,
    Range& e_range){

    size_t is_normal =0;
    Name_type e_name;
    std::unordered_map< std::string, Edge* > edge_map;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edge_map[e.get_name()] = &e; 
    }

    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    auto it = edge_map.find(e_name);

    it -> second -> connect_instance(mod_name); // connect T1/T2 to n1/n2_3

    return is_normal;

}



//friend void Module::connect_ins_edg(~~)
void connect_ins_edge(
    Module& gra, 
    Instance_index_type& instance_name, 
    std::queue< Name_type >& edge_name_queue,
     std::queue< Range >& range_queue){
    // instance => tu_test_ins, T1, A(n1->[-2147483648:-2147483648])B(n2->[-2147483648:3])
    // instance => tu_test_ins, T2, B(n2->[-2147483648:3])
    ;
    // at first find the ins
    auto this_instance = std::find_if( gra.internal_instance.begin(), 
        gra.internal_instance.end(), [&]( Instance v) {
        return std::get<1>( v.get_instance_data() ) == instance_name;
    });
        // Using a hash table to store edges in a vector for quick access
    std::unordered_map< std::string, Edge* > edge_map;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edge_map[e.get_name()] = &e; 
    }
        // queue is not empty do ->
    while (!range_queue.empty() && !edge_name_queue.empty()) {
        Name_type e_name_temp = edge_name_queue.front();
        Name_type e_name;
        Range e_range = range_queue.front();
        edge_name_queue.pop();
        range_queue.pop();

        if(e_range.low < 0 && e_range.high < 0){
                // which means .A(n1->[-2147483648:-2147483648])
            e_name = e_name_temp;
        }
        else{   // which means .B(n2->[-2147483648:3])
            e_name = e_name_temp + '_' + std::to_string(e_range.high);
        }

        this_instance -> connect_edge(e_name); // connect n1/n2_3 to T1/T2
        auto it_ = edge_map.find(e_name); // find edge
        if (it_ != edge_map.end()) {
            // if found, connect_instance
            it_ -> second -> connect_instance(instance_name); // connect T1/T2 to n1/n2_3
        }
    }
};


void Module::add_module( 
    Module_index_type module_index, 
    Module* G_1){
    this -> submodule.push_back(std::make_tuple(module_index, G_1));
    std::vector<Edge_index_type> e_ = {"*"};
    this -> submodule_pin_edge.push_back(e_);
};
