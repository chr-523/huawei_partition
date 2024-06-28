#include <ctime>
#include <string>
#include <iostream>
#include "process.h"

bool is_clk_module(const Name_type& module_name){
    Name_type instance_prefix = "gated_clk_";
    Name_type name_prefix = module_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

//Determine whether it is instance or module through instance_name's prefix
//If instance -> true     if module -> false*
bool is_instance_type(const Name_type& instance_name){
    Name_type instance_prefix_1 = "sky130_fd_sc_hd__";
    Name_type instance_prefix_2 = "aq_spsram_";

    Name_type name_prefix_1 = instance_name.substr(0, instance_prefix_1.length() );
    Name_type name_prefix_2 = instance_name.substr(0, instance_prefix_2.length() );
    return (name_prefix_1 == instance_prefix_1) || (name_prefix_2 == instance_prefix_2);
}

bool is_ins_test(const Name_type& instance_name){
    Name_type instance_prefix = "tu_test_ins";    
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}    

std::vector<std::string> split_by_brackets(const std::string& line, bool use_big_brackets = false){
    std::string re_p;
    if (use_big_brackets){
        re_p = "\\{([^}]+)\\}";
    }
    else{
        re_p = "\\(([^)]+)\\)";
    }
    std::regex re(re_p); // Regular expression matching content within parentheses
    std::sregex_iterator next(line.begin(), line.end(), re);
    std::sregex_iterator end;
    std::vector<std::string> line_data;
    std::vector<std::string> edge_data;
    for ( ; next != end; ++next) {
        line_data.push_back( next -> str(1) ); // str(1) gets the content within the first parentheses
    }
    return line_data;
}

std::vector<std::string> get_group_edge_data(const std::string& edge_data) {
    std::vector<std::string> result_temp;
    std::vector<std::string> result;
    result_temp = split_by_brackets(edge_data, true);
    result = result_temp;
    return result;

}


std::vector<std::string> get_edge_data(const std::string& edge_data) {
    std::vector<std::string> result;
    std::istringstream iss(edge_data);
    std::string token;
    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

Graph_data read_file(const std::string& output_path){
    Module gra;
    std::unordered_map<Name_type, Module*> sub_map;
    std::ifstream file(output_path);
    std::string line;
    size_t module_counter = 0;
    size_t line_c = 2; //第一行开始，且第一行被忽略，所以第二行开始
    double total_time = 0.0;
    clock_t start = clock(); // start time
    clock_t start2 = clock(); // start time
    double max_time = 0.0;
    size_t max_time_module =0;

    if ( !file.is_open() ){
        // When you need to display information or results to users, use std::cout.
        // When you need to report errors or abnormal situations, use std::cerr.
        std::cerr << "Can not open file. " << std::endl;
        
        Graph_data result(gra, sub_map);
        return result; // empty graph
    }


    getline(file,line);
    char first_char;
    while(file.get(first_char)){
        if(line_c == 1120){
            int a=1;
        }
        if(first_char != 'i'){ // not ins
            getline(file,line);
            line_c++;
            if (line[0]=='o'){//module    

                clock_t end = clock(); // 记录结束时间
                double elapsed_time = double(end - start) / CLOCKS_PER_SEC; // 计算经过的秒数
                double one_module_time = double(end - start2) / CLOCKS_PER_SEC; // 计算经过的秒数
                std::cout << "Line:"<< line_c << "          ";
                std::cout << "Modulenumber:"<<module_counter << std::endl;
                std::cout << "Used time: " << elapsed_time << " s" << "          ";
                std::cout << "This module used time: "<<one_module_time <<std::endl<<std::endl;;
                if(one_module_time > max_time){
                    max_time = one_module_time;
                    max_time_module=module_counter;
                }

                if(module_counter == 1595){
                    
                    int a =1;
                }

                // if(module_counter ==  1706){
                //     int a= 1;
                // }

                start2 = clock(); // start time                 
                
                module_counter++;
                Name_type module_name = line.substr(6);
                Name_type previous_module_name = gra.get_module_name();
                if(module_counter!=1){
                    Module* gra_2 = new Module(gra);
                    // xxx.insert 的是{pair}
                    sub_map.insert({previous_module_name, gra_2});
                }
                gra.clear();
                gra.set_module_name(module_name);
                // std::cout<<"module:"<<module_name<<std::endl;      
            }
            else if(line[0]=='i' or line[0]=='e'){ // pin net
                std::vector<std::string> pn_data = split_by_brackets(line);
                std::vector<std::string> e_data = get_edge_data(pn_data[0]);
                Edge_type e_type;
                // Edge_type e_name = e_data[0];
                int low = std::stoi(e_data[1]);
                int high = std::stoi(e_data[2]);
                if (low > high) std::swap(low, high); // just in case
                Range r_(low,high);
                if (pn_data[1] == "1"){
                    e_type = INPUT;
                    gra.IO_map[e_data[0]]={e_type,r_};
                }
                else if(pn_data[1]=="2"){
                    e_type = OUTPUT;
                    gra.IO_map[e_data[0]]={e_type,r_};
                    
                }
                else{
                    e_type = NORMAL;
                }
                gra.add_edge(e_data[0],low,high,e_type);
                
            }
            else if(line[0]=='s'){ //assign
                std::vector<std::string> pn_data = split_by_brackets(line);
                std::vector<std::string> e_data_1 = get_edge_data(pn_data[0]);
                std::vector<std::string> e_data_2 = get_edge_data(pn_data[1]);
                Edge_index_type e_name_1;
                Edge_index_type e_name_2;
                if(e_data_1[2] != "-1"){
                    e_name_1 = e_data_1[0]+"_"+e_data_1[2]; 
                }
                else{
                    e_name_1 = e_data_1[0];
                }                
                if(e_data_2[2] != "-1"){
                    e_name_2 = e_data_2[0]+"_"+e_data_1[2]; 
                }
                else{
                    e_name_2 = e_data_2[0];
                }
                gra.assign_edge_list.push_back({e_name_1,e_name_2});

            }
        }
        else{// is ins or mod
            char last_ch;
            std::string data;
            size_t ins_data_counter = 0;
            Instance_index_type ins_type_name;
            Instance_index_type ins_name;
            Instance_index_type pin_name;
            Name_type pin_name_subfix;
            size_t subfix_counter=0;
            bool meet_group=false;
            bool need_right_bracket=false;
            bool is_this_line_ins;
            while(file.get(first_char)){

                if(first_char=='\n'){
                    line_c++;
                    break;
                }
                if(first_char=='('){
                    data.clear();
                    ins_data_counter++;
                    need_right_bracket = true;
                    if(meet_group) {
                        meet_group=false;
                        subfix_counter =0;
                    }
                }
                else if(first_char==')'){
                    need_right_bracket = false;
                    if(ins_data_counter==1){
                        ins_type_name = data; 
                        data.clear();
                    }
                    else if(ins_data_counter==2){
                        ins_name = data;
                        data.clear();
                        is_this_line_ins = is_instance_type(ins_type_name);
                        if(is_this_line_ins){
                            gra.add_instance(ins_type_name,ins_name);
                        }
                        else{
                            auto sub_module = sub_map.find(ins_type_name);
                            if (sub_module == sub_map.end() ){
                                std::cerr << "can not find submodule" << std::endl;
                            }

                            gra.add_module(ins_name, sub_module -> second);
                            int a = 1;
                            //add module
                        }
                    }
                    else{
                        // std::cout << data;
                        if(!meet_group){
                            if(ins_data_counter%2!=0){ // pin name
                                pin_name =  data;
                            }
                            else{ // edge data
                            std::vector<std::string> edge_data = get_edge_data(data);
                            Name_type edge_name = edge_data[0];
                            int low = std::stoi(edge_data[1]);
                            int high = std::stoi(edge_data[2]);
                            if (low > high) std::swap(low, high); // just in case
                            Range r_(low,high);
                            if(is_this_line_ins){
                                gra.add_back_ins_edge(edge_name);
                                // gra.internal_instance.back().connect_edge_list.push_back(edge_name);
                                connect_ins_edge(gra,ins_name,edge_name,r_);
                            }
                            else{
                                connect_mod_edge(gra,ins_name,edge_name,r_);
                                gra.add_submodule_pin_edge(pin_name);
                                gra.add_submodule_pin_edge(edge_name);
                            }
                        }
                        }
                    }
                }
                else if(first_char =='{'){
                    Name_type ge_data;
                    int low;
                    int high;
                    bool to_h=false;
                    char ch;
                    while(file.get(ch)){
                        int a =1;
                        if(ch!='}'){ 
                            ge_data+=ch;
                        }
                        else{ // 遇到}了
                            meet_group =true;
                            std::vector<std::string> e_data = get_edge_data(ge_data);
                            Name_type e_name_temp = e_data[0];
                            int high = std::stoi(e_data[1]);
                            int low = std::stoi(e_data[2]);
                            if (low > high) std::swap(low, high);
                            Name_type e_name;
                            Name_type p_name;
                            if(low<0 and high <0){
                                e_name = e_name_temp;
                                if(subfix_counter!=0){
                                    p_name = pin_name +"_"+std::to_string(subfix_counter);
                                }
                                else{
                                    p_name = pin_name;
                                }
                                Range r_(-1,-1);
                                //显示是单线{a}，但外部wire [3:0]a也有可能 
                                //t_不是0就有问题
                                if(pin_name == "SUM"){
                                    int a =1;
                                }
                                size_t t_ = connect_mod_edge(gra,ins_name,e_name,r_,p_name,subfix_counter);
                                if(t_ !=0){
                                    subfix_counter+=t_;
                                    int a =1;
                                }
                                else{
                                    gra.add_submodule_pin_edge(p_name);
                                    gra.add_submodule_pin_edge(e_name);
                                    // std::cout<< "p to e:"<<p_name <<" to " <<e_name <<std::endl;
                                }
                                subfix_counter++;
                            }
                            else if(low<0 and high >=0){
                                e_name = e_name_temp +"_"+std::to_string(high);
                                p_name = pin_name +"_"+std::to_string(subfix_counter);
                                subfix_counter++;
                                Range r_(-1,-1);
                                connect_mod_edge(gra,ins_name,e_name,r_);
                                gra.add_submodule_pin_edge(p_name);
                                gra.add_submodule_pin_edge(e_name);
                            }
                            else{
                                for (int counter = low; counter <= high; ++counter) {
                                    e_name = e_name_temp+"_"+std::to_string(counter);
                                    if(line_c== 10923){
                                        int a =1;
                                    }
                                    p_name = pin_name +"_"+std::to_string(subfix_counter);
                                    subfix_counter++;
                                    Range r_(-1,-1);
                                    connect_mod_edge(gra,ins_name,e_name,r_);
                                    gra.add_submodule_pin_edge(p_name);
                                    gra.add_submodule_pin_edge(e_name);
                                    // std::cout<< "p to e:"<<p_name <<" to "<<e_name <<std::endl;

                                }
                            }
                            break;//lianxian
                            last_ch = '{';
                        }
                    }

                }
                else {
                    data += first_char;
                }
            }
        }
    }
    Graph_data result(gra, sub_map);
    
    std::cout<<"The number that took the most time is: "<<max_time_module<<std::endl;
    std::cout <<"Time is: "<<max_time<<std::endl;
    return result;

}
//read_net
Module read_file_1(const std::string& filename){

    Module gra;
    std::unordered_map<Name_type, Module*> sub_map;
    std::ifstream file(filename);

    clock_t start = clock(); // start time

    if ( !file.is_open() ){
        // When you need to display information or results to users, use std::cout.
        // When you need to report errors or abnormal situations, use std::cerr.
        std::cerr << "Can not open file. " << std::endl;
        return gra; // empty graph
    }

    char currentChar;

    std::string line;

    std::getline(file, line); // ignore the first line

    bool first_module = true;

    size_t module_number = 0;

    size_t line_c = 1;

    std::string previous_module_name;

    while( getline(file, line) ){
            
        clock_t end = clock(); // 记录结束时间
        double elapsed_time = double(end - start) / CLOCKS_PER_SEC; // 计算经过的秒数
        std::cout << "Time: " << elapsed_time << " s" << std::endl;

        line_c++;
        if(line_c == 632572){
            int a = 1;
        };
        
        std::cout << line_c << std::endl;
        std::string token;

        if ( (line[0] == 'm') && (line[1] == 'o') && (line[2] == 'd') && (line[3] == 'u') && (line[4] == 'l') && (line[5] == 'e') ){
            module_number++;
            std::string module_name = line.substr(7);
            bool is_clk_mod = is_clk_module(module_name);
            if(is_clk_mod){
                int a = 1;
            }
            if( !(first_module) ){
                Module* gra_2 = new Module(gra);
                std::pair<Name_type, Module*> pair = std::make_pair(previous_module_name, gra_2);
                sub_map.insert(pair); // 组成pair再插入
                gra.clear();
            }
            else{
                first_module = false;
            }
            gra.set_module_name(module_name); //something wrong
            previous_module_name = module_name;
        } 
        else if (   ((line[0] == 'p') && (line[1] == 'i') && (line[2] == 'n')) or
                    ((line[0] == 'n') && (line[1] == 'e') && (line[2] == 't'))  ){
            std::regex re("\\(([^)]+)\\)"); // Regular expression matching content within parentheses
            std::sregex_iterator next(line.begin(), line.end(), re);
            std::sregex_iterator end;
            std::vector<std::string> edge_data;
            for ( ; next != end; ++next) {
                std::istringstream iss( next -> str(1) );
                // use space to split the string
                std::string temp;
                while(iss >> temp){
                    edge_data.push_back(temp);
                }
            }
            int low = std::stoi(edge_data[1]);
            int high = std::stoi(edge_data[2]);
            if (low > high) std::swap(low, high);
            Edge_type e_type;
            if( edge_data[3]=="1" ){ // input
                e_type = Edge_type::INPUT;
            }
            else if( edge_data[3]=="2" ){ // output
                e_type = Edge_type::OUTPUT;
            }
            else{ // NORMAL
                e_type = Edge_type::NORMAL;
            }
            gra.add_edge(edge_data[0], low, high, e_type);
            assert( gra.get_internal_edge_list().back().get_range().low == -1);
        }
        else if ( line.substr(0, 10) == "assignment" ){
            // 解析赋值信息
            // parseAssignment(line, gra);
            int instance = 1;
        } 
        else if ( line.substr(0,8) == "instance" ){
            // 解析实例化信息
            // parseInstance(line, gra, sub_map);
            std::vector<std::string> line_data;
            line_data = split_by_brackets(line);
            if( is_instance_type(line_data[0]) ){ // is instance
            // if( is_ins_test( line_data[0]) ){ // is instance
                std::queue< Name_type> edge_name_queue;
                std::queue< Range > range_queue;
                for (size_t counter = 2; counter< line_data.size(); counter += 2) {
                    if (counter + 1 < line_data.size()) { // 确保至少还有两个元素
                        Name_type connect_pin_name = line_data[counter]; // 使用索引访问元素
                        std::vector<std::string> edge_data = get_edge_data(line_data[counter + 1]);
                        if (edge_data.size() >= 2) { // 确保edge_data至少有两个元素
                            edge_name_queue.push(edge_data[0]);
                            Range range_(std::stoi(edge_data[1]), std::stoi(edge_data[2]));
                            range_queue.push(range_);
                        }
                    }
                }

                gra.add_instance(line_data[0], line_data[1]);//add (type, insname)
                connect_ins_edge(gra, line_data[1], edge_name_queue, range_queue);
            }
            else{ // is module
                std::queue< Name_type > edge_name_queue;
                std::queue< Name_type > pin_name_queue;
                std::queue< Range > range_queue;
                for (size_t counter = 2; counter< line_data.size(); counter += 2) {
                    if (counter + 1 < line_data.size()) { // ensure as least 2 ele

                        std::vector< std::string > edge_data;
                        // not begin with 123 '{'
                        if( line_data[counter+1][0] != 123 ){
                            edge_data = get_edge_data(line_data[counter + 1]);
                            if (edge_data.size() >= 2) { // ensure edge_data at least 2ele
                                Name_type edge_name_temp = edge_data[0];
                                Name_type pin_name_temp = line_data[counter]; // use index to fangwen
                                Name_type edge_name;
                                Name_type pin_name;
                                int low = std::stoi(edge_data[2]);
                                int high = std::stoi(edge_data[1]);
                                if (low > high) std::swap(low, high);
                                if (low < 0 ){ // means signal edge
                                    if (high > 0){
                                        edge_name = edge_name_temp + '_' + std::to_string(high);
                                    }
                                    else{
                                        edge_name = edge_name_temp;
                                    }
                                    pin_name = pin_name_temp; // suffix is unnecessary for signal-edge
                                    edge_name_queue.push(edge_name);
                                    pin_name_queue.push(pin_name);
                                    Range range_(low, high);
                                    range_queue.push(range_);
                                }
                                else{ // low and high both > 0 means multi-edge, split it
                                    for (size_t now_high= 0; now_high <= high; now_high++){ 
                                        edge_name = edge_name_temp + '_' + std::to_string(now_high); 
                                        edge_name_queue.push(edge_name);
                                        // unnecessary to use pin_counter 
                                        // because edge_suffix is equal to pin_suffix in this case
                                        pin_name = pin_name_temp + '_' + std::to_string(now_high); 
                                        pin_name_queue.push(pin_name);
                                        Range range_(-1, now_high);
                                        range_queue.push(range_);
                                    }
                                }
                            }
                        }
                        else{ // begin with '{' which means group edge
                            std::vector< std::string > edge_data_list;
                            int pin_counter = 0;
                            edge_data_list = get_group_edge_data(line_data[counter + 1]);
                            for(size_t counter_l = 0 ; counter_l < edge_data_list.size(); counter_l++){
                                edge_data = get_edge_data(edge_data_list[counter_l]);
                                if (edge_data.size() >= 2){ // ensure edge_data at least 2ele
                                    Name_type edge_name_temp = edge_data[0];
                                    Name_type edge_name;
                                    Name_type pin_name_temp = line_data[counter];
                                    Name_type pin_name;
                                    int low = std::stoi(edge_data[2]);
                                    int high = std::stoi(edge_data[1]);
                                    if (low > high) std::swap(low, high); // just in case
                                    if(low < 0 ){
                                        if (high >= 0){
                                            edge_name = edge_name_temp + '_' + std::to_string(high);
                                        }
                                        else{
                                            edge_name = edge_name_temp;
                                        }
                                        pin_name = pin_name_temp + '_' + std::to_string(pin_counter);
                                        edge_name_queue.push(edge_name);
                                        pin_name_queue.push(pin_name);
                                        pin_counter++;
                                        Range range_(low, high);
                                        range_queue.push(range_);
                                    }
                                    else{
                                        for (size_t now_high= 0; now_high <= high; now_high++){ 
                                            edge_name = edge_name_temp + '_' + std::to_string(now_high); 
                                            edge_name_queue.push(edge_name);
                                            pin_name = pin_name_temp + '_' + std::to_string(pin_counter); 
                                            pin_name_queue.push(pin_name);
                                            pin_counter++;
                                            Range range_(-1, now_high);
                                            range_queue.push(range_);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                auto sub_module = sub_map.find(line_data[0]);
                if (sub_module == sub_map.end() ){
                    std::cerr << "can not find submodule" << std::endl;
                }
                std::vector< std::tuple<Edge_index_type, Edge_index_type >> e_l;
                // gra.add_module(e_l, line_data[1], sub_module -> second);
                // // gra,type,index,e_que,p_que,r_que
                // connect_mod_edge(gra, line_data[1], edge_name_queue, pin_name_queue, range_queue);

            }
        } 
        else{
            std::cerr << "Unknown line token: " << line << std::endl;
        }

    }

    file.close();
    return gra;
}
