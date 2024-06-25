#include "vertex.h"

void Graph::read_graph_data(const Module& gra_data,  Name_type& prefix, Name_type& mod_index) {

    edge_map_type edge_map;
    std::unordered_map<std::string,Edge*> edge_map_small;
    std::vector< Edge > temp_el_small;//<-ctrl F this
        // creat the hash table
    std::vector< Edge > temp_el = gra_data.get_internal_edge_list();

    for (auto& e : temp_el){
        edge_map[e.get_name()] = &e; 
    };

    std::vector< Sub_Module_type > sub_module_list = gra_data.get_submodule_list() ;
    bool is_sub_module_empty = sub_module_list.empty();
    std::unordered_map< Name_type, std::pair<Name_type, Edge_type> > mod_to_ins;
    std::unordered_map< Module_index_type, std::pair< Edge_index_type, Edge_type >> mod_pin_map; 
    std::unordered_map< Edge_index_type, Edge > pin_to_edge_map;
    std::unordered_map< Edge_index_type, Module_index_type> pin_mod_map; 
        // creat the hash table

    if( !is_sub_module_empty ){
        // creat the hash table
        // Recursively perform operations on all submodules
        for( auto& subModule : sub_module_list ){ //?
            // mod_pin_map[std::get<0>(subModule)] = &(std::get<1>(subModule));


            Module* sub_data = std::get<2>(subModule);            
            if (sub_data != nullptr) {

                temp_el_small = sub_data -> get_internal_edge_list();
                // std::vector<Edge> temp_el_small = sub_data -> get_internal_edge_list();
                Name_type submodule_name = std::get<0>(subModule);
                for (auto &e : temp_el_small){
                    edge_map_small[submodule_name+"_"+e.get_name()] = &e; 
                };

                std::ostringstream prefix_stream;
                prefix_stream << prefix << "_" << submodule_name;
                Name_type prefix_temp = prefix;
                prefix = prefix_stream.str();
                read_graph_data(*sub_data, prefix, submodule_name); // 递归调用
                prefix = prefix_temp;
                std::vector< std::tuple< Edge_index_type,Edge_index_type >> t_IOpin = std::get<1>(subModule);

                for(size_t IOpin_index = 0; IOpin_index < t_IOpin.size(); ++IOpin_index){

                    std::ostringstream keyStream;
                    Module_index_type p_ = std::get<1>(t_IOpin[IOpin_index]);
                    keyStream << std::get<0>(subModule) << "_" << p_;
                    Module_index_type key = keyStream.str();
                    // Module_index_type key = std::get<0>(subModule) + "_" ;
                    // key += std::get<1>(t_IOpin[IOpin_index]);
                    
                    Edge_index_type value =  std::get<0>(t_IOpin[IOpin_index]);
                    
                    // std::unordered_map< Name_type, Edge_type > IO_type_map;
                    auto IO_ = sub_data -> get_IO_type_map().find(p_);
                    Edge_type t_ = IO_ -> second;
                    mod_pin_map[key] = {value,t_};
                    // mod_pin_map[key] = {value,t_,name};
                    
                    if(t_ == INPUT){ 
                        Edge_index_type k_ = value + "_i";
                        pin_mod_map[k_] = key;
                    }
                    else{
                        Edge_index_type k_ = value + "_o";
                        pin_mod_map[k_] = key;
                    }
                    int a =1;
                }

            }
        }
    }

    for( auto& ins_data: gra_data.get_internal_instance() ){
        Name_type ins_name = std::get<1>( ins_data.get_instance_data() );
        Name_type all_name =  prefix + "_" + ins_name; // ins all name
        bool is_clk = std::get<0>( ins_data.get_instance_data() );
        weight_type ins_weight = ins_data.get_instance_weight();

        std::vector< std::pair< Name_type, weight_type > > adj_v2e;
        std::vector< std::pair< Name_type, weight_type > > adj_e2v;
        std::vector< std::tuple< Name_type,Edge_type, weight_type > > pin_info;
        degree_type v2e = 0;
        degree_type e2v = 0;
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
                    pin_info.push_back({ mod_index + "_" + e_name, INPUT, e_weight});

                }
                else if( e_data -> second -> get_type() == OUTPUT ){
                    out_degree ++; // out_pin connect to ins
                    pin_info.push_back({ mod_index + "_" + e_name, OUTPUT, e_weight});
                }
                else{ // e_data -> second -> get_type() == NORMAL
                    std::vector< Direction >  array_direction = e_data -> second -> get_adj_array_direction();
                    std::vector< Instance_index_type > adj_array = e_data -> second -> get_adjacency_array();


                    for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {

                        if( array_direction[ins_index] == v_to_edge ){
                            Name_type t_ins_name = adj_array[ins_index];
                            if( t_ins_name != ins_name ){
                                v2e++;
                                if(t_ins_name.back() != '*'){
                                    adj_v2e.push_back({
                                        prefix + "_" + adj_array[ins_index],
                                        e_weight });
                                    // out_degree++;
                                }
                            }
                            else{
                                this_ins_to_edge = v_to_edge;
                            }
                        }
                        else{ //  array_direction[ins_index] == edge_to_v
                            Name_type t_ins_name = adj_array[ins_index];
                            if(t_ins_name != ins_name){
                                e2v++;
                                if(t_ins_name.back() != '*'){
                                    adj_e2v.push_back({
                                        prefix + "_" + adj_array[ins_index],
                                        e_weight });                                    
                                        // in_degree++;
                                }
                            }
                            else{
                                this_ins_to_edge = edge_to_v;
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
                std::vector< std::tuple< Name_type, Edge_type, weight_type >> pair_temp = pair.second;
                while(!pair_temp.empty()){
                    auto e_data = mod_pin_map.find(std::get<0>(pair_temp.front()));
                    pair_temp.pop_back();
                    if(e_data != mod_pin_map.end() ){
                        auto v2m = edge_map.find(e_data -> second.first);
                        mod_to_ins[pair.first] = {v2m->second->get_name(),std::get<1>(pair_temp.front())};
                        int a = 1;
                    }
                }
            }
        }

        if( !(pin_info.empty()) ){
            add_ins_pin_temp(all_name, pin_info);
        }

        out_degree += e2v;
        in_degree += v2e;

        adj_e2v.clear();
        adj_v2e.clear();
        int a = 1;
        Vertex temp_v( all_name, is_clk, ins_weight, out_degree, in_degree);
        // std::cout << all_name << std::endl;
        addVertex(temp_v);

    }

    if(!is_sub_module_empty){
        // std::vector<Name_type> mod_to_mod_ins;
        // std::vector<Name_type> mod_to_mod_pin;
        for (const auto& pair : mod_to_ins) {
            Name_type v_i_name = pair.first;

            std::cout << v_i_name<<" - - - "<<std::get<0>(pair.second) << std::endl;
            auto edge_data = edge_map.find(std::get<0>(pair.second));
            Edge_type edge_dire = std::get<1>(pair.second);
            // if( edge_data -> second-> get_type() == INPUT){
            if( edge_data -> second-> get_type() != NORMAL){


                // mod_to_mod_pin.push_back();
                // mod_to_mod_ins.push_back(v_i_name);
                int a = 1;
            }
            // else if( edge_data -> second-> get_type() == OUTPUT){
            // }
            else{ // edge_data -> second-> get_type() == NROMAL
                Name_type edge_name = edge_data -> second -> get_name();
                std::vector< Direction >  array_direction = edge_data -> second -> get_adj_array_direction();
                std::vector< Instance_index_type > adj_array = edge_data -> second -> get_adjacency_array();


                for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {
                    Name_type now_module_name = adj_array[ins_index];
                    std::cout << now_module_name << std::endl;
                    if( adj_array[ins_index].back() == '*' ){
                        auto e_ = pin_mod_map.find(edge_name);
                        //找1*对应sum1的ins内的边
                        if( array_direction[ins_index] == v_to_edge ){
                            auto i_1 =pin_mod_map.find(edge_name+"_o");
                            auto i_2 = edge_map_small.find(i_1 -> second);

                            int a = 1;//吧现在ad1里面的加入a2u1

                        }
                    }
                    else{                            
                        Name_type v_o_name = prefix + "_" + adj_array[ins_index];
                        auto v_1 = this -> graph_adjlist_plus.find(v_i_name);
                        // 将u1加入这里
                        auto v_2 = this -> graph_adjlist_minus.find(v_o_name);
                        if( array_direction[ins_index] == edge_to_v ){
                            v_2 -> second.emplace_back(v_i_name, default_instance_weight);
                            v_1 -> second.emplace_back(v_o_name, default_instance_weight);
                        }
                        else{
                            v_2 -> second.emplace_back(v_o_name, default_instance_weight);
                            v_1 -> second.emplace_back(v_i_name, default_instance_weight);
                        }
                    }
                }

            }
        }
    }

};