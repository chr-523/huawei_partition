// #include "../include/module.h"
#include "mymodule.h"
#include <cassert>
#include <algorithm>
#include <unordered_set>

Direction fins_ins_pin_direction(Name_type &pin_name){

    std::unordered_set<std::string> input_table = {   
        "A", "A0", "A1", "A2", "A3", "A4", "A_N",
        "B", "B1", "B2", "B_N", "C", "C1", "C2",
        "D", "RESET_B", "CLK", "S","HI"
    }; // these two table should be improved
    std::unordered_set<std::string> output_table = {   
        "X", "Y", "Q","LO",//也许需要补一下
        "?",
    };
    // (.find() != end) means pin_name can be found in input_table
    //      means pin_name -> input
    // for ins, pin_name is input means edge to v
    bool is_e2v = input_table.find(pin_name) != input_table.end();
    bool is_v2e = output_table.find(pin_name) != output_table.end();

    if (is_v2e){
        return v2e;
    }
    else if(is_e2v){
        return e2v;
    }
    else{ // can not find In/Out-put
        std::cerr << "New pin name: " << pin_name << " should be add in table." << std::endl;
    }
    std::cerr << "Something wrong with the name:" << pin_name << "." << std::endl;

    return Direction();
}



//add internal edge -> pin and net
//((Name_type& name, int& low, int& high, Edge_type& type))
void Module::add_edge(Name_type& name, int& low, int& high, Edge_type& type){

    if (low > high) std::swap(low, high); // just in case

    if(low==0 and high == 0){
        name +="_0";
        Edge edge_(name,-1,-1,type);
        this -> E_map[name] = this -> internal_edge_list.size();
        this -> internal_edge_list.push_back(edge_);
    }
    else if(low < 0 && high < 0 ){ // no range means signal edge
        // pin => a->[-1:-1] =>1    ->  add_edge(a, -1, -1, INPUT)
        // net => n1->[-1:-1]       ->  add_edge(n1, -1, -1, NORMAL)    
        Edge edge_(name, -1, -1, type); // name is index
        this -> E_map[name] = this -> internal_edge_list.size();
        this -> internal_edge_list.push_back(edge_); //range = [-1, -1]
    }
    else{   // only in cases where high low is greater than 0
            // pin => dd->[3:0] =>2    =>  add_edge(dd, 15,  0, OUTPUT)
        Edge e_(name, low, high, Multi);
        this -> E_map[name] = this -> internal_edge_list.size();
        this ->internal_edge_list.push_back(e_);
        for (size_t counter = 0; counter <= high ; counter++) // split the multi edge: to instantiation high + 1 Edges
        {   //the counter is from 0 to the high    
            //     -> add_adge(dd_0, -1,  0, OUTPUT)
            //     -> add_adge(dd_1, -1,  1, OUTPUT)
            //     -> add_adge(dd_2, -1,  2, OUTPUT)
            //     -> add_adge(dd_3, -1,  3, OUTPUT)
            Name_type se_name = name + '_' + std::to_string(counter) ; // se_name means signal_edge_name 
            Edge edge_(se_name,-1, counter, type); // name -> name_'counter'
            this -> E_map[se_name] = this -> internal_edge_list.size();
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
    this -> Ins_map[name] = this -> internal_instance.size();
    this -> internal_instance.push_back(v_1);
}

void connect_ins_edge(
    Module& gra, 
    Instance_index_type& instance_name, 
    Name_type& pin_name,
    Name_type& edge_name,
    Range& e_range){
    Name_type e_name;
    Direction direc;
        // creat the hash table
    // for (auto& e : gra.internal_edge_list){
    //     edge_map[e.get_name()] = &e; 
    // }
    
    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    // auto it = edge_map.find(e_name);
    auto it_ = gra.E_map.find(e_name);

    direc = fins_ins_pin_direction(pin_name);

    gra.internal_edge_list[it_ -> second].connect_instance(instance_name);
    gra.internal_edge_list[it_ -> second].connect_instance_direction(direc);
    gra.internal_instance.back().add_edge_direction(direc);
    int  a = 1;
    // it -> second -> connect_instance(instance_name); // connect T1/T2 to n1/n2_3
}

int connect_mod_edge(
    Module& gra, 
    Instance_index_type& mod_name, 
    Name_type& edge_name,
    Range& e_range,
    Name_type& pin_name,
    size_t& subfix_counter,
    Direction& e_m_type){

    int is_normal =-1;
    Name_type e_name;

    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    auto it = gra.E_map.find(e_name);
    size_t index = it -> second;
    if(gra.internal_edge_list[index].get_type()!=Multi){
        gra.internal_edge_list[index].connect_instance(mod_name);
        gra.internal_edge_list[index].connect_instance_direction(e_m_type);
    }
    else{
        int o_high= gra.internal_edge_list[index].get_range().high;
        
        int o_low= gra.internal_edge_list[index].get_range().low;

        int t = subfix_counter;

        for(int i = o_low; i <= o_high; ++i) {
            e_name = edge_name + '_' + std::to_string(i);
            Name_type p_name = pin_name + '_' + std::to_string(t);
            t++;
            auto it_ = gra.E_map.find(e_name);
            size_t index_ = it_ -> second;
            gra.internal_edge_list[index_].connect_instance(mod_name);
            gra.internal_edge_list[index].connect_instance_direction(e_m_type);
            gra.add_submodule_pin_edge(p_name);
            gra.add_submodule_pin_edge(e_name);
        }
        is_normal = o_high-o_low;
    }
    
    return is_normal;

}


int connect_mod_edge(
    Module& gra, 
    Instance_index_type& mod_name, 
    Name_type& edge_name,
    Range& e_range,
    Direction& e_m_type){

    int is_normal =-1;
    Name_type e_name;


    if(e_range.low < 0 && e_range.high < 0){
            // which means .A(n1->[-2147483648:-2147483648])
        e_name = edge_name;
    }
    else{   // which means .B(n2->[-2147483648:3])
        e_name = edge_name + '_' + std::to_string(e_range.high);
    }

    auto it = gra.E_map.find(e_name);

    gra.internal_edge_list[it->second].connect_instance(mod_name); // connect T1/T2 to n1/n2_3
    gra.internal_edge_list[it->second].connect_instance_direction(e_m_type);
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
    // at first find the ins
    auto this_instance = std::find_if( gra.internal_instance.begin(), 
        gra.internal_instance.end(), [&]( Instance v) {
        return std::get<1>( v.get_instance_data() ) == instance_name;
    });
        // Using a hash table to store edges in a vector for quick access

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

        this_instance -> connect_edge(e_name); // connect n1/n2_3 to T1/T2 //int a = 1;

        auto it_ = gra.E_map.find(e_name); // find edge
        if (it_ != gra.E_map.end()) {
            // if found, connect_instance
            gra.internal_edge_list[it_->second].connect_instance(instance_name); // connect T1/T2 to n1/n2_3
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
