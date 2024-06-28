#include "vertex.h"



Graph module_to_graph(Graph_data& gra_data){
    Graph result;
    // Module mod = gra_data.module;
    // std::unordered_map<Name_type, Module *> sm_map =  gra_data.submodule_map;
    int temp_add = 1; // 临时或者啥变量
    result = module_to_graph(gra_data,temp_add);

    return result;
}

Graph module_to_graph(Graph_data& gra_data,int& temp_add){
    Graph result;
    size_t vertex_index = 0;
/***Processing points for the current module***/
    for (auto ins : gra_data.module.get_instance_list()){
        Name_type v_name;
        size_t v_index;
        bool v_is_clk;
        std::vector<std::pair<Name_type,weight_type>> plus_vector;
        std::vector<std::pair<Name_type,weight_type>> minus_vector;


        v_name = std::get<1>(ins.get_instance_data());
        v_is_clk = std::get<0>(ins.get_instance_data());
        v_index = vertex_index;;
        Vertex* v_ = new Vertex (v_name,v_index,v_is_clk);

        result.vertexs[v_name] = v_;
        std::vector<Name_type> e_name_list = ins.get_connect_edge();
        std::vector<Direction> e_direc_list = ins.get_connect_edge_direction();

        for (size_t counter = 0; counter < e_name_list.size() ; ++counter) {
            Name_type e_name = e_name_list[counter];//正在处理的 边
            Direction this_ins_to_edge_direc = e_direc_list[counter];//现在点到处理边的方向
            auto e_ = gra_data.module.E_map.find(e_name);//找到边
            std::vector<std::string> e_i_list = // 这个边连了其他哪些点
                    gra_data.module.get_internal_edge_list()[e_->second].get_adjacency_array();
            std::vector<Direction> e_i_d_list = // 对应其他点的方向
                    gra_data.module.get_internal_edge_list()[e_->second].get_adjacency_array_direction();
            int e_weight = 1;//懒得在前面edge加了
            for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                    // 遍历正在处理的边的所有连接点（要加判断是ins还是mod ----here）
                Name_type connect_v_name = e_i_list[counter_];
                Direction connect_v_diec = e_i_d_list[counter_];
                bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                    // 两个名字不同，且对于边来说方向不同，则需要加入
                if(should_add_v){
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
            int a = 1;
        }

        result.GraphAdjList_Minus[v_name] = plus_vector;
        result.GraphAdjList_Plus[v_name] = minus_vector;

        vertex_index++;
    }
/***Processing points for the current module***/
    int a =1;

    // result = module_to_graph(gra_data,temp_add);//递归

    return result;
}