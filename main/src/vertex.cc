#include "vertex.h"

void Graph::read_graph_data(const Module& gra_data, size_t* level, Name_type& mod_index) {

    edge_map_type edge_map;
        // creat the hash table
    std::vector< Edge > temp_el = gra_data.get_internal_edge_list();
    for (auto& e : temp_el){
        edge_map[e.get_name()] = &e; 
    };

    (*level)++;
    // Recursively perform operations on all submodules
    for( auto& subModule : gra_data.get_submodule_list() ){ //?
    // std::vector< Sub_Module_type > submodule_temp = gra_data.get_submodule_list();
    // for( auto& subModule : submodule_temp ){
        Module* sub_data = std::get<2>(subModule);
        if (sub_data != nullptr) {
            read_graph_data(*sub_data, level, std::get<0>(subModule)); // 递归调用
            // ins -> mod may be in here
            int b = 1;
            (*level)--;
        }
    }

    for( auto& ins_data: gra_data.get_internal_instance() ){
        Name_type ins_name = std::get<1>( ins_data.get_instance_data() );
        Name_type all_name =  mod_index + "_" + ins_name; // ins all name
        bool is_clk = std::get<0>( ins_data.get_instance_data() );
        weight_type ins_weight = ins_data.get_instance_weight();

        std::vector< std::pair< std::string, weight_type > > adj_v2e;
        std::vector< std::pair< std::string, weight_type > > adj_e2v;
        
        degree_type out_degree = 0;
        degree_type in_degree = 0;
        // bool is_e2v_output;
        for(auto& e_name : ins_data.get_connect_edge() ){
            auto e_data = edge_map.find( e_name ); // find edge
            if ( e_data != edge_map.end() ){
                Direction this_ins_to_edge;
                if ( e_data -> second -> get_type() == INPUT ){
                    in_degree ++; // in_pin connect to ins
                }
                else if( e_data -> second -> get_type() == OUTPUT ){
                    out_degree ++; // out_pin connect to ins
                }
                else{ // e_data -> second -> get_type() == NORMAL
                    std::vector< Direction >  array_direction = e_data -> second -> get_adj_array_direction();
                    std::vector< Instance_index_type > adj_array = e_data -> second -> get_adjacency_array();
                    size_t v2e = 0;
                    size_t e2v = 0;
                    for (size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {

                        if( array_direction[ins_index] == v_to_edge ){
                            v2e++;
                            if(adj_array[ins_index] != ins_name){
                                adj_v2e.push_back({
                                    mod_index+"_"+adj_array[ins_index],
                                    e_data -> second -> get_weight() });
                                // out_degree++;
                            }
                            else{
                                this_ins_to_edge = v_to_edge;
                                v2e--;
                            }
                        }
                        else{ //  array_direction[ins_index] == edge_to_v
                            e2v++;                           
                            if(adj_array[ins_index] != ins_name){
                                adj_e2v.push_back({
                                    mod_index+"_"+adj_array[ins_index],
                                    e_data -> second -> get_weight() });                                    
                                    // in_degree++;
                            }
                            else{
                                this_ins_to_edge = edge_to_v;
                                e2v--;
                            }
                        }                    
                    }
                }
            }
        }
        int a = 1;
        // if( this_ins_to_edge == v_to_edge){
        add_adj_plus(all_name,adj_e2v);
        add_adj_minus(all_name,adj_v2e);
        out_degree += adj_e2v.size();
        in_degree += adj_v2e.size();
        adj_e2v.clear();
        adj_v2e.clear();
        // }
        // else{ // this_ins_to_edge == edge_to_v
        //     add_adj_plus(all_name,adj_v2e);
        //     add_adj_minus(all_name,adj_e2v);
        //     out_degree += adj_v2e.size();
        //     in_degree += adj_e2v.size();
        // }
        int b = 1;
        Vertex temp_v( all_name, is_clk, ins_weight, out_degree, in_degree);
        // std::cout << all_name << std::endl;
        addVertex(temp_v);

    }
};//11    