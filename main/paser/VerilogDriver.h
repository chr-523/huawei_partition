#include "mymodule.h"
#ifndef VERILOGPARSER_DRIVER_H
#define VERILOGPARSER_DRIVER_H

#include "VerilogDataBase.h"
#include "mymodule.h"

namespace VerilogParser {

using std::cout;
using std::endl;
using std::cerr;
using std::string; 
using std::vector;
using std::pair;
using std::make_pair;
using std::ostringstream;

/** 
 * @class VerilogParser::Driver
 * The Driver class brings together all components. It creates an instance of
 * the Parser and Scanner classes and connects them. Then the input stream is
 * fed into the scanner object and the parser gets it's token
 * sequence. Furthermore the driver object is available in the grammar rules as
 * a parameter. Therefore the driver class contains a reference to the
 * structure into which the parsed data is saved. */
class Driver
{
public:
    Driver(VerilogDataBase& db);

    /// enable debug output in the flex scanner
    bool trace_scanning;

    /// enable debug output in the bison parser
    bool trace_parsing;

    /// stream name (file or input stream) used for error messages.
    string streamname;

    bool parse_stream(std::istream& in,
		      const string& sname = "stream input");


    bool parse_string(const string& input,
		      const string& sname = "string stream");

    bool parse_file(const string& filename);

    void error(const class location& l, const string& m);
    void error(const string& m);

    /** Pointer to the current lexer instance, this is used to connect the
     * parser to the scanner. It is used in the yylex macro. */
    class Scanner* lexer;

    /** Reference to the database filled during parsing of the
     * expressions. */
    VerilogDataBase& m_db;

    /// @cond
    void module_name_cbk(std::string const&, std::vector<GeneralName> const&); 
    void module_name_cbk(std::string const&, std::vector<GeneralName> const&, std::vector<GeneralName> const&); 
    void wire_pin_cbk(std::string&, std::string&, Range const& = Range());
    void wire_pin_cbk(int, int, std::string&);
    void wire_pin_cbk(std::vector<GeneralName>&, std::string&);
    void wire_declare_cbk(std::vector<GeneralName> const&, Range const&);
    void wire_declare_cbk(std::vector<GeneralName> const& vNetName);
    void pin_declare_cbk(std::vector<GeneralName> const&, unsigned, Range const&);
    void pin_declare_cbk(std::vector<GeneralName> const&, unsigned);
    void module_instance_cbk(std::string const&, std::string const&);
    void modules_cbk();
    void assignment_cbk(std::string const&, Range const&, std::string const&, Range const&);
    void assignment_cbk(std::string const&, Range const&, int, int); 

//     ///adddd
//     void module_name_cbk(std::string const&, std::vector<GeneralName> const&,
//                 Graph* gra, std::unordered_map<Name_type,Graph*>*sub_map); 
//     void module_name_cbk(std::string const&, std::vector<GeneralName> const&, std::vector<GeneralName> const&,
//                 Graph* gra, std::unordered_map<Name_type,Graph*>*sub_map); 
// //     void wire_pin_cbk(std::string&, std::string&, Range const& = Range());
// //     void wire_pin_cbk(int, int, std::string&);
// //     void wire_pin_cbk(std::vector<GeneralName>&, std::string&);
// //     void wire_declare_cbk(std::vector<GeneralName> const&, Range const&);
// //     void wire_declare_cbk(std::vector<GeneralName> const& vNetName);
// //     void pin_declare_cbk(std::vector<GeneralName> const&, unsigned, Range const&);
// //     void pin_declare_cbk(std::vector<GeneralName> const&, unsigned);
// //     void module_instance_cbk(std::string const&, std::string const&);
// //     void modules_cbk();    //add
// //     void assignment_cbk(std::string const&, Range const&, std::string const&, Range const&);
// //     void assignment_cbk(std::string const&, Range const&, int, int); 
    /// @endcond

protected:
	/// @brief Use as a stack for node and pin pairs in a net,  
	/// because wire_pin_cbk will be called before module_instance_cbk
	vector<NetPin> m_vNetPin;
        
};

bool read(VerilogDataBase& db, const string& verilogFile);

} // namespace example

#endif // EXAMPLE_DRIVER_H
