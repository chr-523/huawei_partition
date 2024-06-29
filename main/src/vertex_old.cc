#include "vertex.h"

bool compareStrings(const std::string& str1, const std::string& str2) {
    // 如果字符串长度小于4，直接进行比较
    if (str1.size() < 4 || str2.size() < 4) {
        return str1 == str2;
    }

    // 比较后缀
    if (str1.substr(str1.size() - 4) != str2.substr(str2.size() - 4)) {
        return false;
    }

    // 后缀相同，再进行整个字符串比较
    return str1 == str2;
}


bool is_in_map(const Name_type& pin_name,
               const std::unordered_map<Name_type, std::pair<Edge_type, Range>>& IO_map) {
    auto it = IO_map.find(pin_name);
    return it != IO_map.end();
}

bool is_in_map(const Name_type& pin_name,
               const std::unordered_map<Name_type, int>& IO_map) {
    auto it = IO_map.find(pin_name);
    return it != IO_map.end();
}


Graph module_to_graph_old(Graph_data& gra_data){
    Graph result;
    result.prefix = "C906";
    result.is_top=true;

    int temp_add = 0; // 临时或者啥变量

    //old
    // for (auto it = gra_data.module.IO_map.begin(); it != gra_data.module.IO_map.end(); ++it) {
    //     const auto& pin_name = it->first;
    //     const auto& type = it->second.first;
    //     temp_add++;
    //     Name_type t_ = std::to_string(temp_add);
    //     std::pair<Name_type, Name_type> value = {pin_name, pin_name};
    // 
    //     result.ins_to_mod_pin_map[t_].emplace_back(value);
    // }
    //
    // for (auto it = result.ins_to_mod_pin_map.begin(); it != result.ins_to_mod_pin_map.end(); ++it) {
    //     const auto& ins_name = it->first;
    //     const auto& pin_pairs = it->second;
    //     for (auto jt = pin_pairs.begin(); jt != pin_pairs.end(); ++jt) {
    //         const auto& mod_name = jt->first;
    //         const auto& pin_name = jt->second;
    //         result.mod_pin_to_ins_map[pin_name].push_back(ins_name);
    //     }
    // }
    result.empty_IO_map = gra_data.module.IO_map;

    module_to_graph_old(
        result,
        gra_data.module,
        gra_data.submodule_map, 
        temp_add
    );



    return result;
}


