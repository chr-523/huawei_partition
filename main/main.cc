#include "main.h"


class VerilogDataBase : public VerilogParser::VerilogDataBase
{
// std::ofstream outfile("/home/wy/contest/Extract_fanout/test/output/output.txt", std::ios::out);
	private:
    std::ofstream outfile;
    public:
    VerilogDataBase(){
        outfile.open("/home/chr_523/Work_all/huawei_partition/main/test/output.txt", std::ios::out);
        if (!outfile.is_open()) {
            std::cout << "Failed to open the output file!" << std::endl;
        }
		outfile << "VerilogDataBase::" << __func__ << std::endl;
	}

    virtual void verilog_module_declaration_cbk(std::string const& module_name, std::vector<VerilogParser::GeneralName> const& vPinName)
    {
         outfile << "module" << " " << module_name ;
        // for (std::vector<VerilogParser::GeneralName>::const_iterator it = vPinName.begin(); it != vPinName.end(); ++it)
        //      outfile << "\t" << it->name;
         outfile << std::endl; 
        
    }

    virtual void verilog_instance_cbk(std::string const& macro_name, std::string const& inst_name, std::vector<VerilogParser::NetPin> const& vNetPin)
        {
			 outfile << "instance" << "(" << macro_name << ")(" << inst_name << ")";  
            for (std::vector<VerilogParser::NetPin>::const_iterator it = vNetPin.begin(); it != vNetPin.end(); ++it)
            {
                int low,high;
                if (it->net == "VerilogParser::CONSTANT_NET")
                {
                     outfile << "(" << it->pin << ")(" << it->net << " " << it->extension.constant << ")";
                     // It seems that there is no such situation in c906
                }
                else if (it->net == "VerilogParser::GROUP_NETS")
                {

                     outfile << "(" << it->pin << ")(";
                    for (std::vector<VerilogParser::GeneralName>::const_iterator itn = it->extension.vNetName->begin(); itn != it->extension.vNetName->end(); ++itn)
                    {
                        if(itn->range.low < 0) {low = -1;}
                        else{low = itn->range.low;}
                        if(itn->range.high < 0) {high = -1;}
                        else{high = itn->range.high;}
                         outfile << "{" <<itn->name << " " ;
                         outfile << low << " " ;
                         outfile << high << "}";
                    }
                     outfile << ")";
                }
                else // it->net == NORMAL net
                {
                    if(it->range.low < 0) {low = -1;}
                    else{low = it->range.low;}
                    if(it->range.high < 0) {high = -1;}
                    else{high = it->range.high;}
                     outfile << "(" << it->pin << ")(" << it->net << "";
                     outfile << " " << low <<" "<< high <<")";
                }
            }
             outfile << std::endl;
        }
    
    virtual void verilog_pin_declare_cbk(std::string const& pin_name, unsigned type, VerilogParser::Range const& range)
    {       
        // //  outfile << "pin" << " => " << pin_name << " " << type << endl;
        //  if ( range.low < 0 ){
        //     // outfile << "pin" << " => " << pin_name << " " << type << endl;
        //     outfile << "pin" << " => " << pin_name << "->[-1:-1] =>" << type << std::endl;
        //  }
        //  else{
        //     outfile << "pin" << " => " << pin_name << "->[" << range.low << ":" << range.high << "] =>" << type << std::endl;
        //  }
        int low,high;
        if(range.low < 0) {low = -1;}
        else{low = range.low;}
        if(range.high < 0) {high = -1;}
        else{high = range.high;}
        outfile << "pin" << "(" << pin_name << " " << low << " " << high << ")";
   
        outfile << "(" << type << ")" << std::endl;
         
    }

    virtual void verilog_net_declare_cbk(std::string const& net_name, VerilogParser::Range const& range){
        //  outfile << "net" << " => " << net_name << endl;         
        //  if ( range.low < 0 ){
        //     outfile << "net" << " " << net_name << "-1 -1"  << std::endl;
        //  }
        //  else{
        int low,high;
        if(range.low < 0) {low = -1;}
        else{low = range.low;}
        if(range.high < 0) {high = -1;}
        else{high = range.high;}
            
            outfile << "net" << "(" << net_name << " " << low << " " << high << ")" ;
            outfile << "(0)" << std::endl;
        //  }
    }

    virtual void verilog_assignment_cbk(std::string const& target_name, VerilogParser::Range const& target_range, std::string const& source_name, VerilogParser::Range const& source_range)
    {
        // Here, all range are printed. A "if(range<0)" should be add.
        int t_low,t_high;
        if(target_range.low < 0) {t_low = -1;}
        else{t_low = target_range.low;}
        if(target_range.high < 0) {t_high = -1;}
        else{t_high = target_range.high;}
        int low,high;
        if(source_range.low < 0) {low = -1;}
        else{low = source_range.low;}
        if(source_range.high < 0) {high = -1;}
        else{high = source_range.high;}
         outfile << "assignment" << "(" << target_name << " " << t_low << " " << t_high  << ")";
         outfile <<  "(" << source_name<< " " << low << " " << high << ")" << std::endl;

    }

    virtual void verilog_modules_cbk(){}
    
    ~VerilogDataBase(){
        if( outfile.is_open() ){
            outfile.close();
        }
    }

};

void test1(std::string const& filename)
{
    VerilogDataBase db;
    VerilogParser::Driver driver (db);
    driver.parse_file(filename);
    // if(0){ //test
    //     int t_low = 0;
    //     int t_high = 2;
    //     Name_type pin_name = "pin1";
    //     Edge_type t_type = INPUT;
    //     (*gra).add_edge(pin_name, t_low, t_high, t_type); // & transmit

    //     Name_type mod_name = "mod1";
    //     std::pair<Name_type, Module*> t_submod = std::make_pair(mod_name,gra);
    //     (*sub_map).insert(t_submod);
    // }
}
 
int main(int argc, char** argv)
{    
    bool updata_output = true;

    if(updata_output){
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
    // the previous code is to generate/updata the output.txt
    
    std::string file_path = "test/output.txt";

    // Module C_906("test_name",default_module_weight);
    // std::unordered_map< Name_type, Module* > sub_map;
    // Graph_data temp_data_c906(C_906, sub_map);
    // Graph_data temp_data(C_906, sub_map);

    //old read_file_function
    Graph_data temp_data_c906 = read_file_1(file_path);
    //new read -> to be done
    Graph_data temp_data = read_file(file_path);
    
        Module temp_gra = temp_data_c906.module;

    // Graph gra = read_graph_data(temp_data_c906);
    Graph gra = read_graph_data(temp_data_c906.module);

    int a = 1; // IO degree is the last thing!!!!!!

        // +sub_module map
    // split sub module -> level/weight -> Algorithm 1 
        // many level-split graphs (STA Graphs)
    // Algorithm 2  change to CSR (Endpoint Graphs)
    // kahypar


	return 0;
}

