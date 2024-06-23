// #include "../include/module.h"
#include "mymodule.h"
#include <cassert>
#include <algorithm>
#include <unordered_set>


//add internal edge -> pin and net
//((Name_type& name, int& low, int& high, Edge_type& type))
void Module::add_edge(Name_type& name, int& low, int& high, Edge_type& type){

    if (low > high) std::swap(low, high); // just in case

    if(low < 0 && high < 0 ){ // no range means signal edge
        // pin => a->[-1:-1] =>1    ->  add_edge(a, -1, -1, INPUT)
        // net => n1->[-1:-1]       ->  add_edge(n1, -1, -1, NORMAL)    
        Edge edge_(name, -1, -1, type); // name is index
        this -> internal_edge_list.push_back(edge_); //range = [-1, -1]
        
        if( type != NORMAL ){
            this -> IO_type_map[name] = type;
        }
        else{ // type == NORMAL
            // do nothing
        }
    }
    else{   // only in cases where high low is greater than 0
            // pin => dd->[3:0] =>2    =>  add_edge(dd, 15,  0, OUTPUT)
        for (size_t counter = 0; counter <= high ; counter++) // split the multi edge: to instantiation high + 1 Edges
        {   //the counter is from 0 to the high    
            //     -> add_adge(dd_0, -1,  0, OUTPUT)
            //     -> add_adge(dd_1, -1,  1, OUTPUT)
            //     -> add_adge(dd_2, -1,  2, OUTPUT)
            //     -> add_adge(dd_3, -1,  3, OUTPUT)
            Name_type se_name = name + '_' + std::to_string(counter) ; // se_name means signal_edge_name 
            Edge edge_(se_name,-1, counter, type); // name -> name_'counter'
            this -> internal_edge_list.push_back(edge_);

                    
            if( type != NORMAL ){
                this -> IO_type_map[name] = type;
            }
            else{ // type == NORMAL
                // do nothing
            }
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

//friend void Module::connect_ins_edg(~~)
void connect_ins_edge(
    Module& gra, 
    Instance_index_type& instance_name, 
    std::queue< Name_type >& edge_name_queue,
    std::queue< Name_type >& pin_name_queue,
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
    std::unordered_map< std::string, Edge* > edgeMap;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edgeMap[e.get_name()] = &e; 
    }


    assert(edge_name_queue.size() == pin_name_queue.size());
    assert(range_queue.size() == pin_name_queue.size());
    
        // queue is not empty do ->
    while (
        !edge_name_queue.empty() && 
        !pin_name_queue.empty() &&
        !range_queue.empty() ) {

        Name_type e_name_temp = edge_name_queue.front();
        Name_type e_name;
        Name_type p_name = pin_name_queue.front();
        Range e_range = range_queue.front();
        edge_name_queue.pop();
        pin_name_queue.pop();
        range_queue.pop();

        if(e_range.low < 0 && e_range.high < 0){
                // which means .A(n1->[-2147483648:-2147483648])
            e_name = e_name_temp;
        }
        else{   // which means .B(n2->[-2147483648:3])
            e_name = e_name_temp + '_' + std::to_string(e_range.high);
        }

        this_instance -> connect_edge(e_name); // connect n1/n2_3 to T1/T2
        auto it_ = edgeMap.find(e_name); // find edge
        if (it_ != edgeMap.end()) {
            // if found, connect_instance
            it_ -> second -> connect_instance(instance_name); // connect T1/T2 to n1/n2_3
            it_ -> second -> ci_find_direction_ins(p_name); // true -> instance
        }
    }
}


void Module::add_module(){
    std::vector< std::tuple< Edge_index_type, Edge_index_type >> connect_edge_list;
    Module_index_type module_index = default_name;
    Module* G_1;
    this -> submodule.push_back(std::make_tuple(module_index, connect_edge_list, G_1));
};

void Module::add_module( 
    std::vector< std::tuple< Edge_index_type, Edge_index_type >> connect_edge_list, 
    Module_index_type module_index, 
    Module* G_1){
    this -> submodule.push_back(std::make_tuple(module_index, connect_edge_list, G_1));
};

// This function is used to handle the following three situations:
    // ~~ instance => tu_test_mod, M1, A(n1->[-2147483648:-2147483648])
    // ~~ instance => tu_test_mod, M2, B(n2->[-2147483648:5])
    // ~~ instance => tu_test_mod, M4, D(n7->[7:0])
void connect_mod_edge(
    Module& gra,
    Module_index_type& module_name, 
    std::queue< Name_type >& edge_name_queue, 
    std::queue< Name_type >& pin_name_queue,
    std::queue< Range >& range_queue){
    // at first find the mod
    auto this_module = std::find_if(gra.submodule.begin(), 
        gra.submodule.end(), [&](const Sub_Module_type& submodule){            
            return std::get<0>(submodule) == module_name;
        });

        // Using a hash table to store edges in a vector for quick access
    std::unordered_map< std::string, Edge* > edgeMap;
        // creat the hash table
    for (auto& e : gra.internal_edge_list){
        edgeMap[e.get_name()] = &e; 
    }

    assert(edge_name_queue.size() == pin_name_queue.size());
    assert(range_queue.size() == pin_name_queue.size());
    
        // queue is not empty do ->
    while (
        !edge_name_queue.empty() && 
        !pin_name_queue.empty() &&
        !range_queue.empty() ) {
        Name_type e_name/*_temp*/ = edge_name_queue.front();
        // Name_type e_name; // temp has already been processed outside the function
        Name_type p_name = pin_name_queue.front();
        Range e_range = range_queue.front();
        pin_name_queue.pop();
        edge_name_queue.pop();
        range_queue.pop();

        assert( e_range.low < 0 ); // only signal edge
        
        std::tuple<Edge_index_type, Edge_index_type> e_c_l = std::make_tuple(e_name, p_name);
        std::get<1>(*this_module).push_back(e_c_l);

        std::unordered_map< Name_type, Edge_type > IOMAP;
        IOMAP = std::get<2>(*this_module)->get_IO_type_map();

        auto IO_ = IOMAP.find(p_name); // find IO_type
        if( IO_ == IOMAP.end() ){ 
            std::cerr << "This pin_name can not be found. -> " << p_name << std::endl;
        }
        auto it_ = edgeMap.find(e_name); // find edge
        if ( it_ != edgeMap.end() ){
            // if found, connect_instance
            it_ -> second -> connect_instance(module_name); // connect T1/T2 to n1/n2_3
            it_ -> second -> ci_find_direction_mod(p_name, IO_ -> second);
        }
        else{
            
            std::cerr << "This edge_name can not be found. -> " << e_name << std::endl;
        }
    }
};



void assign_2_edge(
    Module &gra,
    Edge_index_type &edge_name_1,
    Edge_index_type &edge_name_2)
{

    std::unordered_map< std::string, Edge* > edgeMap;        
    for (auto& e : gra.internal_edge_list){
        edgeMap[e.get_name()] = &e; 
    }

    auto it_1 = edgeMap.find(edge_name_1); // find edge
    auto it_2 = edgeMap.find(edge_name_2); // find edge
    bool can_find_both_it1and2 = !(it_1 == edgeMap.end() || it_2 == edgeMap.end());

    if ( can_find_both_it1and2 ){// both e_1 and e_2 has found.
        std::vector< Edge_index_type > ee_1 = it_1 -> second -> get_adjacency_array();
        std::vector< Edge_index_type > ee_2 = it_2 -> second -> get_adjacency_array();
        // std::vector< Edge_index_type > ee_c;
        std::vector< Direction > dd_1 = it_1 -> second -> get_adj_array_direction();
        std::vector< Direction > dd_2 = it_2 -> second -> get_adj_array_direction();
        // std::vector< Direction > dd_c;

        assert(ee_1.size() == dd_1.size());
        assert(ee_2.size() == dd_2.size());

        size_t temp_size = ee_1.size() + ee_2.size();

        dd_1.reserve(temp_size);
        ee_1.reserve(temp_size); //Pre-allocate sufficient space
        ee_1.insert(ee_1.end(), ee_2.begin(), ee_2.end());
        dd_1.insert(dd_1.end(), dd_2.begin(), dd_2.end());
            // it_2 -> second -> set_adj_byassign(ee_1); // was abandoned
            // it_1 -> second -> set_adj_byassign(ee_2);
        it_1 -> second -> set_adjacency_array(ee_1);
        it_2 -> second -> set_adjacency_array(ee_1);
        it_1 -> second -> set_adj_array_direction(dd_1);
        it_2 -> second -> set_adj_array_direction(dd_1);
    }
    else{ // canot be found
        if (it_1 == edgeMap.end() && it_2 == edgeMap.end()){
            std::cerr << "    Cannot find both " << edge_name_1 << " and " << edge_name_2 << ".\n";
        } 
        else if ( it_1 == edgeMap.end() ){
            std::cerr << "    Cannot find " << edge_name_1 << ".\n";
        } 
        else{
            std::cerr << "    Cannot find " << edge_name_2 << ".\n";
        }
    }
};
