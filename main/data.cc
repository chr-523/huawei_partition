#include "data.h"

void Edge::split_edge(){
    
}

void Edge::connect_vertex(Vertex* vertex){
    /*
    instance => tu_test_ins, T1, A(n5->[-1: -1])        
    instance => tu_test_ins, T2, B(n6->[-1:3])
    instance => tu_test_ins, T3, C(GROUP_NETS {(n1)(n2)(n5->[15:0])})
    instance => tu_test_ins, T4, D(n7->[7:0])
    */
    // if offset is not initialize, then = 1
    if ( this-> offset_array[1] == -1)
    {
        this-> offset_array[1] = 1;
    }
    else{
        this -> offset_array[1] += 1;        
    }
    //get the vertex's index and push into adj_.
    this -> adjacency_array.push_back(std::get<1>(vertex->get_vertex_data()));
}

void Vertex::connect_edge(Edge* edge){
// instance => tu_test_ins, T1, A(n5->[-1:-1])
// instance => tu_test_ins, T2, B(n6->[-1:3])
// instance => tu_test_ins, T3, C(GROUP_NETS {(n1->[-1:-1])(n2->[-1:-1])(n5->[15:0])})
// instance => tu_test_ins, T4, D(n7->[7:0])
    //get the edge's index and push into edgelist
    this -> connect_edgelist.push_back(edge->get_edge_index());
}

//Determine if it is clk based on the entered name
bool Vertex::is_clk(name_type name){



};

void Edge::connect_vertex_old(Vertex* vertex){
    if( this -> range.low < 0){
        if(this -> range.high < 0 ){
        // instance => tu_test_ins, T1, A(n5->[-1: -1])
        this -> offset_array[1]+=1;
        //get the vertex's index and push into adj_.
        this -> adjacency_array.push_back(std::get<1>(vertex->get_vertex_data()));
        }
        else{
        // instance => tu_test_ins, T2, B(n6->[-1:3])
        }
    }        
    else{

    }   // instance => tu_test_ins, T3, C(GROUP_NETS {(n1)(n2)(n5->[15:0])})
        // instance => tu_test_ins, T4, D(n7->[7:0])
}