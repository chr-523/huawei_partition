module half_adder(a,b,sum,cout,cc,dd);
    input a;
    input b;
    input [15:0] cc;
    output sum;
    output cout;
    output [15:0] dd;
    assign dd = cc;
    wire n1, n2, n3, n4;

    wire [15:0] n5;

    tuu T1 ( .A(n5));
    tuu T2 ( .B(n5[3]));
    tuu T3 ( .C({n1, n2, n5[15:0]}));
    tuu T4 ( .D(n5[7:0])); //question! and 2

//   aq_idu_expand_32_7 x_aq_idu_expand_32_wbt_dst0_reg ( .x_num(
//         dp_wbt_dst0_reg[4:0]), .x_num_expand({wbt_dst0_reg_expand, 
//         __UC__n_1}) );

    sky130_fd_sc_hd__xor2_1 U1 ( .A(a), .B(b), .X(n1) );
    sky130_fd_sc_hd__and2_1 U2 ( .A(a), .B(b), .X(cout) );
    sky130_fd_sc_hd__inv_1 U3 ( .A(n1), .Y(n2) );
    sky130_fd_sc_hd__inv_1 U4 ( .A(n2), .Y(sum) );
    sky130_fd_sc_hd__inv_1 U5 ( .A(n3), .Y(n4) );
    sky130_fd_sc_hd__inv_1 U6 ( .A(n4), .Y(cout) );

endmodule