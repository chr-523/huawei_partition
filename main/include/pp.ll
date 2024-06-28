#include "vertex.h"



Graph module_to_graph(Graph_data& gra_data){
    Graph result;
    int temp_add = 1; // 临时或者啥变量
    module_to_graph(
        result,
        gra_data.module,
        gra_data.submodule_map, 
        temp_add
    );

    return result;
}

void module_to_graph(
        Graph& result, Module& gra,
        std::unordered_map<Name_type, Module*> sub_map,
        int& temp_add){

    size_t vertex_index = 0;

/***Processing points in the current module***/
    for (auto ins : gra.get_instance_list()){

        Name_type v_name = std::get<1>(ins.get_instance_data());
        bool v_is_clk = std::get<0>(ins.get_instance_data());
        int v_weight = ins.get_instance_weight();
        size_t v_index = vertex_index;;

        int in_degree = 0;
        int out_degree = 0;
        std::vector<std::pair<Name_type,weight_type>> plus_vector;
        std::vector<std::pair<Name_type,weight_type>> minus_vector;
        std::vector<Name_type> to_pin_vector;

        std::vector<Name_type> e_name_list = ins.get_connect_edge();
        std::vector<Direction> e_direc_list = ins.get_connect_edge_direction();
    /***遍历边，处理边***/
        for (size_t counter = 0 ; counter < e_name_list.size() ; ++counter) {
            Name_type e_name = e_name_list[counter];//正在处理的 边
            Direction this_ins_to_edge_direc = e_direc_list[counter];//现在点到处理边的方向
            auto e_ = gra.E_map.find(e_name);//找到边
            std::vector<std::string> e_i_list = // 这个边连了其他哪些点
                    gra.get_internal_edge_list()[e_->second].get_adjacency_array();
            std::vector<Direction> e_i_d_list = // 对应其他点的方向
                    gra.get_internal_edge_list()[e_->second].get_adjacency_array_direction();
            int e_weight = 1;//懒得在前面edge加了
            for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                    // 遍历正在处理的边的所有连接点（要加判断是ins还是mod ----here）
                Name_type connect_v_name = e_i_list[counter_];
                Direction connect_v_diec = e_i_d_list[counter_];
                bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                    // 两个名字不同，且对于边来说方向不同，则需要加入
                if(should_add_v){
                    if(connect_v_name.back()=='*'){//lian mod 
                        auto mod = gra.Submodule_map.find(connect_v_name);
                        size_t sub_index = mod -> second;
                        // gra.get_submodule_list()[sub_index].second//这个是module指针
                        std::vector<Name_type> sp_list = gra.get_submodule_pin_edge_list()[sub_index];
                        for( size_t sub_pin_index = 2;sub_pin_index < sp_list.size();sub_pin_index += 2){
                            if(sp_list[sub_pin_index]==e_name){
                                to_pin_vector.push_back(sp_list[sub_pin_index-1]);
                                // result.mod_pin_to_ins_map[v_name] = v_name
                                /**/
                                if (mod_pin_to_ins_map.find(sp_list[sub_pin_index-1]) == mod_pin_to_ins_map.end()) {
                                    mod_pin_to_ins_map[mod_pin] = std::vector<Name_type>();
                                }
                                // 向vector中添加元素
                                mod_pin_to_ins_map[mod_pin].push_back(name_instance);
                                /**/
                            }
                        }
                        
                    }
                    else{
                        to_pin_vector.push_back("*");
                    }

                    if(this_ins_to_edge_direc == e2v){
                        //这个边指向，从另一点指过来 -> 加入 minus
                        minus_vector.push_back({connect_v_name,e_weight});
                    }
                    else{//反之加入PLUS
                        plus_vector.push_back({connect_v_name,e_weight});
                    }
                }
                else{ // should not add
                    //do nothing
                }
            }
        
            /*和非assign边处理相同，多遍历一个边list而已*/
            if (!gra.get_internal_edge_list()[e_->second].get_assign_list().empty()){
                std::vector<Name_type> assign_list = gra.get_internal_edge_list()[e_->second].get_assign_list();
                for (auto a_e_name :assign_list){
                    Direction this_ins_to_edge_direc = e_direc_list[counter];//现在点到处理边的方向
                    auto e_ = gra.E_map.find(a_e_name);//找到边
                    std::vector<std::string> e_i_list = // 这个边连了其他哪些点
                            gra.get_internal_edge_list()[e_->second].get_adjacency_array();
                    std::vector<Direction> e_i_d_list = // 对应其他点的方向
                            gra.get_internal_edge_list()[e_->second].get_adjacency_array_direction();
                    int e_weight = 1;//懒得在前面edge加了
                    for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                    // 遍历正在处理的边的所有连接点（要加判断是ins还是mod ----here）
                        Name_type connect_v_name = e_i_list[counter_];
                        Direction connect_v_diec = e_i_d_list[counter_];
                        bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                    // 两个名字不同，且对于边来说方向不同，则需要加入
                        if(should_add_v){
                            if(connect_v_name.back()=='*'){//lian mod 
                               auto mod = gra.Submodule_map.find(connect_v_name);
                               size_t sub_index = mod -> second;
                                // gra.get_submodule_list()[sub_index].second//这个是module指针
                                std::vector<Name_type> sp_list = gra.get_submodule_pin_edge_list()[sub_index];
                                for( size_t sub_pin_index = 2;sub_pin_index < sp_list.size();sub_pin_index += 2){
                                    if(sp_list[sub_pin_index]==e_name){
                                        to_pin_vector.push_back(sp_list[sub_pin_index-1]);
                                    }
                                }
                        
                            }
                            else{
                                to_pin_vector.push_back("*");
                            }

                            if(this_ins_to_edge_direc == e2v){
                                //这个边指向，从另一点指过来 -> 加入 minus
                                minus_vector.push_back({connect_v_name,e_weight});
                            }
                            else{//反之加入PLUS
                                plus_vector.push_back({connect_v_name,e_weight});
                            }
                        }
                        else{ // should not add
                            //do nothing
                        }
                    }
                }
            }
            /*和非assign边处理相同，多遍历一个边list而已*/
        }
    /***遍历边，处理边***/
        Vertex* v_ = new Vertex (v_name, v_is_clk, v_weight,v_index, out_degree, in_degree);
        result.vertexs[v_name] = v_;
        vertex_index++;
        result.GraphAdjList_Minus[v_name] = minus_vector;
        result.GraphAdjList_Plus[v_name] = plus_vector;
        result.ins_to_mod_pin_map[v_name] = to_pin_vector;
        vertex_index++;

    }
/***Processing points for the current module***/

/***有子模块，进入内部递归处理***/
    size_t submodule_number = gra.Submodule_map.empty();
    if( submodule_number != 0 ){
        for(size_t submodule_counter = 0 ; submodule_counter < submodule_number ; submodule_counter++ ){
            // module_to_graph(result, gra, sub_map, temp_add);
            int a  = 1;
        }
    }
/***有子模块，进入内部递归处理***/

/**TEST**/
    if(0){
        Name_type t_name = "U119";
        auto t1 = result.vertexs.find(t_name);
        auto t2 = result.GraphAdjList_Minus.find(t_name);
        auto t3 = result.GraphAdjList_Plus.find(t_name);
        int a = 1;
    }
/**TEST**/

}