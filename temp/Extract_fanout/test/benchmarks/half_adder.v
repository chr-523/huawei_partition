module half_adder(a,b,sum,cout,cc,dd);
    input a;
    input b;
    input cc;
    output sum;
    output cout;
    output dd;
    assign dd = cc;
    wire n1, n2, n3, n4;


    sky130_fd_sc_hd__xor2_1 U1 ( .A(a), .B(b), .X(n1) );
    sky130_fd_sc_hd__and2_1 U2 ( .A(a), .B(b), .X(cout) );
    sky130_fd_sc_hd__inv_1 U3 ( .A(n1), .Y(n2) );
    sky130_fd_sc_hd__inv_1 U4 ( .A(n2), .Y(sum) );
    sky130_fd_sc_hd__inv_1 U5 ( .A(n3), .Y(n4) );
    sky130_fd_sc_hd__inv_1 U6 ( .A(n4), .Y(cout) );

endmodule