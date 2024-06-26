#include "vertex.h"







void Graph::read_graph_data(
        const Module& gra_data,  
        Name_type& prefix, 
        Name_type& mod_index) {
    rec_level++;
    edge_map_type edge_map;
        // creat the hash table
    std::vector< Edge > temp_el = gra_data.get_internal_edge_list();

    for (auto& e : temp_el){
        edge_map[e.get_name()] = &e; 
    };

    std::vector< Sub_Module_type > sub_module_list = gra_data.get_submodule_list() ;
    bool is_sub_module_empty = sub_module_list.empty();

        // creat the hash table

    if( !is_sub_module_empty ){
        // creat the hash table
        // Recursively perform operations on all submodules
        for( auto& subModule : sub_module_list ){ //?
            // pin_outedge_temp[std::get<0>(subModule)] = &(std::get<1>(subModule));


            Module* sub_data = std::get<2>(subModule);            
            if (sub_data != nullptr) {
                Name_type submodule_name = std::get<0>(subModule);

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
                    pin_outedge_temp[key] = {value,t_};
                    
                    if(t_ == INPUT){ 
                        Edge_index_type k_ = value + "_i";
                        outedge_pin_temp[k_] = key;
                    }
                    else{
                        Edge_index_type k_ = value + "_o";
                        outedge_pin_temp[k_] = key;
                    }

                }
            }
            rec_level--;
        }
    }

    if( !is_sub_module_empty){
        for (const auto& i_p : this -> ins_pin_edge_temp ){
            // std::cout << pair.first << std::endl;
            Name_type this_ins_name = i_p.first;
            std::vector< std::tuple< Name_type, Edge >> i_p_temp = i_p.second;
            while(!i_p_temp.empty()){
                Name_type pin_name = std::get<0>(i_p_temp.back());
                auto out_edge = pin_outedge_temp[pin_name];
                Edge_type edge_dire_for_ins = std::get<1>(i_p_temp.back()).get_type();
                // for ins is output-> for edge is v2e
                Direction this_ins_dire = edge_to_v;
                //e2v是从边input进去module，搜索_o
                //v2e反过来,搜索_i

                if(edge_dire_for_ins == OUTPUT){ 
                    this_ins_dire = v_to_edge; 
                }
                // else{ this_ins_dire == edge_to_v; }
                
                i_p_temp.pop_back();                
                Name_type o_e_name = out_edge.first;
                auto oe = edge_map.find(o_e_name);

                bool is_IO = false;
                if( oe->second->get_type() != NORMAL ){ is_IO = true;}
                // else{ is_IO = false;}
                weight_type oe_weight = oe -> second -> get_weight();
                std::vector< Direction >  adj_direction = oe -> second -> get_adj_array_direction();
                std::vector< Instance_index_type > adj_array = oe -> second -> get_adjacency_array();
                
                if(!is_IO){ // not io -> mod-mod/mod-ins
                    for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {
                        int a = 1;
                        
                        if( adj_array[ins_index].back() != '*'){ 
                            // mod to ins -> all dire should be add
                            Name_type connect_ins_name = prefix+"_"+adj_array[ins_index];
                            if(adj_direction[ins_index] == edge_to_v){
                                add_adj_minus(connect_ins_name,{this_ins_name, oe_weight});
                                add_adj_plus(this_ins_name,{connect_ins_name, oe_weight});
                            }
                            else{
                                add_adj_plus(connect_ins_name,{this_ins_name, oe_weight});
                                add_adj_minus(this_ins_name,{connect_ins_name, oe_weight});
                            }
                        }
                        else{ //mod to mod <- adj_array[ins_index].back() != '*'
                            // only dire different to adj_dire[index] should be add
                            if(adj_direction[ins_index] != this_ins_dire){
                                // e2v是从边input进去module，搜索_o
                                   Name_type subfix = "_o";
                                if (this_ins_dire == v_to_edge){
                                    // for another module io exchange
                                    // v2e反过来,搜索_i
                                   subfix = "_i";
                                }
                                // else{// this_ins_dire == edge_to_v
                                // e2v是从边input进去module，搜索_o
                                //    Name_type subfix = "_i";
                                // }
                                Name_type check_pin_name = o_e_name + subfix;
                                Name_type other_pin_name = outedge_pin_temp[check_pin_name];
                                Edge should_c_edge = std::get<1>(pin_ins_edge_temp[other_pin_name][0]);
                                weight_type sce_weight = should_c_edge.get_weight();
                                std::vector< Instance_index_type > c_ins =should_c_edge.get_adjacency_array();
                                std::vector< Direction > c_ins_d =should_c_edge.get_adj_array_direction();

                                while(
                                    (!c_ins.empty()) && 
                                    (!c_ins_d.empty())){
                                    // add_adj_minus(); 
                                    Name_type t = adj_array[ins_index];
                                    t.pop_back();
                                    Name_type connect_ins_name = prefix + "_" + t + "_" +c_ins.back();                      
                                    int b = 4;
                                    if( c_ins_d.back() == edge_to_v){
                                    // if(1){
                                        std::cout<<"begin:"<<this_ins_name<<std::endl;
                                        std::cout<<"  ->  "<<pin_name<<std::endl;
                                        std::cout<<"  ->  "<<check_pin_name<<std::endl;
                                        std::cout<<"  ->  "<<other_pin_name<<std::endl;
                                        std::cout<<"  ->  "<<connect_ins_name<<std::endl<<std::endl;

                                        add_adj_minus(connect_ins_name,{this_ins_name, oe_weight});
                                        std::cout<<connect_ins_name<<" minus= "<<this_ins_name<<std::endl;
                                        add_adj_plus(this_ins_name,{connect_ins_name, oe_weight});
                                        std::cout<<this_ins_name<<" plus = "<<connect_ins_name<<std::endl<<std::endl;
                                    }
                                    else{
                                        std::cout<<"begin:"<<this_ins_name<<std::endl;
                                        std::cout<<"  ->  "<<pin_name<<std::endl;
                                        std::cout<<"  ->  "<<check_pin_name<<std::endl;
                                        std::cout<<"  ->  "<<other_pin_name<<std::endl;
                                        std::cout<<"  ->  "<<connect_ins_name<<std::endl<<std::endl;

                                        add_adj_plus(connect_ins_name,{this_ins_name, oe_weight});
                                        std::cout<<connect_ins_name<<" plus = "<<this_ins_name<<std::endl;
                                        add_adj_minus(this_ins_name,{connect_ins_name, oe_weight});
                                        std::cout<<this_ins_name<<" minus= "<<connect_ins_name<<std::endl<<std::endl;
                                    }
                                    c_ins.pop_back();
                                    c_ins_d.pop_back();
                                }
                            }
                            else{ // do nothing
                                int b = 1;
                            }
                            int a =1;
                        }                    
                    }
                }
                else{ // mod to io_pin 
                    int a = 1;
                    // clear后加回temp hrere
                    // std::cout<<this_ins_name<<std::endl;

                }
                
            }
        }
        int a = 1;//hrer
        clear_temp_1();
        clear_temp_2();
        int b = 1;
    }
    

    for( auto& ins_data: gra_data.get_internal_instance() ){
        Name_type ins_name = std::get<1>( ins_data.get_instance_data() );
        Name_type all_name =  prefix + "_" + ins_name; // ins all name
        bool is_clk = std::get<0>( ins_data.get_instance_data() );
        weight_type ins_weight = ins_data.get_instance_weight();

        std::vector< std::pair< Name_type, weight_type > > adj_v2e;
        std::vector< std::pair< Name_type, weight_type > > adj_e2v;
        std::vector< std::tuple< Name_type, Edge > > ins_edge_info;
        std::vector< std::tuple< Name_type, Edge > > edge_pin_info;

        degree_type v2e = 0;
        degree_type e2v = 0;
        degree_type out_degree = 0;
        degree_type in_degree = 0;
        // bool is_e2v_output;
        for(auto& e_name : ins_data.get_connect_edge() ){
            Name_type all_e_name = mod_index + "_" + e_name;
            auto e_data = edge_map.find( e_name ); // find edge
            if ( e_data != edge_map.end() ){
                weight_type e_weight = e_data -> second -> get_weight(); 
                Direction this_ins_to_edge;
                if ( e_data -> second -> get_type() == INPUT ){
                    in_degree ++; // in_pin connect to ins
                    edge_pin_info.push_back({all_e_name, *e_data->second});
                    ins_edge_info.push_back({all_name, *e_data->second});
                }
                else if( e_data -> second -> get_type() == OUTPUT ){
                    out_degree ++; // out_pin connect to ins
                    edge_pin_info.push_back({all_e_name, *e_data->second});
                    ins_edge_info.push_back({all_name, *e_data->second});
                }
                else{ // e_data -> second -> get_type() == NORMAL
                    std::vector< Direction >  adj_direction = e_data -> second -> get_adj_array_direction();
                    std::vector< Instance_index_type > adj_array = e_data -> second -> get_adjacency_array();


                    for(size_t ins_index = 0; ins_index < adj_array.size(); ++ins_index) {

                        if( adj_direction[ins_index] == v_to_edge ){
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
                        else{ //  adj_direction[ins_index] == edge_to_v
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


            if( !(ins_edge_info.empty()) ){
                add_pin_ins_edge_temp(mod_index + "_" + e_name, ins_edge_info);
                ins_edge_info.clear();
            }
        }

        add_adj_plus(all_name, adj_e2v);
        add_adj_minus(all_name, adj_v2e);
        int a = 1;


        if( !(edge_pin_info.empty()) ){
            add_ins_pin_edge_temp(all_name, edge_pin_info);
        }        


        out_degree += e2v;
        in_degree += v2e;

        adj_e2v.clear();
        adj_v2e.clear();

        Vertex temp_v( all_name, is_clk, ins_weight, out_degree, in_degree);
        // std::cout << all_name << std::endl;
        addVertex(temp_v);

    }

    int a = 1;
};