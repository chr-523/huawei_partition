
#include "process.h"

bool is_clk_module(const Name_type& module_name){
    Name_type instance_prefix = "gated_clk_";
    Name_type name_prefix = module_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

//Determine whether it is instance or module through instance_name's prefix
//If instance -> true     if module -> false*
bool is_instance_type(const Name_type& instance_name){
    Name_type instance_prefix = "sky130_fd_sc_hd__";    
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}

bool is_ins_test(const Name_type& instance_name){
    Name_type instance_prefix = "tu_test_ins";    
    Name_type name_prefix = instance_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}    



std::vector<std::string> get_edge_data(const std::string& tokens) {
    std::vector<std::string> result;
    std::istringstream iss(tokens);
    std::string token;
    while (iss >> token) {
        result.push_back(token);
    }
    return result;
}

//read_net
Graph read_file(const std::string& filename){

    Graph gra;
    std::unordered_map<Name_type,Graph*> sub_map;
    std::ifstream file(filename);

    if ( !file.is_open() ){
        // When you need to display information or results to users, use std::cout.
        // When you need to report errors or abnormal situations, use std::cerr.
        std::cerr << "Can not open file. " << std::endl;
        return gra; // empty graph
    }

    std::string line;
    std::getline(file, line); // ignore the first line

    while( getline(file, line) ){

        std::string token;

        if ( (line[0] == 'm') && (line[1] == 'o') && (line[2] == 'd') && (line[3] == 'u') && (line[4] == 'l') && (line[5] == 'e') ){
            std::string module_name = line.substr(7);
            Graph* gra_2 = new Graph(gra);
            std::pair<Name_type, Graph*> pair = std::make_pair(module_name, gra_2);
            gra.set_module_name(module_name); //something wrong
            sub_map.insert(pair); // 组成pair再插入
        } 
        else if (   ((line[0] == 'p') && (line[1] == 'i') && (line[2] == 'n')) or
                    ((line[0] == 'n') && (line[1] == 'e') && (line[2] == 't'))  ){
            std::regex re("\\(([^)]+)\\)"); // Regular expression matching content within parentheses
            std::sregex_iterator next(line.begin(), line.end(), re);
            std::sregex_iterator end;
            std::vector<std::string> token_list;
            std::vector<std::string> tokens;
            std::vector<std::string> tokens_1;
            for ( ; next != end; ++next) {
                token_list.push_back( next -> str(1) ); // str(1) gets the content within the first parentheses
                std::istringstream iss( next -> str(1) );
                // use space to split the string
                std::string temp;
                while(iss >> temp){
                    tokens.push_back(temp);
                }
            }
            int low = std::stoi(tokens[1]);
            int high = std::stoi(tokens[2]);
            Edge_type e_type;
            if( tokens[3]=="1" ){ // input
                e_type = INPUT;
            }
            else if( tokens[3]=="2" ){ // output
                e_type = OUTPUT;
            }
            else{ // NORMAL
                e_type = NORMAL;
            }
            gra.add_edge(tokens[0], low, high, e_type);
        }
        else if ( line.substr(0, 10) == "assignment" ){
            // 解析赋值信息
            // parseAssignment(line, gra);
            int instance = 1;
        } 
        else if ( line.substr(0,8) == "instance" ){
            // 解析实例化信息
            // parseInstance(line, gra, sub_map);
            std::regex re("\\(([^)]+)\\)"); // Regular expression matching content within parentheses
            std::sregex_iterator next(line.begin(), line.end(), re);
            std::sregex_iterator end;
            std::vector<std::string> token_list;
            std::vector<std::string> tokens;
            for ( ; next != end; ++next) {
                token_list.push_back( next -> str(1) ); // str(1) gets the content within the first parentheses
            }
            // if( is_instance_type(token_list[0]) ){ // is instance
            if( is_ins_test( token_list[0]) ){ // is instance
                gra.add_instance(token_list[0], token_list[1]);//add (type, insname)

                std::queue< Name_type> edge_name_queue;
                std::queue< Range > range_queue;
                for (size_t counter = 2; counter< token_list.size(); counter += 2) {
                    if (counter + 1 < token_list.size()) { // 确保至少还有两个元素
                        Name_type connect_pin_name = token_list[counter]; // 使用索引访问元素
                        std::vector<std::string> tokens = get_edge_data(token_list[counter + 1]);
                        if (tokens.size() >= 2) { // 确保tokens至少有两个元素
                            edge_name_queue.push(tokens[0]);
                            Range range_(std::stoi(tokens[1]), std::stoi(tokens[2]));
                            range_queue.push(range_);
                        }
                    }
                }
                connect_ins_edge(gra, token_list[1], edge_name_queue, range_queue);
            }
            else{ // is module
                auto sub_module = sub_map.find(token_list[0]);
                if (sub_module == sub_map.end() ){
                    std::cerr << "can not find submodule" << std::endl;
                }
                gra.add_module( sub_module -> second );
                // gra.add_instance(token_list[0], token_list[1]);//add (type, insname)

                std::queue< Name_type> edge_name_queue;
                std::queue< Range > range_queue;

            }
        } 
        else{
            std::cerr << "Unknown line token: " << line << std::endl;
        }

    }

    file.close();
    return gra;
}
