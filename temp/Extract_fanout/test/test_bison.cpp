#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <string>
#include <algorithm>
#include <parser/bison/VerilogDriver.h>
#include <boost/regex.hpp>
#include <unordered_map>
using std::cout;
using std::cin;
using std::endl;
using std::string;

// std::unordered_map<string, string> output_pin;

// std::ofstream outfile("/home/wy/contest/Extract_fanout/test/output/output.txt", std::ios::out);

class VerilogDataBase : public VerilogParser::VerilogDataBase
{
// std::ofstream outfile("/home/wy/contest/Extract_fanout/test/output/output.txt", std::ios::out);
	private:
    std::ofstream outfile;
    public:
    VerilogDataBase(){
        outfile.open("/home/chr_523/Work_all/huawei_partition/temp/Extract_fanout/test/output/output.txt", std::ios::out);
        if (!outfile.is_open()) {
            std::cout << "Failed to open the output file!" << std::endl;
        }
		outfile << "VerilogDataBase::" << __func__ << endl;
	}

    virtual void verilog_module_declaration_cbk(std::string const& module_name, std::vector<VerilogParser::GeneralName> const& vPinName)
    {
         outfile << "module" << " => " << module_name << "\n";
        for (std::vector<VerilogParser::GeneralName>::const_iterator it = vPinName.begin(); it != vPinName.end(); ++it)
             outfile << "\t" << it->name;
         outfile << endl; 
    }

    virtual void verilog_instance_cbk(std::string const& macro_name, std::string const& inst_name, std::vector<VerilogParser::NetPin> const& vNetPin)
        {
			 outfile << "instance" << " => " << macro_name << ", " << inst_name << ", ";  
            for (std::vector<VerilogParser::NetPin>::const_iterator it = vNetPin.begin(); it != vNetPin.end(); ++it)
            {
                if (it->net == "VerilogParser::CONSTANT_NET")
                {
                     outfile << it->pin << "(" << it->net << " " << it->extension.constant << ")";
                }
                else if (it->net == "VerilogParser::GROUP_NETS")
                {
                     outfile << it->pin << "(" << it->net << " {";
                    for (std::vector<VerilogParser::GeneralName>::const_iterator itn = it->extension.vNetName->begin(); itn != it->extension.vNetName->end(); ++itn)
                    {
                         outfile << "(" << itn->name << ")" ;
                    }
                     outfile << "} " << ")";
                }
                else 
                {
                     outfile << it->pin << "(" << it->net << ")";
                }
            }
             outfile << endl;
        }
    
    virtual void verilog_pin_declare_cbk(std::string const& pin_name, unsigned type, VerilogParser::Range const& range)
    {
         outfile << "pin" << " => " << pin_name << " " << type << endl;
    }
    
    virtual void verilog_assignment_cbk(std::string const& target_name, VerilogParser::Range const& target_range, std::string const& source_name, VerilogParser::Range const& source_range)
    {
         outfile << "assigment" << " => " << target_name << " => " << source_name  <<  endl;
    }

    virtual void verilog_net_declare_cbk(std::string const& net_name, VerilogParser::Range const& range){
         outfile << "net" << " => " << net_name << endl;
    }

    virtual void verilog_modules_cbk(){
        
    }
    
    ~VerilogDataBase() {
        if (outfile.is_open()) {
            outfile.close();
        }
    }

};



void test1(string const& filename, string filename1)
{
	VerilogDataBase db;
	VerilogParser::Driver driver (db);
	driver.parse_file(filename);
    
}

int main(int argc, char** argv)
{    
    std::ifstream infile;
    std::string path = argv[1]; 
    boost::regex re("[^/]*$");
    boost::smatch match;
    boost::regex_search(path, match, re);
    std::string filename = match.str();
    infile.open(path, std::ios::in);
	test1(path,filename);
    infile.close();
	return 0;
}
