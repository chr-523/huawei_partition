#pragma once
#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

#include "edge.h"
#include "mymodule.h"
#include "instance.h"

struct Graph_data{
    Module module;
    std::unordered_map<Name_type, Module*> submodule_map;
    Graph_data(
        Module gra, std::unordered_map<Name_type, Module*> s_m):
            module(gra), submodule_map(s_m) {}
};

bool is_clk_module(const Name_type& module_name);

bool is_instance_type(const Name_type& instance_name);
bool is_ins_test(const Name_type& instance_name);

Graph_data read_file(const std::string& output_path);
Module read_file_1(const std::string& output_path);
// // If sub_map is useful, the above function needs to be changed to the following function
// std::tuple< Module, std::unordered_map< Name_type, Module* >> read_file_plus(){
//  
//     }

