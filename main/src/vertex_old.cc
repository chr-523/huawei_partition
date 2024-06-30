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

    // for (auto& pair : result.empty_IO_map) {
    //     // pair.first 是键（Edge_index_type）
    //     // pair.second 是值，即 std::pair<Edge_type, Range>
    //     Edge_index_type index = pair.first;
    //     std::pair<Name_type,Name_type> pair_ = {"C906",index};
    //     result.ins_to_mod_pin_map["outside"+std::to_string(temp_add)].push_back(pair_);
    // }
    // temp_add = 0;



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
/*TEST*/
    auto it6 = result.mod_pin_to_ins_map.find("C906_x_aq_top_0*_lsu_biu_vb_wvalid");
    if( it6 != result.mod_pin_to_ins_map.end()){
        int a = 1;
    }
/*TEST*/

    /*遍历所有点 + 处理 + 生成压入 Graph */
    for (auto ins : gra.get_instance_list()){
        Name_type v_name = std::get<1>(ins.get_instance_data());
        std::cout<<"IO前遍历的节点名字: "<<result.prefix+"_"+v_name<<std::endl;
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
        /***遍历正在处理的点的所有边，处理边***/
        for (size_t counter = 0 ; counter < e_name_list.size() ; ++counter) {
        
                // bool is_this_edge_finish;
                //好像后续可以所有边只遍历一次，但是好难改
            /**只处理不是io的情况**/
            Name_type e_name = e_name_list[counter];//正在处理的 边名字
            std::cout<<"  正在处理的正常边: "<<result.prefix+"_"+ e_name<<std::endl;
            auto io_ = gra.IO_map.find(e_name);//用于判断是否是IO_pin
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
                    //默认权重
                int e_weight = 1;
                /* 遍历正在处理的边的所有连接点，判断ins mod分别处理*/
                for(size_t counter_ =0;counter_<e_i_list.size();++counter_){
                    Name_type connect_v_name = e_i_list[counter_];

                    Direction connect_v_diec = e_i_d_list[counter_];
                    bool should_add_v=(connect_v_name!=v_name) and (connect_v_diec!= this_ins_to_edge_direc);
                    
                    std::cout<<"    正在处理连边的点: "<<result.prefix+"_"+connect_v_name<<std::endl;
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
                            if(counter_!=0){ // int a = 1;
                                if(connect_v_name == e_i_list[counter_ - 1 ]){
                                    continue;
                                }
                            }
                            for( size_t sub_pin_index = 2 ; sub_pin_index < sp_list.size() ; sub_pin_index += 2){
                                if(sp_list[sub_pin_index]==e_name){
                                    // 这里会找到点 ->边 -> 所连接的mod的 所有pin口
                                    // 所以前面如果mod名字重复就不再操作, continue 出去下一个点
                                    /*TEST*/    
                                        if(v_name == "U165"){
                                            int a = 1;
                                        }
                                    /*TEST*/    
                                    to_pin_vector.push_back({connect_v_name,connect_v_name+"_"+sp_list[sub_pin_index-1]});

                                    std::cout<<"      ins->mod_pin_map ["<<result.prefix <<"_"<< v_name<<"] <- "<< connect_v_name+"_"<<sp_list[sub_pin_index-1]<<std::endl;

                                    //key是正在处理一开始 点的 name，生成点的时候再压入graph储存
                                    //value 为 <点连接的module的name, 点连接的module的pin name>
                                    //也就是 ins to mod pin map
                                }
                                // 点的邻接链就不存module了，另用ins_to_mod_pin来存
                                // 后续submodule递归 之前找到pinname到name的映射，也就是mod pin to ins map
                                // 递归submodule的时候 module内pin name 找外部 mod_to_pin_ins_map 来找到所连接的ins
                                // 再 将内部信息 push进点的邻接 
                            }
                            /**找到正在连接现在在处理的边对应的pin**/
                                
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
                                    if(counter_!=0){ // int a = 1;
                                        if(connect_v_name == e_i_list[counter_ - 1 ]){
                                            continue;
                                        }
                                    }//与非assign一样，防止重复push ins名 到modpin的map
                                    for( size_t sub_pin_index = 2;sub_pin_index < sp_list.size();sub_pin_index += 2){
                                        if(sp_list[sub_pin_index]==e_name){
                                            to_pin_vector.push_back({connect_v_name,sp_list[sub_pin_index-1]});                                    
                                            std::cout<<"      ins->mod_pin_map ["<<result.prefix <<"_"<< v_name<<"] <- "<< sp_list[sub_pin_index-1]<<std::endl;

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
        /***遍历边，处理的点的所有边，处理边***/
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
    /*遍历所有点+处理+生成压入Graph/  
        /*TEST*/
            if(t_p == "add_277*"){
                int a = 1;
            }
            auto it61 = result.mod_pin_to_ins_map.find("C906_x_aq_top_0*_lsu_biu_vb_wvalid");
            if( it61 != result.mod_pin_to_ins_map.end()){
                int a = 1;
            }
        
        
        /*TEST*/
    /*遍历IO边 <- IO边处理需要在所有点处理完之后*/
    for (auto IO_ = gra.IO_map.begin(); IO_ != gra.IO_map.end(); ++IO_){
    
        Name_type IO_name = IO_ -> first;
        Edge_type IO_type = IO_ -> second.first; 
        std::cout<<"正在处理的IO边: "<<result.prefix+"_"+IO_name<< " -> "<<result.prefix+"_"<<IO_type<<std::endl;
        /*TEST*/
            if(1){
                int a = 1;
            }
        /*TEST*/
        auto IO_edge_index = gra.E_map.find(IO_name);// 找到IO边index
        auto em_ = result.empty_IO_map.find(IO_name);
        Edge now_edge = gra.get_internal_edge_list()[IO_edge_index -> second];
        if(now_edge.get_type() == Multi){
            continue;
        }
        bool is_connect_empty;//默认不是空
        if(em_ == result.empty_IO_map.end()){//没有找到 不在empty列表 IO不连着空
            is_connect_empty = false; // 在empty表 设置为true
        }
        else{//找得到 那就是空 （处理完）erase掉
            is_connect_empty = true;
            result.empty_IO_map.erase(IO_name);
        }
        if(IO_edge_index == gra.E_map.end()){ // cerr
            std::cerr<<"Cannot find IO_edge's index."<<std::endl;
        }
        else{
            Name_type e_name = now_edge.get_name();
            std::vector<Name_type> e_i_list = now_edge.get_adjacency_array();
            std::vector<Direction> e_i_d_list = now_edge.get_adjacency_array_direction();
            std::vector<Name_type> assign_list = now_edge.get_assign_list();
            /*OUTPIN处理****情况*/
            if(IO_type == OUTPUT){
                std::vector< Name_type> same_dire_v_list ;
                std::vector< Name_type> not_same_v_list;
                size_t size = e_i_d_list.size();
                /*遍历IO边连接的点找到方向信息并分类*/
                for (size_t ins_index = 0; ins_index < size ; ++ins_index) {    
                    Name_type v_name = e_i_list[ins_index];
                    Direction v_direc = e_i_d_list[ins_index];//v2e is 1 <=> e2v is 2
                    
                    // bool this_is_mod = (v_name.back() == '*');
                    if(v_direc == v2e){ // OUTPUT 特殊点/同向的点 是 点到边 是v2e
                        same_dire_v_list.push_back(v_name);
                    }
                    else{ // 不同向
                        not_same_v_list.push_back(v_name);
                    }
                }
                /*遍历IO边连接的点找到方向信息并分类*/
                
                /*若有assign，遍历IO边assign连接的点找到方向信息并分类*/                
                if(( !assign_list.empty()) /*&& (assign_list.back().back()=='-'*/){
                    assign_list.back().pop_back();
                    Name_type assign_edge_name = assign_list.back();
                    auto assign_e = gra.E_map.find(assign_edge_name);      
                    Edge a_edge = gra.get_internal_edge_list()[assign_e -> second];
                    std::vector<Name_type> a_i_list = a_edge.get_adjacency_array();
                    std::vector<Direction> a_i_d_list = a_edge.get_adjacency_array_direction();
                    size = a_i_list.size();
                    for (size_t ins_index = 0; ins_index < size ; ++ins_index) {    
                        Name_type v_name = a_i_list[ins_index];
                        Direction v_direc = a_i_d_list[ins_index];//v2e is 1 <=> e2v is 2
                        
                        // bool this_is_mod = (v_nam e.back() == '*');
                        if(v_direc == v2e){ // OUTPUT 特殊点/同向的点 是 点到边 是v2e
                            same_dire_v_list.push_back(v_name);
                        }
                        else{ // 不同向
                            not_same_v_list.push_back(v_name);
                        }
                    }
                }
                /*若有assign，遍历IO边assign连接的点找到方向信息并分类*/

                /*对这条IO边的信息进行处理*/
                for(auto s_v : same_dire_v_list){
                    //情况12 + 567
                    if(s_v.back()!='*'){
                        size_t size_counter = not_same_v_list.size();
                        //需要处理12
                        if(size_counter != 0){
                            for (auto c_index = 0; c_index < size_counter; ++c_index ){
                                Name_type c_v = not_same_v_list[c_index];
        std::cout<<"    正在处理IO边ins "<< result.prefix+"_"+s_v<<std::endl;
                                bool is_connect_mod = (c_v.back()=='*');
                                //情况1
                                if(!is_connect_mod){
        std::cout<<"        连接到 ins "<< result.prefix+"_"+c_v<<std::endl;
                                    Name_type s_v_p = result.prefix +"_"+s_v;
                                    Name_type c_v_p = result.prefix +"_"+c_v;
                                    result.vertexs[s_v_p]->add_outDgree();//1是权重
                                    result.GraphAdjList_Plus[s_v_p].push_back({c_v_p,1});
                                    result.vertexs[c_v_p]->add_inDgree();
                                    result.GraphAdjList_Minus[c_v_p].push_back({s_v_p,1});
                                }
                                //情况1
                                //情况2
                                else{
                                    Name_type c_v_p = result.prefix +"_"+c_v;
                                    Name_type s_v_p = result.prefix +"_"+s_v;
        std::cout<<"        连接到 mod "<<result.prefix+"_"+c_v_p<<std::endl;
                                    size_t submodule_index = gra.Submodule_map[c_v];
                                    std::vector<Name_type> sp_list = gra.get_submodule_pin_edge_list()[submodule_index];
                            
                    
                                    /**遍历对应module的pin_线 map 找到线对应pin 更新ins to mod pin**/                                       
                                    bool xx = false;
                                    for( size_t sub_pin_index = 2 ; sub_pin_index < sp_list.size() ; sub_pin_index += 2){
 
                                        if(sp_list[sub_pin_index] == e_name){
                                            xx = true;
                                            Name_type c_pin_name = sp_list[sub_pin_index-1];
                                            std::pair<Name_type, Name_type> pair_value(c_v_p, c_pin_name);
                                            auto it = result.ins_to_mod_pin_map.find(s_v_p);
                                            if (it != result.ins_to_mod_pin_map.end()) {
                                                // 存在，向 vector 中 push_back 新的 pair
                                                it->second.push_back(pair_value);
                                            } 
                                            else {
                                                // 不存在，使用 [ ] 运算符插入新 key，并给它赋值一个包含 pair 的 vector
                                                result.ins_to_mod_pin_map[s_v] = {pair_value};
                                            }    
        std::cout<<"            即 ins map 到"<<c_v_p<<", "<< "pinname"<<std::endl;                                                                 
                                        }
                                    }
                                    if(xx){
                                        std::cerr<<"在"<< c_v_p <<"的module pin map中"<<
                                            " 找不到正在处理的IO边"<<std::endl;
                                    }
                                /**遍历对应module的pin_线 map 找到线对应pin 更新ins to mod pin**/

                                }
                                //情况2

                            }
                        }
                        //需要处理12
                        /*ins567*/
                        /*ins567*/
                    }    //需要处理12
                    else{ //情况34 + 567
                        std::cout<<"  output出去的mod为: "<<result.prefix+"_"+s_v<<std::endl;
                        //先找到submodule对应这条IO边的信息
                        // 根据名字找到mod信息
                        auto mod = gra.Submodule_map.find(s_v);
                        size_t sub_index = mod -> second; //这是对应submodule的index
                        // gra.get_submodule_list()[sub_index].second//这个是module指针 没用上
                        /**外部**//*情况5*//*输出到empty*/

                        /**外部**//*情况5*//*输出到empty*/
                        /**外部**//*情况6*//*输出到ins*/
                        /**外部**//*情况6*//*输出到ins*/
                        /**外部**//*情况7*//*输出到mod*/
                        /**外部**//*情况7*//*输出到mod*/
                    }
                   

                } 
                /*对这条IO边的信息进行处理*/
            }
            /*OUTPIN处理****情况*/
            /*INPIN处理*****情况*/
            else{ // IO_type == INPUT
                Name_type same_dire_v ;
                std::vector< Name_type> not_same_v_list;
                size_t size = e_i_d_list.size();
                /*遍历IO边连接的点*/
                for (size_t ins_index = 0; ins_index < size ; ++ins_index) {    
                    Name_type v_name = e_i_list[ins_index];
                    Direction v_direc = e_i_d_list[ins_index];//v2e is 1 <=> e2v is 2
                    bool this_is_mod = (v_name.back() == '*');
                    if(!(this_is_mod)){  // 情况5 输入到 ins
                        
                        int a = 1;
                    }
                    else{//情况6 输入到 mod

                        int a = 1;
                    }

                }
                /*遍历IO边连接的点*/

            }
            /*INPIN处理*****情况*/
                    // /*遍历IO边连接的点***/
                    // size_t size = e_i_d_list.size();
                    // Name_type only_to_IO_name;
                    // std::vector< Name_type > other_to_IO_name_list;
                    // for (size_t ins_index = 0; ins_index < size ; ++ins_index) {    
                    //     Name_type v_name = e_i_list[ins_index];
                    //     Direction v_direc = e_i_d_list[ins_index];//v2e is 1 <=> e2v is 2
                    //     bool this_is_mod = (v_name.back() == '*');
                    // }
                    // /*遍历IO边连接的点***/




        }
    }
    /*遍历IO边 <- IO边处理需要在所有点处理完之后*/

        std::cout<<"\t\t已处理点数量: "<<temp_add+1<<std::endl;
    /*TEST*/
    auto it = result.mod_pin_to_ins_map.find("C906_x_aq_top_0*_lsu_biu_vb_wvalid");
    if( it != result.mod_pin_to_ins_map.end()){
        int a = 1;
    }
    /*TEST*/
/***Processing points for the current module***/
/**处理前遍历map 生成/更新ins to mod pin map**/
    result.mod_pin_to_ins_map.clear();
    for (auto it = result.ins_to_mod_pin_map.begin(); it != result.ins_to_mod_pin_map.end(); ++it) {
        const auto& ins_name = it->first;
        const auto& pin_pairs = it->second;
        for (auto jt = pin_pairs.begin(); jt != pin_pairs.end(); ++jt) {
            const auto& mod_name = jt->first;
            const auto& pin_name = jt->second;
            std::string key = mod_name + "_" + pin_name;
            if( key=="C906_x_aq_top_0*_lsu_biu_vb_wvalid"){
                int a = 1;
            }
            result.mod_pin_to_ins_map[key].push_back(ins_name);
        }
    }
/**处理前遍历map 生成/更新ins to mod pin map**/

/***有子模块，进入内部递归处理***/
    size_t submodule_number = gra.Submodule_map.size();
    result.is_top = false;//?
    if( submodule_number != 0 ){
        for(size_t submodule_counter = 0 ; submodule_counter < submodule_number ; submodule_counter++ ){
            Module submodule= *std::get<1>(gra.get_submodule_list()[submodule_counter]);
            result.this_submodule_index= std::get<0>(gra.get_submodule_list()[submodule_counter]);
            if(temp_add == 3 ){
                int a = 1;
            }
            auto it = result.mod_pin_to_ins_map.find("C906_x_aq_top_0*_lsu_biu_vb_wvalid");
            if( it != result.mod_pin_to_ins_map.end()){
                int a = 1;
            }
            module_to_graph_old(result, submodule, sub_map, temp_add);
            auto it1234 = result.mod_pin_to_ins_map.find("C906_x_aq_top_0*_lsu_biu_vb_wvalid");
            if( it1234 != result.mod_pin_to_ins_map.end()){
                int a = 1;
            }
            int a  = 1;
        }
    }
/***有子模块，进入内部递归处理***/

    /*TEST*/
        if(0){
            Name_type t_name = "U119";
            auto t1 = result.vertexs.find(t_name);
            auto t2 = result.GraphAdjList_Minus.find(t_name);
            auto t3 = result.GraphAdjList_Plus.find(t_name);
            int a = 1;
        }
    /*TEST*/
    int n = t_p.size();
    if (n > result.prefix.size()) {
        n = result.prefix.size();
    }
    /**恢复prefix到上一级**/
    result.prefix.erase(result.prefix.size() - (n+1) );
    /**恢复prefix到上一级**/
}