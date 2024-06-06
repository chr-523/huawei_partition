// #include "../include/graph.h"
#include "graph.h"
#include <algorithm>
#include <unordered_set>

//Determine whether it is instance or module through instance_name's prefix
//If instance -> true     if module -> false*
bool Graph::is_instance_type(Name_type& instance_name){
    Name_type instance_prefix = "sky130_fd_sc_hd__";
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

void Graph::add_edge(Name_type& name, int& low, int& high, Edge_type& type){

    if (low > high) std::swap(low, high); // just in case

    if(low < 0 && high < 0 ){ // no range means signal edge
        // pin => a->[-1:-1] =>1    ->  add_edge(a, -1, -1, INPUT)
        // net => n1->[-1:-1]       ->  add_edge(n1, -1, -1, NORMAL)    
        Edge edge_(name, type); // name is index
        this -> internal_edge_list.push_back(edge_); //range = [-1, -1]
    }
    else{   // only in cases where high low is greater than 0
            // pin => dd->[3:0] =>2    =>  add_edge(dd, 15,  0, OUTPUT)
        for (size_t counter = 0; counter <= high ; counter++) // split the multi edge: to instantiation high + 1 Edges
        {   //the counter is from 0 to the high    
            //     -> add_adge(dd_0, -1, -1, OUTPUT)
            //     -> add_adge(dd_1, -1, -1, OUTPUT)
            //     -> add_adge(dd_2, -1, -1, OUTPUT)
            //     -> add_adge(dd_3, -1, -1, OUTPUT)
            Name_type se_name = name + '_' + std::to_string(counter) ; // se_name means signal_edge_name 
            Edge edge_(se_name, type); // name -> name_'counter'
            this -> internal_edge_list.push_back(edge_);
        }
    }
}

    //  same as add_edge(...,low,high,...)
void Graph::add_edge(Name_type& name, Range& range, Edge_type& type){
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

void Graph::add_instance(Name_type& type_name, Vertex_index_type& name){
    // eg. ↓ 
    // instance => tu_test_ins, T1, A(n1)
    //          -> vertex(tu_test_ins, T1)
    //              then push_back it
    Vertex v_1(type_name, name); // type_name is only used to determine is_clk during initialization
    this -> vertex.push_back(v_1);
}

void Graph::connect_ins_edge(Vertex_index_type& vertex_name, std::queue< Name_type >& edge_name_queue, std::queue< Range >& range_queue){
    // instance => tu_test_ins, T1, A(n1->[-2147483648:-2147483648])B(n2->[-2147483648:3])
    // instance => tu_test_ins, T2, B(n2->[-2147483648:3])
    ;
    // at first find the ins
    auto it = std::find_if( this -> get_vertex_list().begin(), 
                            this -> get_vertex_list().end(), [&]( Vertex v) {
        return std::get<1>( v.get_vertex_data() ) == vertex_name;
    });
        // Using a hash table to store edges in a vector for quick access
    std::unordered_map< std::string, Edge* > edgeMap;
        // creat the hash table
    for (auto& e : this -> get_internal_edge_list()){
        edgeMap[e.get_name()] = &e; 
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

        it -> connect_edge(e_name); // connect n1/n2_3 to T1/T2
        auto it_ = edgeMap.find(e_name); // find edge
        if (it_ != edgeMap.end()) {
            // if found, connect_vertex
            it_ -> second -> connect_vertex(vertex_name); // connect T1/T2 to n1/n2_3
        }
    }
};

// add module...to be done
void Graph::add_module(Module_index_type& module_name){
    // eg. ↓ 
    // instance => tu_test_mod, T1, A(n1->[-2147483648:-2147483648])
    // instance => tu_test_mod, T2, B(n2->[-2147483648:5])
    // instance => tu_test_mod, T3, C(GROUP_NETS {(n1->[-1:-1])(n2->[-1:-1])(n5->[15:0])})
    // instance => tu_test_mod, T4, D(n7->[7:0])
    //              then push_back it
    Graph* G_1;
    this -> subgraph.push_back(G_1);
};

// connect module and edge...to be done
void Graph::connect_mod_edge(
    Module_index_type& module_name, 
    std::queue< Name_type >& edge_name_queue, 
    std::queue< Range >& range_queue){
    // instance => tu_test_mod, M1, A(n1->[-2147483648:-2147483648])
    // instance => tu_test_mod, M2, B(n2->[-2147483648:5])
    // instance => tu_test_mod, M3, C(GROUP_NETS {(n1->[-1:-1])(n2->[-1:-1])(n5->[15:0])})
    // instance => tu_test_mod, M4, D(n7->[7:0])

    // at first find the mod
    auto it = std::find_if( this -> get_subgraph_list().begin(), 
                            this -> get_subgraph_list().end(), [&]( Graph* g) {//*?
        return g -> get_module_name() == module_name;
    });

        // Using a hash table to store edges in a vector for quick access
    std::unordered_map< std::string, Edge* > edgeMap;
        // creat the hash table
    for (auto& e : this -> get_internal_edge_list()){
        edgeMap[e.get_name()] = &e; 
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
                // instance => tu_test_mod, M3, C(GROUP_NETS {(n1->[-1:-1])(n2->[-1:-1])(n5->[15:0])})
                // instance => tu_test_mod, M4, D(n7->[7:0])
            e_name = e_name_temp;
        }
        else if(e_range.low < 0 && e_range.high >= 0){   // which means .B(n2->[-2147483648:3])
            
        }
        else if(e_range.low >= 0 && e_range.high >= 0){// which means .D(n7->[7:0])
        
        }
        else{// which means .C(GROUP_NETS {(n1->[-1:-1])(n2->[-1:-1])(n5->[15:0])}) or ?

        }

        // it -> connect_edge(e_name); // connect n1/n2_3 to M1/M2
        // auto it_ = edgeMap.find(e_name); // find edge
        // if (it_ != edgeMap.end()) {
        //     // if found, connect_vertex
        //     it_ -> second -> connect_vertex(vertex_name); // connect T1/T2 to n1/n2_3
        // }


    }
};
