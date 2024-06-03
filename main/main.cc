#include "main.h"

int main(){

    Graph half_adder;
    
    for (/*for each pin*/){
    half_adder.add_edge(name,low,high,pin_type);//   pin name [123]  1/2  
        // + pin split
    }

    for (/*for each pin*/){
    half_adder.add_edge(name,low,high);//   net name [123]
    }    

    for(/*each instance*/){
        if(/*type = ins*/){//name
            Vertex v_xx(name)//name -> clk 
            half_adder.add_vertex();//   pn [123]  1/2

            for(/*each ins's pin*/ /*pin(net)*/  ){
                connect(v_xx,edge);
                }
        }
        else{//type = module
            Graph module_1//(packge graph shengcheng, recursion);
            half_adder.add_module();
        }
    }
    
    return 0;
}
