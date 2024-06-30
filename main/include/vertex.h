#pragma once

#include <map>
#include <list>
#include <queue>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

#include "mymodule.h"
using weight_type = int;
using degree_type = int;


class Vertex
{
public:

    Vertex( const Name_type& name = default_name, const int& index = 0,
            const bool& is_clk = false)://无参数初始化
            name(name), index(index), is_clk(is_clk){
            this -> weight = 1;
            this -> outDegree = 0;
            this -> inDegree = 0;
            this -> comb_rank = 0;
            this -> comb_rank_temp = 0;
            this -> reg_rank = 0;
            this -> reg_temp = 0;
    }

    // name,clk,w_v,out,int
    Vertex( const Vertex& other):
            name(other.name), 
            is_clk(other.is_clk), weight(other.weight),
            index(other.index),
            outDegree(other.outDegree), inDegree(other.inDegree),
            comb_rank(other.comb_rank), comb_rank_temp(other.comb_rank_temp), reg_rank(other.reg_rank), 
            reg_temp(other.reg_temp){};  

    Vertex( const Name_type& name, const bool& is_clk, const weight_type& weight,
        const size_t& index,
        const degree_type& outDegree, const degree_type& inDegree):
        name(name), is_clk(is_clk), weight(weight), 
        index(index),
        outDegree(outDegree), inDegree(inDegree){
        this -> comb_rank = 0;
        this -> comb_rank_temp = 0;
        this -> reg_rank = 0;
        this -> reg_temp = 0;
    };
    ~Vertex(){};
public:/**tu**/
    void add_inDgree(){ inDegree++ ; }
    void set_inDgree( int I_ ){ inDegree = I_ ; }
    void add_outDgree(){ outDegree++ ; }
    void set_outDgree( int O_ ){ outDegree = O_ ; } 

public:

    std::string getName() { return name; };
    bool getisReg() { return is_clk; };

        void pushRegRank(int &temp) { reg_rank = temp; }
    int getRegRank() { return reg_rank; }

        void pushOutDegree(int temp) { outDegree = temp; }
        void pushInDegree(int temp) { inDegree = temp; }
    int getOutDegree() { return outDegree; }
    int getInDegree() { return inDegree; }

        void pushRegTemp(int temp) { reg_temp = temp; }
    int getInRegTemp() { return reg_temp; }

        void pushCombRank(int &temp) { comb_rank = temp; }
    int getCombRank() { return comb_rank; }
        void pushCombRankTemp(int &temp) { comb_rank_temp = temp; }
    int getCombRankTemp() { return comb_rank_temp; }

        void pushWeight(int &temp){weight = temp; }
    int getWeight(){return weight;}


private:
    std::string name;
    int weight = 1; // the weight of vertex
    int index;
    int outDegree;
    int inDegree;

    bool is_clk;        // whether reg or comb
    int comb_rank;      // the rank of combs between regs
    int comb_rank_temp; // is used to classify combs
    int reg_rank = 0;   // the rank of regs
    int reg_temp;       // is used to classify regs
};




struct Graph{
    std::unordered_map< Name_type, Vertex* > vertexs; 
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> GraphAdjList_Plus;
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> GraphAdjList_Minus;
    
    std::string prefix;
    std::string this_submodule_index;
    std::unordered_map<std::string, std::vector<std::pair<Name_type,Name_type>>> ins_to_mod_pin_map;
    std::unordered_map<std::string, std::vector<Name_type>> mod_pin_to_ins_map;
    std::unordered_map<Edge_index_type, std::pair<Edge_type, Range>> empty_IO_map;

    // std::unordered_map<std::string, std::vector<<std::string, int>>>> GraphAdjList_Minus;
    bool is_top;
    // std::unordered_map< std::string >
    Graph(){};
    ~Graph(){
        // for (auto& pair : vertexs) {
        //     delete pair.second;
        // }
        // vertexs.clear();
    }