void module_to_graph_old(
        Graph& result, Module& gra,
        std::unordered_map<Name_type, Module*> sub_map,
        int& temp_add){
    std::string t_p = result.this_submodule_index;
    if(!t_p.empty()){
            result.prefix = result.prefix+"_"+t_p;
    }
    size_t vertex_index = temp_add;


    /*遍历IO边需要在所有点处理完之后处理*/
    for (auto ins : gra.get_instance_list()){
        Name_type v_name = std::get<1>(ins.get_instance_data());
        std::cout<<"IO前遍历的节点名字: "<<v_name<<std::endl;
        /**TEST**/
            if(v_name == "U457"){
                int a = 1;
            };
        /**TEST**/
        bool v_is_clk = std::get<0>(ins.get_instance_data());
        int v_weight = ins.get_instance_weight();
        size_t v_index = vertex_index;
        int in_degree = 0;
        int out_degree = 0;
        std::vector<std::pair<Name_type,weight_type>> plus_vector;
        std::vector<std::pair<Name_type,weight_type>> minus_vector;
        std::vector<std::pair<Name_type,weight_type>> ins_to_plus_module_pin;
        std::vector<std::pair<Name_type,weight_type>> ins_to_minus_module_pin;
        std::vector<std::pair<Name_type,Name_type>> to_pin_vector;//暂存信息

        std::vector<Name_type> e_name_list = ins.get_connect_edge();//连接点的边list
        std::vector<Direction> e_direc_list = ins.get_connect_edge_direction();//连接点的边的方向
        /***遍历边，处理边***/
        for (size_t counter = 0 ; counter < e_name_list.size() ; ++counter) {
        
                // bool is_this_edge_finish;
                //好像后续可以所有边只遍历一次，但是好难改
            /**只处理不是io的情况**/
                // if (!is_io or (vertex_index < 768)){//处理内部线 而不是pin口的结构
            Name_type e_name = e_name_list[counter];//正在处理的 边
            std::cout<<"  正在处理的正常边: "<<e_name<<std::endl;
            auto io_ = gra.IO_map.find(e_name);
            bool is_io = (io_ != gra.IO_map.end());//是否对应正在处理模块的IO_pin
            if (!is_io){//处理内部线 而不是pin口的结构
                auto e_ = gra.E_map.find(e_name);//找到边的index
                            Direction this_ins_to_edge_direc = e_direc_list[counter];//现在点到处理边的方向
                    // 这个边连了其他哪些点->根据index找edge的连哪些ins的属性
                std::vector<std::string> e_i_list = 
                        gra.get_internal_edge_list()[e_->second].get_adjacency_array();
                    // 对应其他点的方向
                std::vector<Direction> e_i_d_list = 
                        gra.get_internal_edge_list()[e_->second].get_adjacency_array_direction();
                int e_weight = 1;//默认权重
                /* 遍历正在处理的边的所有连接点，判断ins mod分别处理*/
                for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                    Name_type connect_v_name = e_i_list[counter_];
                    Direction connect_v_diec = e_i_d_list[counter_];
                    bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                    
                    std::cout<<"    正在处理连边的点: "<<connect_v_name<<std::endl;
                    /**是否需要加入节点**/
                    if(should_add_v){                    
                        // 两个点 名字不同，且对于边来说方向不同，则需要加入
                        bool is_this_v_mod =(connect_v_name.back()=='*');//判断是否是submod
                        /**要加入的节点是submodule**/
                        if(is_this_v_mod){
                            // 根据名字找到mod信息
                            auto mod = gra.Submodule_map.find(connect_v_name);
                            size_t sub_index = mod -> second; //这是对应submodule的index
                            // gra.get_submodule_list()[sub_index].second//这个是module指针 没用上
                            std::vector<Name_type> sp_list = gra.get_submodule_pin_edge_list()[sub_index];
                            // 上面这个是正在处理的submodule的pin和对应连线的list
                            /**找到正在连接现在在处理的边对应的pin**/
                            for( size_t sub_pin_index = 2;sub_pin_index < sp_list.size();sub_pin_index += 2){
                                if(sp_list[sub_pin_index]==e_name){
                                    to_pin_vector.push_back({connect_v_name,sp_list[sub_pin_index-1]});
                                    //key是正在处理的点name，生成点的时候再压入graph
                                    //value 为 <点连接的module的name, 点连接的module的pin name>
                                    //也就是ins to mod pin map
                                }
                            }
                            /**找到正在连接现在在处理的边对应的pin**/
                                // 点的邻接链就不存module了，另用ins_to_mod_pin来存
                                // 后续submodule递归 之前找到pinname到name的映射，也就是mod pin to ins map
                                // 递归submodule的时候 module内pin name 找外部 mod_to_pin_ins_map 来找到所连接的ins
                                // 再 将内部信息 push进点的邻接 
                        }
                        /**要加入的节点是submodule**/
                        /**要加入的节点是普通节点**/
                        else{
                            if(this_ins_to_edge_direc == e2v){
                            //这个边指向，从另一点指过来 -> 加入 minus
                                minus_vector.push_back({result.prefix+"_"+connect_v_name,e_weight});
                                in_degree++;
                            }
                            else{//反之加入PLUS
                                plus_vector.push_back({result.prefix+"_"+connect_v_name,e_weight});
                                out_degree++;
                            }
                            // to_pin_vector.push_back("*");
                        }
                        /**要加入的节点是普通节点**/                        
                    }
                    /**是否需要加入节点**/
                    else{ //这个是不需要加入节点的情况 同向了
                        //名字相同或者方向相同 should not add
                        //do nothing
                    }
                
                }
                /* 遍历正在处理的边的所有连接点，判断ins mod分别处理*/
                    // if(e_name == "pcgen_btb_ifpc_32"){
                    //     int a = 1;
                    // }
                /*正在遍历的边有assign,则遍历这个assign list，处理方法相同*/
                if (!gra.get_internal_edge_list()[e_->second].get_assign_list().empty()){
                    /* 遍历正在处理的ASSIGN的边的所有连接点，判断ins mod分别处理*/
                    for(auto assign_e_name: gra.get_internal_edge_list()[e_->second].get_assign_list()){
                        /*每个assign过来的边和一般边相同处理*/
                        e_i_list.clear();
                        e_i_list = 
                                gra.get_internal_edge_list()[e_->second].get_adjacency_array();
                            // 对应其他点的方向
                        e_i_d_list.clear();
                        e_i_d_list = 
                                gra.get_internal_edge_list()[e_->second].get_adjacency_array_direction();

                        for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                            Name_type connect_v_name = e_i_list[counter_];
                            Direction connect_v_diec = e_i_d_list[counter_];
                            bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                            /**是否需要加入节点**/
                            if(should_add_v){                    
                                // 两个点 名字不同，且对于边来说方向不同，则需要加入
                                bool is_this_v_mod =(connect_v_name.back()=='*');//判断是否是submod
                                /**要加入的节点是submodule**/
                                if(is_this_v_mod){
                                    // 根据名字找到mod信息
                                    auto mod = gra.Submodule_map.find(connect_v_name);
                                    size_t sub_index = mod -> second; //这是对应submodule的index
                                    // gra.get_submodule_list()[sub_index].second//这个是module指针 没用上
                                    std::vector<Name_type> sp_list = gra.get_submodule_pin_edge_list()[sub_index];
                                    // 上面这个是正在处理的submodule的pin和对应连线的list
                                    /**找到正在连接现在在处理的边对应的pin**/
                                    for( size_t sub_pin_index = 2;sub_pin_index < sp_list.size();sub_pin_index += 2){
                                        if(sp_list[sub_pin_index]==e_name){
                                            to_pin_vector.push_back({connect_v_name,sp_list[sub_pin_index-1]});
                                            //key是正在处理的点name，生成点的时候再压入graph
                                            //value 为 <点连接的module的name, 点连接的module的pin name>
                                            //也就是ins to mod pin map
                                        }
                                    }
                                    /**找到正在连接现在在处理的边对应的pin**/
                                        // 点的邻接链就不存module了，另用ins_to_mod_pin来存
                                        // 后续submodule递归 之前找到pinname到name的映射，也就是mod pin to ins map
                                        // 递归submodule的时候 module内pin name 找外部 mod_to_pin_ins_map 来找到所连接的ins
                                        // 再 将内部信息 push进点的邻接 
                                }
                                /**要加入的节点是submodule**/
                                /**要加入的节点是普通节点**/
                                else{
                                    if(this_ins_to_edge_direc == e2v){
                                    //这个边指向，从另一点指过来 -> 加入 minus
                                        minus_vector.push_back({result.prefix+"_"+connect_v_name,e_weight});
                                        in_degree++;
                                    }
                                    else{//反之加入PLUS
                                        plus_vector.push_back({result.prefix+"_"+connect_v_name,e_weight});
                                        out_degree++;
                                    }
                                    // to_pin_vector.push_back("*");
                                }
                                /**要加入的节点是普通节点**/                        
                            }
                            /**是否需要加入节点**/
                            else{ //这个是不需要加入节点的情况 同向了
                                //名字相同或者方向相同 should not add
                                //do nothing
                            }
                        
                        }
                        /*每个assign过来的边和一般边相同处理*/
                    }/* 遍历正在处理的边的所有连接点，判断ins mod分别处理*/
                }
                /*正在遍历的边有assign,则遍历这个assign list，处理方法相同*/
            }
            /**只处理不是io的情况**/
        }
        /***遍历边，处理边***/
        /**根据点信息生成点，压入graph**/
        v_name = result.prefix + "_" + v_name;
        Vertex* v_ = new Vertex (v_name, v_is_clk, v_weight,v_index, out_degree, in_degree);
        result.vertexs[v_name] = v_;
        result.GraphAdjList_Minus[v_name] = minus_vector;
        result.GraphAdjList_Plus[v_name] = plus_vector;
        if(!to_pin_vector.empty()){
            result.ins_to_mod_pin_map[v_name] = to_pin_vector;
        }
        vertex_index++;temp_add++;
        /**根据点信息生成点，压入graph**/

            // if(out_degree!=plus_vector.size()){
            //     int a =1;
            // }           /*test*/
            // else if(in_degree!=minus_vector.size()){
            //     int a =1;
                
            // }

    }

        if(1){
            int a = 1;
        }
    /*遍历IO边需要在所有点处理完之后处理*/
    /*遍历IO边*/
    for (auto IO_ = gra.IO_map.begin(); IO_ != gra.IO_map.end(); ++IO_){
    
        Name_type IO_name = IO_ -> first;
        Edge_type IO_type = IO_ -> second.first; 
        std::cout<<"正在处理的IO边: "<<IO_name<< " -> "<<IO_type<<std::endl;

            if(1){
                int a = 1;
            }
        
        auto IO_edge_index = gra.E_map.find(IO_name);// 找到IO边index
        if(IO_edge_index == gra.E_map.end()){
            std::cerr<<"Wrong"<<std::endl;
        }
        else{
            std::vector<Name_type> e_i_list = 
                    gra.get_internal_edge_list()[IO_edge_index -> second].get_adjacency_array();
            std::vector<Direction> e_i_d_list = 
                    gra.get_internal_edge_list()[IO_edge_index -> second].get_adjacency_array_direction();
        
            /*遍历IO边连接的点***/
            size_t size = e_i_d_list.size();
            Name_type only_to_IO_name;
            std::vector< Name_type > other_to_IO_name_list;
            for (size_t ins_index = 0; ins_index < size ; ++ins_index) {    
                Name_type v_name = e_i_list[ins_index];
                Direction v_direc = e_i_d_list[ins_index];//v2e is 1 <=> e2v is 2
                bool this_is_mod = (v_name.back() == '*');
                /**对于module处理**/
                if(this_is_mod){
                    std::cout<<"  遍历IO的mod名: "<< v_name << "  (1 is v2e : 2 is e2v): " << v_direc << std::endl;

                    /**内部处理**/

                    /**内部处理**/                    
                    
                    /**外部处理**/

                    /**外部处理**/

                }
                /**对于module处理**/
                /**对于ins处理**/
                else{ //this is ins , not module
                    std::cout<<"  遍历IO的ins名: "<< v_name << "  (1 is v2e : 2 is e2v): " << v_direc << std::endl;
                    /**内部处理**/
                    if(IO_type == OUTPUT){
                        //PIN OUTPUT
                        
                    }
                    /**内部处理**/

                    /**外部处理**/

                    /**外部处理**/
                }
                /**对于ins处理**/
            }
            /*遍历IO边连接的点***/




        }
    }
    /*遍历IO边*/


        std::cout<<"\t\t已处理点数量: "<<temp_add+1<<std::endl;



    
    /**生成点,压入graph**/
