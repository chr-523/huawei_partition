#include "vertex.h"

void Graph::read_graph_data(const Module& gra_data, size_t* level, Name_type& mod_index) {

    (*level)++;
    // 递归地对所有子 Module 执行操作
    std::vector< Sub_Module_type > submodule_temp = gra_data.get_submodule_list();
    // for( auto& subModule : gra_data.get_submodule_list() ){
    for( auto& subModule : submodule_temp ){
        Module* sub_data = std::get<2>(subModule);
        if (sub_data != nullptr) {
            read_graph_data(*sub_data, level, std::get<0>(subModule)); // 递归调用
            //generete the graph//to be done
            (*level)--;
        }
    }

    for( auto& ins: gra_data.get_internal_instance() ){
        degree_type out_degree = 0;
        degree_type in_degree = 0;

        // name,clk,weight,o,i
        Vertex temp_v(  
            mod_index + "_" + std::get<1>( ins.get_instance_data() ), 
            std::get<0>(ins.get_instance_data()),
            ins.get_instance_weight(),
            out_degree,
            in_degree);
        addVertex(temp_v);
        int a=1;
    }
};//11    