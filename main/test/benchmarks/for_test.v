module test(a,dd);
    input a;
    output [15:0] dd;
    wire n1;
    wire [15:0] n2;

    assign dd = cc;
    assign dd[7] = cc[9];
    assign dd[8] = n1; // only signal net
    // assign dd[7:0] = cc[9:2]; // never appeared
    // assign dd[7:0] = {cc[9:1], n2}; // never appeared
    
    tu_test_ins T1 ( .A(n1), .B(n2[3]));
    tu_test_ins T2 ( .B(n2[3]));
    tu_test_mod M1 ( .A(n1));
    tu_test_mod M2 ( .B(n2[5]));
    tu_test_mod M3 ( .C({n1, n2[4], n5[15:0]}));
    tu_test_mod M4 ( .D(n7[7:0])); //question! and 2
    //2 and 4 has been detected (NAME range was detected twice) but not push_back

endmodule