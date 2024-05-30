#ifndef VERILOGPARSER_DATABASE_H
#define VERILOGPARSER_DATABASE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <limits>
//#include "ObjArrayList.h"
#include <map>
#include <unordered_map>
/// namespace for VerilogParser
namespace VerilogParser {

enum PinType
{
    kINPUT = 0x1, 
    kOUTPUT = 0x2,
    kREG = 0x4
};

struct Range 
{
    int low; ///< low value, min infinity if not specified
    int high; ///< high value, min infinity if not specified 
    /// @brief constructor 
    Range() {low = high = std::numeric_limits<int>::min();}
    Range(int l, int h) : low(l), high(h) {}
};

struct GeneralName
{
    std::string name;
    Range range; 
    GeneralName(std::string const& n = "") {name = n; range.low = range.high = std::numeric_limits<int>::min();}
    GeneralName(int n = 0) {name = std::to_string(n); range.low = range.high = std::numeric_limits<int>::min();}
    GeneralName(std::string const& n, int low, int high) {name = n; range.low = low; range.high = high;}
};


 
struct NetPin
{
    std::string net; ///< net name, reserved names VerilogParser::CONSTANT_NET, VerilogParser::GROUP_NETS 
    std::string pin; ///< pin name 
    Range range; ///< range of net 
    union Extension {
        int constant; ///< constant value if the net is a constant 
        std::vector<GeneralName>* vNetName; ///< a group of net names if the net is a group of nets 
    } extension; ///< extension to handle a net with constant values or a regular net 

    NetPin(std::string& n, std::string& p, Range const& r = Range())
    {
        // std::cout << "constructor" << std::endl; //test
        net.swap(n);
        pin.swap(p);
        range = r;
        // std::cout << net << " " << pin << " " << range.low << " " << range.high << std::endl; //test
    }

    /// @param n net name; it will be VerilogParser::CONSTANT_NET if the net is actually a value 
    /// @param p pin name 
    /// @param c constant value only valid if the net is a VerilogParser::CONSTANT_NET
   NetPin(std::string& n, std::string& p, Range const& r, int c)
    {
        net.swap(n);
        pin.swap(p);
        range = r;
        extension.constant = c; 
    }
    /// @param vNetName group of nets only valid if the net is a VerilogParser::GROUP_NETS
   NetPin(std::string& n, std::string& p, std::vector<GeneralName>& vNetName)
    {
        net.swap(n);
        pin.swap(p);
        range = Range();
        extension.vNetName = new std::vector<GeneralName>();
        extension.vNetName->swap(vNetName); 
    }

    NetPin(NetPin const& rhs)
    {
        // std::cout << "copy constructor 1" << std::endl; //test
        copy(rhs);  // There is no copy of range in this copy function.
                    // Only copy _low_ and _high_.
        // std::cout << net << " " << pin << " " << range.low << " " << range.high << std::endl; 
    }
    NetPin& operator=(NetPin const& rhs)
    {
        // std::cout << "copy constructor 2" << std::endl; //test
        if (this != &rhs)
            copy(rhs);
        return *this;
    }
    /// @brief destructor
    ~NetPin()
    {
        if (net == "VerilogParser::GROUP_NETS")
        {
            delete extension.vNetName; 
        }
    }

    /// @brief copy function 
    /// @param rhs right hand side 
    void copy(NetPin const& rhs)
    {
        if (net == "VerilogParser::GROUP_NETS")
        {
            delete extension.vNetName; 
        }
        net = rhs.net; 
        pin = rhs.pin; 
        if (net == "VerilogParser::CONSTANT_NET")
        {
            extension.constant = rhs.extension.constant; 
        }
        else if (net == "VerilogParser::GROUP_NETS")
        {
            extension.vNetName = new std::vector<GeneralName> (*rhs.extension.vNetName);
        }
        range = rhs.range;//1111
    }
};

// bison does not support vector very well, so here create a dummy class for string array. 
class StringArray : public std::vector<std::string>
{
	public: 
        /// @nowarn 
		typedef std::vector<std::string> base_type;
		using base_type::size_type;
		using base_type::value_type;
		using base_type::allocator_type;
        /// @endnowarn

		StringArray(const allocator_type& alloc = allocator_type())
			: base_type(alloc) {}
        /// @brief constructor 
		StringArray(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
			: base_type(n, val, alloc) {}
};


class GeneralNameArray : public std::vector<GeneralName>
{
	public: 
        /// @nowarn 
		typedef std::vector<GeneralName> base_type;
		using base_type::size_type;
		using base_type::value_type;
		using base_type::allocator_type;
        /// @endnowarn

        /// @brief constructor 
        /// @param alloc memory allocator 
		GeneralNameArray(const allocator_type& alloc = allocator_type())
			: base_type(alloc) {}
        /// @brief constructor 
		GeneralNameArray(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
			: base_type(n, val, alloc) {}
};


/// @class VerilogParser::VerilogDataBase
/// @brief Base class for verilog database.  
class VerilogDataBase
{
	public:

        virtual void verilog_module_declaration_cbk(std::string const& module_name, std::vector<GeneralName> const& vPinName); 

        virtual void verilog_instance_cbk(std::string const& macro_name, std::string const& inst_name, std::vector<NetPin> const& vNetPin) = 0;

        virtual void verilog_net_declare_cbk(std::string const& net_name, Range const& range) = 0;

        virtual void verilog_pin_declare_cbk(std::string const& pin_name, unsigned type, Range const& range) = 0;

        virtual void verilog_modules_cbk() = 0;

        // virtual void verilog_assignment_cbk(std::string const& target_name, std::string const& source_name); 
        virtual void verilog_assignment_cbk(std::string const& target_name, Range const& target_range, std::string const& source_name, Range const& source_range) = 0;

    protected:
        /// @brief remind users to define some optional callback functions at runtime 
        /// @param str message including the information to the callback function in the reminder 
        void verilog_user_cbk_reminder(const char* str) const; 
};

} // namespace VerilogParser

#endif