/***Processing points for the current module***/
/**处理前遍历map 生成/更新ins to mod pin map**/
    result.mod_pin_to_ins_map.clear();
    for (auto it = result.ins_to_mod_pin_map.begin(); it != result.ins_to_mod_pin_map.end(); ++it) {
        const auto& ins_name = it->first;
        const auto& pin_pairs = it->second;
        for (auto jt = pin_pairs.begin(); jt != pin_pairs.end(); ++jt) {
            const auto& mod_name = jt->first;
            const auto& pin_name = jt->second;
            result.mod_pin_to_ins_map[pin_name].push_back(ins_name);
        }
    }
/**处理前遍历map 生成/更新ins to mod pin map**/
    auto it =result.mod_pin_to_ins_map.find("SUM");
/***有子模块，进入内部递归处理***/
    size_t submodule_number = gra.Submodule_map.size();
    result.is_top = false;
    if( submodule_number != 0 ){
        for(size_t submodule_counter = 0 ; submodule_counter < submodule_number ; submodule_counter++ ){
            Module submodule= *std::get<1>(gra.get_submodule_list()[submodule_counter]);
            result.this_submodule_index= std::get<0>(gra.get_submodule_list()[submodule_counter]);

     
            module_to_graph_old(result, submodule, sub_map, temp_add);
            
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
    int n = t_p.size()+1;
    if (n > result.prefix.size()) {
        n = result.prefix.size();
    }
    
    result.prefix.erase(result.prefix.size() - n);
}