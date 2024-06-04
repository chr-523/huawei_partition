
#include "process.h"

bool is_clk_module(Name_type& module_name){
    Name_type instance_prefix = "gated_clk_";
    Name_type name_prefix = module_name.substr(0, instance_prefix.length() );
    return name_prefix == instance_prefix;
}