    /* old_old
    // void addVertex(const Vertex& v_){
    //     // Vertex* newVertex = new Vertex(v_);
    //     // Name_type name =  v_.get_name();
    //     // vertexs[name] = newVertex;
    //     vertexs[v_.get_name()] = new Vertex(v_);
    // }
    //
    // // void add_ins_pin_temp(const Name_type& ins_name, 
    // //         const std::vector<std::tuple<Name_type,Edge_type, weight_type>>& pin_info){
    // //     pin_temp[ins_name] = pin_info;
    // // }
    // void clear_all_temp(){
    //     ins_pin_edge_temp.clear();
    //     pin_ins_edge_temp.clear();
    //     pin_outedge_temp.clear();
    //     outedge_pin_temp.clear();
    // }
    //
    // void add_ins_pin_edge_temp(const Name_type& ins_name, 
    //         const std::vector<std::tuple<Name_type, Edge>>& edge_pin_info){
    //     ins_pin_edge_temp[ins_name] = edge_pin_info;
    // }
    //
    // void add_ins_pin_edge_temp_2(const Name_type& ins_name, 
    //         const std::vector<std::tuple<Name_type, Edge>>& edge_pin_info){
    //     ins_pin_edge_temp_2[ins_name] = edge_pin_info;
    // }    
    // void add_ins_pin_edge_temp_2(const Name_type& ins_name, 
    //         const std::tuple<Name_type, Edge>& edge_pin_info){
    //     auto it = ins_pin_edge_temp_2.find(ins_name);
    //     if (it != ins_pin_edge_temp_2.end()) {
    //         // 如果存在，将单个 tuple 添加到对应的 vector 中
    //         it->second.push_back(edge_pin_info);
    //     } else {
    //         // 如果不存在，创建新的 vector 并将 tuple 添加进去
    //         ins_pin_edge_temp_2[ins_name].push_back(edge_pin_info);
    //     }
    // }
    //
    // void add_pin_outedge_temp(const Name_type& pin_name, 
    //         const std::pair< Edge_index_type, Edge_type> out_edge){
    //     pin_outedge_temp[pin_name] = out_edge;
    // }
    //
    // void add_outedge_pin_temp(const Name_type& out_edge, 
    //         const Module_index_type& pin_name){
    //     outedge_pin_temp[out_edge] = pin_name;
    // }
    //
    // void add_pin_ins_edge_temp(const Name_type& pin_name, 
    //         const std::vector<std::tuple<Name_type, Edge>>& ins_edge_info){
    //     pin_ins_edge_temp[pin_name] = ins_edge_info;
    // }
    //
    // void add_pin_ins_edge_temp_2(const Name_type& pin_name, 
    //         const std::vector<std::tuple<Name_type, Edge>>& ins_edge_info){
    //     pin_ins_edge_temp_2[pin_name] = ins_edge_info;
    //
    // }
    //
    // void add_pin_ins_edge_temp_2(const Name_type& pin_name, 
    //         const std::tuple<Name_type, Edge>& ins_edge_info){
    //
    //     auto it = pin_ins_edge_temp_2.find(pin_name);
    //     if (it != pin_ins_edge_temp_2.end()) {
    //         // 如果存在，将单个 tuple 添加到对应的 vector 中
    // 
    //         it->second.push_back(ins_edge_info);
    //     } else {
    //         // 如果不存在，创建新的 vector 并将 tuple 添加进去
    //         pin_ins_edge_temp_2[pin_name].push_back(ins_edge_info);
    //     }
    // }
    //
    //
    // void add_adj_minus(const Name_type& vertex_name, 
    //         const std::vector<std::pair<Name_type, weight_type>>& adj_info ){
    //                     // 检查键是否存在
    //     auto it = graph_adjlist_minus.find(vertex_name);
    //     if (it != graph_adjlist_minus.end()) {
    //         // 如果存在，将新的邻接信息添加到现有的向量中
    //         it->second.insert(it->second.end(), adj_info.begin(), adj_info.end());
    //     } else {
    //         // 如果不存在，创建新的条目
    //         graph_adjlist_minus[vertex_name] = adj_info;
    //     }
    // }    
    // void add_adj_minus(const Name_type& vertex_name, 
    //         const std::pair<Name_type, weight_type>& single_adj_info) {
    //         // 检查键是否存在
    //     auto it = graph_adjlist_minus.find(vertex_name);
    //     std::vector<std::pair<Name_type, weight_type>> adj_list;
    //     if (it != graph_adjlist_minus.end()) {
    //         // 如果存在，获取现有的向量
    //         adj_list = it->second;
    //     } 
    //     // 创建向量并添加单个邻接信息
    //     adj_list.push_back(single_adj_info);
    //
    //     // 更新graph_adjlist_plus中的向量
    //     graph_adjlist_minus[vertex_name] = adj_list;
    // }
    //
    // void add_adj_plus(const Name_type& vertex_name, 
    //         const std::vector<std::pair<Name_type, weight_type>>& adj_info ){
    //         // 检查键是否存在
    //     auto it = graph_adjlist_plus.find(vertex_name);
    //     if (it != graph_adjlist_plus.end()){
    //         // 如果存在，将新的邻接信息添加到现有的向量中
    //         it->second.insert(it->second.end(), adj_info.begin(), adj_info.end());
    //     } 
    //     else{
    //     // 如果不存在，创建新的条目
    //     graph_adjlist_plus[vertex_name] = adj_info;
    //     }
    // }
    // void add_adj_plus(const Name_type& vertex_name, 
    //         const std::pair<Name_type, weight_type>& single_adj_info) {
    //         // 检查键是否存在
    //     auto it = graph_adjlist_plus.find(vertex_name);
    //     std::vector<std::pair<Name_type, weight_type>> adj_list;
    //     if (it != graph_adjlist_plus.end()) {
    //         // 如果存在，获取现有的向量
    //         adj_list = it->second;
    //     } 
    //     // 创建向量并添加单个邻接信息
    //     adj_list.push_back(single_adj_info);
    //
    //     // 更新graph_adjlist_plus中的向量
    //     graph_adjlist_plus[vertex_name] = adj_list;
    // }
    //
    //
    // void read_graph_data(const Module& gra_data, Name_type& prefix, Name_type& mod_index);
    */

};


/*old*/
Graph module_to_graph_old(Graph_data& gra_data);
bool is_in_map(const Name_type& pin_name,
            const std::unordered_map<Edge_index_type, std::pair<Edge_type, Range>>& IO_map);
            
bool is_in_map(const Name_type& pin_name,
               const std::unordered_map<Name_type, int>& IO_map);
void module_to_graph_old(
        Graph& result, Module& gra,
        std::unordered_map<Name_type, Module*> sub_map,
        int& temp_add);


/*old*/











/**yours**/
struct GarphData
{
    int *xadj;
    int *vwgt;
    int *adjncy;
    int *adjcwgt;
    std::map<int,std::string> code_endpoints;
    int comb_number = -1;
    int point_number = 0;
    int leng_adj = 0;
};

/*****************************Add to Graph********************************************/

void addPair(std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> &graph,
             const std::string &key, const std::string &neighbor, int value);