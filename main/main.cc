#include "main.h"

int main(int argc, char** argv)
{    
    bool generate_new_output = true;
    if(generate_new_output){
        std::ifstream infile;
            // argv[0] = /home/chr_523/Work_all/huawei_partition/main/build/test_bison
            // argv[1] = ./test/benchmarks/for_test.v
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
    
    Graph_data C906_data = read_file(file_path);

    Graph gra = module_to_graph_old(C906_data);

    int a=1;

	return 0;
}

