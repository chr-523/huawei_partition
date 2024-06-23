#pragma once

#include <string>
#include <regex>
#include <iostream>
#include <fstream>
#include <sstream>

#include "edge.h"
#include "mymodule.h"
#include "instance.h"
#include "vertex.h"

struct Graph_data{
    Module module;
    std::unordered_map<Name_type, Module*> submodule_map;
    Graph_data(
        Module gra, std::unordered_map<Name_type, Module*> s_m):
            module(gra), submodule_map(s_m){}
};

bool is_clk_module(const Name_type& module_name);
bool is_instance_type(const Name_type& instance_name);
bool is_ins_test(const Name_type& instance_name);

Graph_data read_file(const std::string& output_path);
Graph_data read_file_1(const std::string& output_path);
Graph read_graph_data(const Graph_data& gra_data_);
Graph read_graph_data(const Module& gra_data);
