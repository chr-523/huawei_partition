#include <unordered_map>
#include <string>
#include <vector>
#include <utility>

using weight_type = float;

class Vertex
{
public:
    Vertex( const std::string& name = default_name, const bool& is_clk = false):
        name(name), is_clk(is_clk)  {
            this -> weight = 1.0;
            this -> outDegree = 0;
            this -> inDegree = 0;
            this -> comb_rank = -1;
            this -> reg_rank = -1;
            this -> reg_temp = -1;
    };
    ~Vertex(){};
public:
    std::string get_name() const { return name; }
    weight_type get_weight() const { return weight; }
    int get_outDgree() const { return outDegree; }
        void add_outDgree(){ outDegree++ ; }
        void set_outDgree( int O_ ){ outDegree = O_ ; }
    int get_inDgree() const { return inDegree; }
        void add_inDgree(){ inDegree++ ; }
        void set_inDgree( int I_ ){ inDegree = I_ ; }
    bool get_is_clk() const { return is_clk; }
    int get_comb_rank() const { return comb_rank; }
    int get_reg_rank() const { return reg_rank; }
    int get_reg_temp() const { return reg_temp; }
private:
    std::string name;
    weight_type weight;
    int outDegree;
    int inDegree;
    
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
    std::unordered_map< std::string, Vertex* > vertexs;
    // map of : vertexs' name and IO vector< adj_name, edge_weight >
    std::unordered_map< std::string, std::vector< std::pair< std::string, weight_type >>> graph_adjlist_plus;
    std::unordered_map< std::string, std::vector< std::pair< std::string, weight_type >>> graph_adjlist_minus;
    
    // std::unordered_map<std::string,std::tuple<Vertex*, graph+,graph->>

    Graph(){};

    ~Graph(){
        for (auto& pair : vertexs) {
            delete pair.second;
        }
        vertexs.clear();
    }

    void addVertex(const Vertex& v_) {
        // 创建一个新的Vertex对象，并将其复制构造
        Vertex* newVertex = new Vertex(v_);

        std::string name =  v_.get_name();
        
        vertexs[name] = newVertex;
    }

    // void addadj()
};
