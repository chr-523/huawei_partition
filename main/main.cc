#include "main.h"


int main(int argc, char** argv)
{    
    bool generate_new_output = true;
    if(generate_new_output){
        std::ifstream infile;
        std::string path = argv[1]; 
        boost::regex re("[^/]*$");
        boost::smatch match;
        boost::regex_search(path, match, re);
        std::string filename = match.str();
        infile.open(path, std::ios::in);
        test1(path);
        infile.close();
    }

    std::string file_path = "test/output.txt";
    // std::unordered_map<>;
    // std::unordered_map<>;
    // std::unordered_map<>;
    // std::unordered_map<>;
    // C_906 = read_file_1(file_path);
    
    Graph_data C_906 = read_file(file_path);

    // read_file_time_test(file_path);
    int a=1;
        // +sub_module map
    // split sub module -> level/weight -> Algorithm 1 
        // many level-split graphs (STA Graphs)
    // Algorithm 2  change to CSR (Endpoint Graphs)
    // kahypar


	return 0;
}

