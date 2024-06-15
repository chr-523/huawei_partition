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
                         outfile << "{" <<itn->name << " " ;
                         outfile << itn->range.low << " " ;
                         outfile << itn->range.high << "}";
                    }
                     outfile << ")";
                }
                else // it->net == NORMAL net
                {
                     outfile << "(" << it->pin << ")(" << it->net << "";
                     outfile << " " << it->range.low<<" "<<it->range.high<<")";
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
        outfile << "pin" << "(" << pin_name << " " << range.low << " " << range.high << ")";
   
        outfile << "(" << type << ")" << std::endl;
         
    }

    virtual void verilog_net_declare_cbk(std::string const& net_name, VerilogParser::Range const& range){
        //  outfile << "net" << " => " << net_name << endl;         
        //  if ( range.low < 0 ){
        //     outfile << "net" << " " << net_name << "-1 -1"  << std::endl;
        //  }
        //  else{
            outfile << "net" << "(" << net_name << " " << range.low << " " << range.high << ")" ;
            outfile << "(0)" << std::endl;
        //  }
    }

    virtual void verilog_assignment_cbk(std::string const& target_name, VerilogParser::Range const& target_range, std::string const& source_name, VerilogParser::Range const& source_range)
    {
        // Here, all range are printed. A "if(range<0)" should be add.
         outfile << "assignment" << "(" << target_name << " " << target_range.low << " " << target_range.high  << ")";
         outfile <<  "(" << source_name<< " " << source_range.low << " " << source_range.high << ")" << std::endl;


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
    //     std::pair<Name_type, Graph*> t_submod = std::make_pair(mod_name,gra);
    //     (*sub_map).insert(t_submod);
    // }
}
 
int main(int argc, char** argv)
{    
    if(1){
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
    Graph C_906("test_name",default_module_weight);

    std::unordered_map< Name_type, Graph* > sub_map;
	// test1(path, &C_906, &sub_map);
    C_906 = read_file(file_path);

    if(0){ // test
        std::cout << " 1 " << std::endl;
        auto it = sub_map.find("mod1");
        if (it != sub_map.end() ){
            std::cout << it -> second -> get_module_name() << std::endl; // it->second means Graph*
        } else{
            std::cout << "Not found" << std::endl;
        }
    }

	return 0;
}

