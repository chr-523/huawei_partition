#include "vertex.h"

void Graph::read_graph_data(const Module& gra_data,  Name_type& prefix, Name_type& mod_index) {

    edge_map_type edge_map;
        // creat the hash table
    std::vector< Edge > temp_el = gra_data.get_internal_edge_list();

    for (auto& e : temp_el){
        edge_map[e.get_name()] = &e; 
    };
    std::vector< Sub_Module_type > sub_module_list = gra_data.get_submodule_list() ;
    bool is_sub_module_empty = sub_module_list.empty();
    std::unordered_map< Name_type, Name_type > mod_to_ins;
    std::unordered_map< Module_index_type, Edge_index_type > mod_pin_map; 
    
    if( !is_sub_module_empty ){
        // creat the hash table
        // Recursively perform operations on all submodules
        for( auto& subModule : sub_module_list ){ //?
            // mod_pin_map[std::get<0>(subModule)] = &(std::get<1>(subModule));
            Module* sub_data = std::get<2>(subModule);
            if (sub_data != nullptr) {

                std::ostringstream prefix_stream;
                prefix_stream << prefix << "_" << std::get<0>(subModule);
                Name_type prefix_temp = prefix;
                prefix = prefix_stream.str();
                read_graph_data(*sub_data, prefix, std::get<0>(subModule)); // 递归调用
                prefix = prefix_temp;
                std::vector< std::tuple< Edge_index_type,Edge_index_type >> t_IOpin = std::get<1>(subModule);
                for(size_t IOpin_index = 0; IOpin_index < t_IOpin.size(); ++IOpin_index){
                    std::ostringstream keyStream;
                    keyStream << std::get<0>(subModule) << "_" << std::get<1>(t_IOpin[IOpin_index]);
                    Module_index_type key = keyStream.str();
                    // Module_index_type key = std::get<0>(subModule) + "_" ;
                    // key += std::get<1>(t_IOpin[IOpin_index]);
                    
                    Edge_index_type value =  std::get<0>(t_IOpin[IOpin_index]);
                    mod_pin_map[key] = value;
                }
            }
        }
    }

    for( auto& ins_data: gra_data.get_internal_instance() ){
        Name_type ins_name = std::get<1>( ins_data.get_instance_data() );
        Name_type all_name =  mod_index + "_" + ins_name; // ins all name
        bool is_clk = std::get<0>( ins_data.get_instance_data() );
        weight_type ins_weight = ins_data.get_instance_weight();

        std::vector< std::pair< Name_type, weight_type > > adj_v2e;
        std::vector< std::pair< Name_type, weight_type > > adj_e2v;
        std::vector< std::pair< Name_type, weight_type > > pin_info;

        degree_type out_degree = 0;
        degree_type in_degree = 0;
        // bool is_e2v_output;
        for(auto& e_name : ins_data.get_connect_edge() ){
            auto e_data = edge_map.find( e_name ); // find edge
            if ( e_data != edge_map.end() ){
                weight_type e_weight = e_data -> second -> get_weight(); 
                Direction this_ins_to_edge;
                if ( e_data -> second -> get_type() == INPUT ){
                    in_degree ++; // in_pin connect to ins
                    pin_info.push_back({ mod_index + "_" + e_name, e_weight});

                }
                else if( e_data -> second -> get_type() == OUTPUT ){
                    out_degree ++; // out_pin connect to ins
                    pin_info.push_back({ mod_index + "_" + e_name, e_weight});
                }
                else{ // e_data -> second -> get_type() == NORMAL
                    std::vector< Direction >  array_direction = e_data -> second -> get_adj_array_direction();
                    std::vector< Instance_index_type > adj_array = e_data -> second -> get_adjacency_array();
                    size_t v2e = 0;
                    size_t e2v = 0;

                    for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {

                        if( array_direction[ins_index] == v_to_edge ){
                            v2e++;
                            if(adj_array[ins_index] != ins_name){
                                adj_v2e.push_back({
                                    mod_index + "_" + adj_array[ins_index],
                                    e_weight });
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
                                    mod_index + "_" + adj_array[ins_index],
                                    e_weight });                                    
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

        add_adj_plus(all_name, adj_e2v);
        add_adj_minus(all_name, adj_v2e);
        
        if(!is_sub_module_empty){
            for (const auto& pair : this -> pin_temp ){
                // std::cout << pair.first << std::endl;
                std::vector< std::pair< Name_type, weight_type >> pair_temp = pair.second;
                while(!pair_temp.empty()){
                    auto e_data = mod_pin_map.find(pair_temp.front().first);
                    pair_temp.pop_back();
                    if(e_data != mod_pin_map.end() ){
                        auto v2m = edge_map.find(e_data -> second);
                        mod_to_ins[pair.first] = v2m->second->get_name();
                        int b = 1;
                    }
                }
            }
        }

        if( !(pin_info.empty()) ){
            add_ins_pin_temp(all_name, pin_info);
        }

        out_degree += adj_e2v.size();
        in_degree += adj_v2e.size();

        adj_e2v.clear();
        adj_v2e.clear();
        int a = 1;
        Vertex temp_v( all_name, is_clk, ins_weight, out_degree, in_degree);
        // std::cout << all_name << std::endl;
        addVertex(temp_v);

    }
    //  prefix  imhrer  !!!!!!!!!!!
    if(!is_sub_module_empty){
        for (const auto& pair : mod_to_ins) {
            
            std::cout << pair.first<<"---"<<pair.second << std::endl;
            auto edge_data = edge_map.find(pair.second);

            if(edge_data -> second-> get_type() == INPUT){
                // IO
            }
            else if(edge_data -> second-> get_type() == OUTPUT){

                // IO
            }
            else{
                std::vector< Direction >  array_direction = edge_data -> second -> get_adj_array_direction();
                std::vector< Instance_index_type > adj_array = edge_data -> second -> get_adjacency_array();
                size_t v2e = 0;
                size_t e2v = 0;

                for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {
                    std::cout << adj_array[ins_index] << std::endl;
                    if( adj_array[ins_index].back() == '*' ){
                        int c = 1;//?
                    }
                    else{
                        if( array_direction[ins_index] == edge_to_v ){
                            auto v_1 = this -> graph_adjlist_plus.find(pair.first);
                            Name_type e_name = mod_index+"_"+adj_array[ins_index];
                            auto v_2 = this -> graph_adjlist_minus.find(e_name);
                            int a = 1;
                        }
                    }
                }

            }
        }
    }
};