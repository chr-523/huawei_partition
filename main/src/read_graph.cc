#include <ctime>
#include "read_graph.h"

//Determine whether it is only_clk_module through module_name's prefix
bool is_clk_module(const Name_type& module_name){
    Name_type instance_prefix = "gated_clk_";
    Name_type name_prefix = module_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

//Determine whether it is instance or module through instance_name's prefix
//If instance -> true     if module -> false
bool is_instance_type(const Name_type& instance_name){

    Name_type instance_prefix_1 = "sky130_fd_sc_hd__";
    Name_type name_prefix_1 = instance_name.substr(0, instance_prefix_1.length() );
    
    if(name_prefix_1 == instance_prefix_1){
        return true;
    }
    else{
        Name_type instance_prefix_2 = "aq_spsram_";
        Name_type name_prefix_2 = instance_name.substr(0, instance_prefix_2.length() );
        return name_prefix_2 == instance_prefix_2;
    }

    return false;
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
    std::unordered_map<Name_type, Module*> submodule_map;

    Graph_data result(gra, submodule_map);
    
    return result;

}
//read_net
Graph_data read_file_1(const std::string& filename){

    Module gra;
    std::unordered_map<Name_type, Module*> sub_map;
    std::ifstream file(filename);

    if ( !file.is_open() ){
        // When you need to display information or results to users, use std::cout.
        // When you need to report errors or abnormal situations, use std::cerr.
        std::cerr << "Can not open file. " << std::endl;
        
        Graph_data result(gra, sub_map);
        return result; // empty graph
    }

    std::string line;

    std::getline(file, line); // ignore the first line

    int module_counter = 0;

    int line_c = 1;

    Name_type module_name;
    Name_type previous_module_name;
// char A;
// while(file.get(A)){
//     std::cout << A;

// }

    double total_time = 0.0;
    clock_t start = clock(); // start time
    std::vector<std::tuple<Edge_index_type,Edge_index_type>> module_assign_list;
    while( getline(file, line) ){
            
        clock_t end = clock(); // 记录结束时间
        double elapsed_time = double(end - start) / CLOCKS_PER_SEC; // 计算经过的秒数
        // std::cout << "Time: " << elapsed_time << " s" << std::endl;

        line_c++;

        // char a;
        // file.get(a);

        // std::cout << line_c << std::endl;

        if(line_c == 37){  //for test
            int a = 1;
        };

        if ( (line[0] == 'm') && (line[1] == 'o') && (line[2] == 'd') && (line[3] == 'u') && (line[4] == 'l') && (line[5] == 'e') ){
            // used to stored precious module into sub_map
            previous_module_name = module_name;
            // get the new module's name
            module_name = line.substr(7);
            // module_counter means how many modules have been processed
            if( module_counter != 0 ){ // not the first module
                int size = module_assign_list.size();
                for (int counter = 0; counter < size; ++counter) {
                    Edge_index_type edge_name_0 = std::get<0>(module_assign_list[counter]);
                    Edge_index_type edge_name_1 = std::get<1>(module_assign_list[counter]);
                    assign_2_edge(gra, edge_name_0,edge_name_1);
                }
                // If a new module appears, the previous module needs to be assigned 
                //     (the biggest module_assign's code is at the end)
                // Clear assign_list and use it to store the assignment information for this new module
                module_assign_list.clear();        
                // Store old modules in the sub_map
                    // Module* gra_2 = new Module(gra);
                    // std::pair<Name_type, Module*> pair = std::make_pair(previous_module_name, gra_2);
                    // sub_map.insert(pair); // Form a pair before insert it
                sub_map.insert( std::make_pair(previous_module_name, new Module(gra)));

                // Clear gra to store the new module
                gra.clear();

            }
            else{ // do nothing
                // The first module does not need to handle assign
                // so do nothing
            }

            module_counter++;
            gra.set_module_name(module_name);
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
            // parseAssignment(line, gra);
            std::vector<std::string> assign_data = split_by_brackets(line);
            std::vector<std::string> edge_data_0 = get_edge_data(assign_data[0]);
            std::vector<std::string> edge_data_1 = get_edge_data(assign_data[1]);
            // store assign's information
            // this will be processed after all other information has been processed
            module_assign_list.push_back(std::make_pair(edge_data_0[0],edge_data_1[0]));
        } 
        else if ( line.substr(0,8) == "instance" ){
            // parseInstance(line, gra, sub_map);
            std::vector<std::string> line_data;
            line_data = split_by_brackets(line);
            if( is_instance_type(line_data[0]) ){ // is instance // tesst
            // if( is_ins_test( line_data[0]) ){ // is instance // tesst
                std::queue< Name_type > edge_name_queue;
                std::queue< Name_type > pin_name_queue;
                std::queue< Range > range_queue;
                for (size_t counter = 2; counter< line_data.size(); counter += 2) {
                    if (counter + 1 < line_data.size()) { // 确保至少还有两个元素
                        Name_type connect_pin_name = line_data[counter]; // 使用索引访问元素
                        std::vector<std::string> edge_data = get_edge_data(line_data[counter + 1]);
                        if (edge_data.size() >= 2) { // 确保edge_data至少有两个元素
                            edge_name_queue.push(edge_data[0]);
                            pin_name_queue.push(connect_pin_name);
                            Range range_(std::stoi(edge_data[1]), std::stoi(edge_data[2]));
                            range_queue.push(range_);
                        }
                    }
                }

                gra.add_instance(line_data[0], line_data[1]);//add (type, insname)
                connect_ins_edge(gra, line_data[1], edge_name_queue, pin_name_queue, range_queue);
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
                gra.add_module(e_l, line_data[1], sub_module -> second);
                // gra,type,index,e_que,p_que,r_que
                connect_mod_edge(gra, line_data[1], edge_name_queue, pin_name_queue, range_queue);

            }
        } 
        else{
            std::cerr << "Unknown line token: " << line << std::endl;
        }
        total_time = elapsed_time;
    }

    file.close();

    std::cout << "Data in output.txt has been stored." << std::endl;
    std::cout << "The total time spent: " << total_time << " seconds." << std::endl << std::endl;

    // the assign operation of the last module (C906)  
    int size = module_assign_list.size();
    for (int counter = 0; counter < size; ++counter) {
        Edge_index_type edge_name_0 = std::get<0>(module_assign_list[counter]);
        Edge_index_type edge_name_1 = std::get<1>(module_assign_list[counter]);
        assign_2_edge(gra, edge_name_0, edge_name_1);
    }

    Graph_data result(gra, sub_map);
    
    return result;
}

Graph read_graph_data(const Module& gra_data){

    Graph result_gra;
    size_t level = 0;
    Name_type empty_index = "C_906";
    result_gra.read_graph_data(gra_data, &level, empty_index);

    return result_gra;
};

Graph read_graph_data(const Graph_data& gra_data){
    Graph result_gra = read_graph_data(gra_data.module);
    return result_gra;
};