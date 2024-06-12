
#include "process.h"

bool is_clk_module(const Name_type& module_name){
    Name_type instance_prefix = "gated_clk_";
    Name_type name_prefix = module_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}


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
            gra.set_module_name(module_name);
        } 
        else if ( (line[0] == 'p') && (line[1] == 'i') && (line[2] == 'n') ){

            std::regex re("\\(([^)]+)\\)"); // Regular expression matching content within parentheses
            std::sregex_iterator next(line.begin(), line.end(), re);
            std::sregex_iterator end;
            std::vector<std::string> token_list;
            for ( ; next != end; ++next) {
                token_list.push_back( next -> str(1) ); // str(1) gets the content within the first parentheses
                std::istringstream iss(next -> str(1) );
                std::vector<std::string> tokens;
                // 使用空格作为分隔符来分割字符串
                std::string temp;
                std::vector<std::string> edge_data;
                Edge_type e_t;
                while(iss >> temp){
                    if (temp != "0" && temp != "1" && temp != "2" ){
                        edge_data.push_back(temp);
                    }
                    else if(temp == "0"){//normal
                        e_t = NORMAL;
                    }
                    else if(temp == "1"){//input
                        e_t = INPUT;
                    }
                    else{ // temp == "2" //output
                        e_t = OUTPUT;
                    }
                }

                // im here graaddedge

            }
        } 
        else if ( (line[0] == 'n') && (line[1] == 'e') && (line[2] == 't') ){
            // 解析网络信息
            // parseNet(line, gra);
            int a = 1;
        } 
        else if ( line.substr(0, 10) == "assignment" ){
            // 解析赋值信息
            // parseAssignment(line, gra);
            int a = 1;
        } 
        else if ( line.substr(0,8) == "instance" ){
            // 解析实例化信息
            // parseInstance(line, gra, sub_map);
            int a = 1;
        } 
        else{
            std::cerr << "Unknown line token: " << line << std::endl;
        }

    }

    file.close();
    return gra;
}