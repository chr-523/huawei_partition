#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <iostream>

#include "mymodule.h"

using weight_type = float;
using degree_type = size_t;

class Vertex
{
public:
    // name,clk -> other settings need to be set later
    Vertex( const Name_type& name = default_name, const bool& is_clk = false):
        name(name), is_clk(is_clk)  {
            this -> weight = 1.0;
            this -> outDegree = 0;
            this -> inDegree = 0;
            this -> comb_rank = -1;
            this -> reg_rank = -1;
            this -> reg_temp = -1;
    }

    // name,clk,w_v,out,int
    Vertex( const Vertex& other):
            name(other.name), is_clk(other.is_clk), weight(other.weight),
            outDegree(other.outDegree), inDegree(other.inDegree),
            comb_rank(other.comb_rank),reg_rank(other.reg_rank),reg_temp(other.reg_temp){};  

    Vertex( const Name_type& name, const bool& is_clk, const weight_type& weight,
        const degree_type& outDegree, const degree_type& inDegree):
        name(name), is_clk(is_clk),
        weight(weight),outDegree(outDegree), inDegree(inDegree){
        this -> comb_rank = -1;
        this -> reg_rank = -1;
        this -> reg_temp = -1;
    };
    ~Vertex(){};
public:
    Name_type get_name() const { return name; }
    weight_type get_weight() const { return weight; }
    degree_type get_outDgree() const { return outDegree; }
        void add_outDgree(){ outDegree++ ; }
        void set_outDgree( int O_ ){ outDegree = O_ ; }
    degree_type get_inDgree() const { return inDegree; }
        void add_inDgree(){ inDegree++ ; }
        void set_inDgree( int I_ ){ inDegree = I_ ; }
    bool get_is_clk() const { return is_clk; }
    int get_comb_rank() const { return comb_rank; }
    int get_reg_rank() const { return reg_rank; }
    int get_reg_temp() const { return reg_temp; }
private:
    Name_type name;
    weight_type weight;
    degree_type outDegree;
    degree_type inDegree;
    
    bool is_clk;
    // Required by subsequent processes
    int comb_rank;
    // Required by subsequent processes
    int reg_rank;
    // Required by subsequent processes
    int reg_temp;
};

struct Graph{
    // vertex's name, the data of vertex (use*)
    std::unordered_map< Name_type, Vertex* > vertexs;
    size_t rec_level;
    
    std::unordered_map< Name_type, 
        std::pair< Edge_index_type, Edge_type >> pin_outedge_temp; 
    
    std::unordered_map< Edge_index_type, 
                                Module_index_type> outedge_pin_temp; 

    std::unordered_map< Name_type, 
        std::vector< std::tuple< Name_type, Edge >>> pin_ins_edge_temp;
    std::unordered_map< Name_type, 
        std::vector< std::tuple< Name_type, Edge >>> ins_pin_edge_temp;
        
    // map of : vertexs' name and IO vector< adj_name, edge_weight >
    std::unordered_map< Name_type, std::vector< std::pair< Name_type, weight_type >>> graph_adjlist_plus;
    std::unordered_map< Name_type, std::vector< std::pair< Name_type, weight_type >>> graph_adjlist_minus;
    
    // std::unordered_map<Name_type,std::tuple<Vertex*, graph+,graph->>

    Graph(){};

    ~Graph(){
        for (auto& pair : vertexs) {
            delete pair.second;
        }
        vertexs.clear();
    }

    void addVertex(const Vertex& v_){
        // Vertex* newVertex = new Vertex(v_);
        // Name_type name =  v_.get_name();
        // vertexs[name] = newVertex;
        vertexs[v_.get_name()] = new Vertex(v_);
    }

    // void add_ins_pin_temp(const Name_type& ins_name, 
    //         const std::vector<std::tuple<Name_type,Edge_type, weight_type>>& pin_info){
    //     pin_temp[ins_name] = pin_info;
    // }
    void clear_temp_1(){
        ins_pin_edge_temp.clear();
        pin_ins_edge_temp.clear();
    }
    
    void clear_temp_2(){
        pin_outedge_temp.clear();
        outedge_pin_temp.clear();
    }

    void add_ins_pin_edge_temp(const Name_type& ins_name, 
            const std::vector<std::tuple<Name_type, Edge>>& edge_pin_info){
        ins_pin_edge_temp[ins_name] = edge_pin_info;
    
    }

    void add_pin_outedge_temp(const Name_type& pin_name, 
            const std::pair< Edge_index_type, Edge_type> out_edge){
        pin_outedge_temp[pin_name] = out_edge;
    }

    void add_outedge_pin_temp(const Name_type& out_edge, 
            const Module_index_type& pin_name){
        outedge_pin_temp[out_edge] = pin_name;
    }

    void add_pin_ins_edge_temp(const Name_type& pin_name, 
            const std::vector<std::tuple<Name_type, Edge>>& ins_edge_info){
        pin_ins_edge_temp[pin_name] = ins_edge_info;
    
    }



    void add_adj_minus(const Name_type& vertex_name, 
            const std::vector<std::pair<Name_type, weight_type>>& adj_info ){
                        // 检查键是否存在
        auto it = graph_adjlist_minus.find(vertex_name);
        if (it != graph_adjlist_minus.end()) {
            // 如果存在，将新的邻接信息添加到现有的向量中
            it->second.insert(it->second.end(), adj_info.begin(), adj_info.end());
        } else {
            // 如果不存在，创建新的条目
            graph_adjlist_minus[vertex_name] = adj_info;
        }
    }    
    void add_adj_minus(const Name_type& vertex_name, 
            const std::pair<Name_type, weight_type>& single_adj_info) {
            // 检查键是否存在
        auto it = graph_adjlist_minus.find(vertex_name);
        std::vector<std::pair<Name_type, weight_type>> adj_list;
        if (it != graph_adjlist_minus.end()) {
            // 如果存在，获取现有的向量
            adj_list = it->second;
        } 
        // 创建向量并添加单个邻接信息
        adj_list.push_back(single_adj_info);

        // 更新graph_adjlist_plus中的向量
        graph_adjlist_minus[vertex_name] = adj_list;
    }
 
    void add_adj_plus(const Name_type& vertex_name, 
            const std::vector<std::pair<Name_type, weight_type>>& adj_info ){
            // 检查键是否存在
        auto it = graph_adjlist_plus.find(vertex_name);
        if (it != graph_adjlist_plus.end()){
            // 如果存在，将新的邻接信息添加到现有的向量中
            it->second.insert(it->second.end(), adj_info.begin(), adj_info.end());
        } 
        else{
        // 如果不存在，创建新的条目
        graph_adjlist_plus[vertex_name] = adj_info;
        }
    }
    void add_adj_plus(const Name_type& vertex_name, 
            const std::pair<Name_type, weight_type>& single_adj_info) {
            // 检查键是否存在
        auto it = graph_adjlist_plus.find(vertex_name);
        std::vector<std::pair<Name_type, weight_type>> adj_list;
        if (it != graph_adjlist_plus.end()) {
            // 如果存在，获取现有的向量
            adj_list = it->second;
        } 
        // 创建向量并添加单个邻接信息
        adj_list.push_back(single_adj_info);

        // 更新graph_adjlist_plus中的向量
        graph_adjlist_plus[vertex_name] = adj_list;
    }
    

    void read_graph_data(const Module& gra_data, Name_type& prefix, Name_type& mod_index);
};

