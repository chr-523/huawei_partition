#include "VerilogDriver.h"
#include "VerilogScanner.h"

namespace VerilogParser {

bool Driver::parse_stream_1(std::istream& in, const std::string& sname,
            Graph* gra,
            std::unordered_map<Name_type,Graph*>* sub_map)
{
    streamname = sname;
    std::cout <<"streamnameFileName:" << streamname << std::endl;

    Scanner scanner(&in);
    scanner.set_debug(trace_scanning);
    this->lexer = &scanner;

    if(0){ //test
        int t_low = 0;
        int t_high = 2;
        Name_type pin_name = "pin1";
        Edge_type t_type = INPUT;
        (*gra).add_edge(pin_name, t_low, t_high, t_type); // & transmit

        Name_type mod_name = "mod1";
        std::pair<Name_type, Graph*> t_submod = std::make_pair(mod_name,gra);
        (*sub_map).insert(t_submod);
    }

    Parser parser(*this);
    parser.set_debug_level(trace_parsing);
    return (parser.parse() == 0);
}

bool Driver::parse_file_1(const std::string &filename, Graph* gra,
        std::unordered_map<Name_type,Graph*>* sub_map)
{
    std::ifstream in(filename.c_str());
    if(0){ //test
        int t_low = 0;
        int t_high = 2;
        Name_type pin_name = "pin1";
        Edge_type t_type = INPUT;
        (*gra).add_edge(pin_name, t_low, t_high, t_type); // & transmit

        Name_type mod_name = "mod1";
        std::pair<Name_type, Graph*> t_submod = std::make_pair(mod_name,gra);
        (*sub_map).insert(t_submod);
    }

    if (!in.good()) return false;
    return parse_stream_1(in, filename, gra, sub_map);
}


void Driver::module_name_cbk(std::string const& module_name, std::vector<GeneralName> const& vPinName,
            Graph* gra, std::unordered_map<Name_type,Graph*>* sub_map)
{   
    if(1){ //test
        int t_low = 0;
        int t_high = 2;
        Name_type pin_name = "pin1";
        Edge_type t_type = INPUT;
        (*gra).add_edge(pin_name, t_low, t_high, t_type); // & transmit

        Name_type mod_name = "mod1";
        std::pair<Name_type, Graph*> t_submod = std::make_pair(mod_name,gra);
        (*sub_map).insert(t_submod);
    }
    m_db.verilog_module_declaration_cbk(module_name, vPinName);
}

void Driver::module_name_cbk(std::string const& module_name, std::vector<GeneralName> const& vInPinName, std::vector<GeneralName> const& vOutPinName,
            Graph* gra, std::unordered_map<Name_type,Graph*>* sub_map)
{
    std::vector<GeneralName> vPinName (vInPinName); 
    vPinName.insert(vPinName.end(), vOutPinName.begin(), vOutPinName.end()); 
    if(1){ //test
        int t_low = 0;
        int t_high = 2;
        Name_type pin_name = "pin1";
        Edge_type t_type = INPUT;
        (*gra).add_edge(pin_name, t_low, t_high, t_type); // & transmit

        Name_type mod_name = "mod1";
        std::pair<Name_type, Graph*> t_submod = std::make_pair(mod_name,gra);
        (*sub_map).insert(t_submod);
    }
    this->module_name_cbk(module_name, vPinName); 
    this->pin_declare_cbk(vInPinName, kINPUT);
    this->pin_declare_cbk(vOutPinName, kOUTPUT);
}

/*
void Driver::module_instance_cbk(std::string const& macro_name, std::string const& inst_name) 
{

	m_db.verilog_instance_cbk(macro_name, inst_name, m_vNetPin);
	m_vNetPin.clear();
}

void Driver::wire_pin_cbk(std::string& net_name, std::string& pin_name, Range const& range)
{
    // std::cout << net_name << " " << pin_name << " " << range.low << " " << range.high << std::endl; 
	// m_vNetPin.emplace_back(net_name, pin_name, range);
    // std::cout << "emplace_back" << std::endl; //test
    m_vNetPin.push_back(NetPin(net_name, pin_name, range));
    // std::cout << m_vNetPin.back().net << " " << m_vNetPin.back().pin << " " << m_vNetPin.back().range.low << " " << m_vNetPin.back().range.high << std::endl; 

}

void Driver::wire_pin_cbk(int bits, int value, std::string& pin_name)
{
    std::string net_name = "VerilogParser::CONSTANT_NET";
	m_vNetPin.push_back(NetPin(net_name, pin_name, Range(0, bits), value));
}

void Driver::wire_pin_cbk(std::vector<GeneralName>& vNetName, std::string& pin_name)
{
    std::string net_name = "VerilogParser::GROUP_NETS";
	m_vNetPin.push_back(NetPin(net_name, pin_name, vNetName));
}

void Driver::wire_declare_cbk(std::vector<GeneralName> const& vNetName, Range const& range)
{
    for (std::vector<GeneralName>::const_iterator it = vNetName.begin(); it != vNetName.end(); ++it)
    {
        if (it->range.low != std::numeric_limits<int>::min() || it->range.high != std::numeric_limits<int>::min())
            std::cerr << "warning: multiple definitions of ranges " << it->name << std::endl;
        m_db.verilog_net_declare_cbk(it->name, range);
    }
}

void Driver::wire_declare_cbk(std::vector<GeneralName> const& vNetName)
{
    for (std::vector<GeneralName>::const_iterator it = vNetName.begin(); it != vNetName.end(); ++it)
        m_db.verilog_net_declare_cbk(it->name, it->range);
}

void Driver::pin_declare_cbk(std::vector<GeneralName> const& vPinName, unsigned type)
{
    for (std::vector<GeneralName>::const_iterator it = vPinName.begin(); it != vPinName.end(); ++it)
        m_db.verilog_pin_declare_cbk(it->name, type, it->range);
}

void Driver::pin_declare_cbk(std::vector<GeneralName> const& vPinName, unsigned type, Range const& range)
{
    for (std::vector<GeneralName>::const_iterator it = vPinName.begin(); it != vPinName.end(); ++it)
    {
        if (it->range.low != std::numeric_limits<int>::min() || it->range.high != std::numeric_limits<int>::min())
            std::cerr << "warning: multiple definitions of ranges " << it->name << std::endl;
        m_db.verilog_pin_declare_cbk(it->name, type, range);
    }
}

void Driver::assignment_cbk(std::string const& target_name, Range const& target_range, std::string const& source_name, Range const& source_range) 
{
    m_db.verilog_assignment_cbk(target_name, target_range, source_name, source_range);
}

void Driver::assignment_cbk(std::string const& target_name, Range const& target_range, int bits, int value) 
{
    m_db.verilog_assignment_cbk(target_name, target_range,  std::to_string(value), Range(0, bits));
}

void Driver::modules_cbk(){
    m_db.verilog_modules_cbk();
}
*/




}

