//# 9 inputs
//# 11 outputs
//# 15 D-type flipflops
//# 57 inverters
//# 104 gates (44 ANDs + 19 NANDs + 10 ORs + 31 NORs)

module dff (CK,Q,D);
input CK,D;
output Q;

  wire NM,NCK;
  trireg NQ,M;

  nmos N7 (M,D,NCK);
  not P3 (NM,M);
  nmos N9 (NQ,NM,CK);
  not P5 (Q,NQ);
  not P1 (NCK,CK);

endmodule

module s349(GND,VDD,CK,A0,A1,A2,A3,B0,B1,B2,B3,CNTVCO2,CNTVCON2,P0,P1,P2,P3,P4,
  P5,P6,P7,READY,START);
input GND,VDD,CK,START,B0,B1,B2,B3,A0,A1,A2,A3;
output CNTVCO2,CNTVCON2,READY,P0,P1,P2,P3,P4,P5,P6,P7;

  wire CT2,CNTVG3VD,CT1,CNTVG2VD,CT0,CNTVG1VD,ACVQN3,ACVG4VD1,ACVQN2,ACVG3VD1,
    ACVQN1,ACVG2VD1,ACVQN0,ACVG1VD1,MRVQN3,MRVG4VD,MRVQN2,MRVG3VD,MRVQN1,
    MRVG2VD,MRVQN0,MRVG1VD,AX3,AM3,AX2,AM2,AX1,AM1,AX0,AM0,READYN,CT1N,
    CNTVG3VQN,CNTVG2VQN,CNTVCO0,CNTVG1VQN,CNTVCON0,CNTVG1VD1,S3,ADDVG4VSN,CO,
    ADDVG4VCN,S2,ADDVG3VSN,ADDVC3,ADDVG3VCN,S1,ADDVG2VSN,ADDVC2,ADDVG2VCN,
    ADDVC1,ADDVG1VCN,S0,ADDVG1VP,AD0,AD0N,AD1,AD1N,AD2,AD2N,AD3,AD3N,ACVPCN,
    SMVG5VS0P,SMVS0N,SM3,SMVG5VX,SMVG4VS0P,SM2,SMVG4VX,SMVG3VS0P,SM1,SMVG3VX,
    SMVG2VS0P,SM0,SMVG2VX,ADSH,MRVSHLDN,BMVG5VS0P,BMVS0N,BM3,BMVG5VX,BMVG4VS0P,
    BM2,BMVG4VX,BMVG3VS0P,BM1,BMVG3VX,BMVG2VS0P,BM0,BMVG2VX,AMVG5VS0P,AMVS0N,
    AMVG5VX,AMVG4VS0P,AMVG4VX,AMVG3VS0P,AMVG3VX,AMVG2VS0P,AMVG2VX,IINIIT,
    ADDVG4VCNVAD4NF,ADDVG4VCNVAD3NF,ADDVG4VCNVOR2NF,ADDVG4VCNVAD2NF,
    ADDVG4VCNVOR1NF,ADDVG4VCNVAD1NF,ADDVG3VCNVAD4NF,ADDVG3VCNVAD3NF,
    ADDVG3VCNVOR2NF,ADDVG3VCNVAD2NF,ADDVG3VCNVOR1NF,ADDVG3VCNVAD1NF,
    ADDVG2VCNVAD4NF,ADDVG2VCNVAD3NF,ADDVG2VCNVOR2NF,ADDVG2VCNVAD2NF,
    ADDVG2VCNVOR1NF,ADDVG2VCNVAD1NF,SMVG5VG1VAD2NF,SMVG5VG1VAD1NF,
    SMVG4VG1VAD2NF,SMVG4VG1VAD1NF,SMVG3VG1VAD2NF,SMVG3VG1VAD1NF,SMVG2VG1VAD2NF,
    SMVG2VG1VAD1NF,MRVG4VDVAD2NF,MRVG4VDVAD1NF,MRVG3VDVAD2NF,MRVG3VDVAD1NF,
    MRVG2VDVAD2NF,MRVG2VDVAD1NF,MRVG1VDVAD2NF,MRVG1VDVAD1NF,BMVG5VG1VAD2NF,
    BMVG5VG1VAD1NF,BMVG4VG1VAD2NF,BMVG4VG1VAD1NF,BMVG3VG1VAD2NF,BMVG3VG1VAD1NF,
    BMVG2VG1VAD2NF,BMVG2VG1VAD1NF,AMVG5VG1VAD2NF,AMVG5VG1VAD1NF,AMVG4VG1VAD2NF,
    AMVG4VG1VAD1NF,AMVG3VG1VAD2NF,AMVG3VG1VAD1NF,AMVG2VG1VAD2NF,AMVG2VG1VAD1NF,
    CNTVG3VG2VOR1NF,CNTVG3VD1,CNTVG2VG2VOR1NF,CNTVG2VD1,CNTVG1VG2VOR1NF,
    ADDVG1VPVOR1NF,CNTVCO1,CNTVG3VZ,CNTVG3VZ1,CNTVCON1,CNTVG2VZ,CNTVG2VZ1,
    CNTVG1VZ,CNTVG1VZ1;

  dff DFF_0(CK,CT2,CNTVG3VD);
  dff DFF_1(CK,CT1,CNTVG2VD);
  dff DFF_2(CK,CT0,CNTVG1VD);
  dff DFF_3(CK,ACVQN3,ACVG4VD1);
  dff DFF_4(CK,ACVQN2,ACVG3VD1);
  dff DFF_5(CK,ACVQN1,ACVG2VD1);
  dff DFF_6(CK,ACVQN0,ACVG1VD1);
  dff DFF_7(CK,MRVQN3,MRVG4VD);
  dff DFF_8(CK,MRVQN2,MRVG3VD);
  dff DFF_9(CK,MRVQN1,MRVG2VD);
  dff DFF_10(CK,MRVQN0,MRVG1VD);
  dff DFF_11(CK,AX3,AM3);
  dff DFF_12(CK,AX2,AM2);
  dff DFF_13(CK,AX1,AM1);
  dff DFF_14(CK,AX0,AM0);
  not NOT_0(READY,READYN);
  not NOT_1(CT1N,CT1);
  not NOT_2(CNTVG3VQN,CT2);
  not NOT_3(CNTVG2VQN,CT1);
  not NOT_4(CNTVCO0,CNTVG1VQN);
  not NOT_5(CNTVCON0,CT0);
  not NOT_6(CNTVG1VQN,CT0);
  not NOT_7(CNTVG1VD1,READY);
  not NOT_8(S3,ADDVG4VSN);
  not NOT_9(CO,ADDVG4VCN);
  not NOT_10(S2,ADDVG3VSN);
  not NOT_11(ADDVC3,ADDVG3VCN);
  not NOT_12(S1,ADDVG2VSN);
  not NOT_13(ADDVC2,ADDVG2VCN);
  not NOT_14(ADDVC1,ADDVG1VCN);
  not NOT_15(S0,ADDVG1VP);
  not NOT_16(AD0,AD0N);
  not NOT_17(AD1,AD1N);
  not NOT_18(AD2,AD2N);
  not NOT_19(AD3,AD3N);
  not NOT_20(ACVPCN,START);
  not NOT_21(P7,ACVQN3);
  not NOT_22(P6,ACVQN2);
  not NOT_23(P5,ACVQN1);
  not NOT_24(P4,ACVQN0);
  not NOT_25(SMVG5VS0P,SMVS0N);
  not NOT_26(SM3,SMVG5VX);
  not NOT_27(SMVG4VS0P,SMVS0N);
  not NOT_28(SM2,SMVG4VX);
  not NOT_29(SMVG3VS0P,SMVS0N);
  not NOT_30(SM1,SMVG3VX);
  not NOT_31(SMVG2VS0P,SMVS0N);
  not NOT_32(SM0,SMVG2VX);
  not NOT_33(SMVS0N,ADSH);
  not NOT_34(MRVSHLDN,ADSH);
  not NOT_35(P3,MRVQN3);
  not NOT_36(P2,MRVQN2);
  not NOT_37(P1,MRVQN1);
  not NOT_38(P0,MRVQN0);
  not NOT_39(BMVG5VS0P,BMVS0N);
  not NOT_40(BM3,BMVG5VX);
  not NOT_41(BMVG4VS0P,BMVS0N);
  not NOT_42(BM2,BMVG4VX);
  not NOT_43(BMVG3VS0P,BMVS0N);
  not NOT_44(BM1,BMVG3VX);
  not NOT_45(BMVG2VS0P,BMVS0N);
  not NOT_46(BM0,BMVG2VX);
  not NOT_47(BMVS0N,READYN);
  not NOT_48(AMVG5VS0P,AMVS0N);
  not NOT_49(AM3,AMVG5VX);
  not NOT_50(AMVG4VS0P,AMVS0N);
  not NOT_51(AM2,AMVG4VX);
  not NOT_52(AMVG3VS0P,AMVS0N);
  not NOT_53(AM1,AMVG3VX);
  not NOT_54(AMVG2VS0P,AMVS0N);
  not NOT_55(AM0,AMVG2VX);
  not NOT_56(AMVS0N,IINIIT);
  and AND3_0(ADDVG4VCNVAD4NF,ADDVC3,AD3,P7);
  and AND2_0(ADDVG4VCNVAD3NF,ADDVG4VCNVOR2NF,ADDVG4VCN);
  and AND2_1(ADDVG4VCNVAD2NF,ADDVC3,ADDVG4VCNVOR1NF);
  and AND2_2(ADDVG4VCNVAD1NF,AD3,P7);
  and AND3_1(ADDVG3VCNVAD4NF,ADDVC2,AD2,P6);
  and AND2_3(ADDVG3VCNVAD3NF,ADDVG3VCNVOR2NF,ADDVG3VCN);
  and AND2_4(ADDVG3VCNVAD2NF,ADDVC2,ADDVG3VCNVOR1NF);
  and AND2_5(ADDVG3VCNVAD1NF,AD2,P6);
  and AND3_2(ADDVG2VCNVAD4NF,ADDVC1,AD1,P5);
  and AND2_6(ADDVG2VCNVAD3NF,ADDVG2VCNVOR2NF,ADDVG2VCN);
  and AND2_7(ADDVG2VCNVAD2NF,ADDVC1,ADDVG2VCNVOR1NF);
  and AND2_8(ADDVG2VCNVAD1NF,AD1,P5);
  and AND2_9(SMVG5VG1VAD2NF,SMVG5VS0P,CO);
  and AND2_10(SMVG5VG1VAD1NF,SMVS0N,P7);
  and AND2_11(SMVG4VG1VAD2NF,SMVG4VS0P,S3);
  and AND2_12(SMVG4VG1VAD1NF,SMVS0N,P6);
  and AND2_13(SMVG3VG1VAD2NF,SMVG3VS0P,S2);
  and AND2_14(SMVG3VG1VAD1NF,SMVS0N,P5);
  and AND2_15(SMVG2VG1VAD2NF,SMVG2VS0P,S1);
  and AND2_16(SMVG2VG1VAD1NF,SMVS0N,P4);
  and AND2_17(MRVG4VDVAD2NF,MRVSHLDN,BM3);
  and AND2_18(MRVG4VDVAD1NF,ADSH,S0);
  and AND2_19(MRVG3VDVAD2NF,MRVSHLDN,BM2);
  and AND2_20(MRVG3VDVAD1NF,ADSH,P3);
  and AND2_21(MRVG2VDVAD2NF,MRVSHLDN,BM1);
  and AND2_22(MRVG2VDVAD1NF,ADSH,P2);
  and AND2_23(MRVG1VDVAD2NF,MRVSHLDN,BM0);
  and AND2_24(MRVG1VDVAD1NF,ADSH,P1);
  and AND2_25(BMVG5VG1VAD2NF,BMVG5VS0P,B3);
  and AND2_26(BMVG5VG1VAD1NF,BMVS0N,P3);
  and AND2_27(BMVG4VG1VAD2NF,BMVG4VS0P,B2);
  and AND2_28(BMVG4VG1VAD1NF,BMVS0N,P2);
  and AND2_29(BMVG3VG1VAD2NF,BMVG3VS0P,B1);
  and AND2_30(BMVG3VG1VAD1NF,BMVS0N,P1);
  and AND2_31(BMVG2VG1VAD2NF,BMVG2VS0P,B0);
  and AND2_32(BMVG2VG1VAD1NF,BMVS0N,P0);
  and AND2_33(AMVG5VG1VAD2NF,AMVG5VS0P,A3);
  and AND2_34(AMVG5VG1VAD1NF,AMVS0N,AX3);
  and AND2_35(AMVG4VG1VAD2NF,AMVG4VS0P,A2);
  and AND2_36(AMVG4VG1VAD1NF,AMVS0N,AX2);
  and AND2_37(AMVG3VG1VAD2NF,AMVG3VS0P,A1);
  and AND2_38(AMVG3VG1VAD1NF,AMVS0N,AX1);
  and AND2_39(AMVG2VG1VAD2NF,AMVG2VS0P,A0);
  and AND2_40(AMVG2VG1VAD1NF,AMVS0N,AX0);
  or OR2_0(CNTVG3VG2VOR1NF,CT2,CNTVG3VD1);
  or OR2_1(CNTVG2VG2VOR1NF,CT1,CNTVG2VD1);
  or OR2_2(CNTVG1VG2VOR1NF,CT0,CNTVG1VD1);
  or OR3_0(ADDVG4VCNVOR2NF,ADDVC3,AD3,P7);
  or OR2_3(ADDVG4VCNVOR1NF,AD3,P7);
  or OR3_1(ADDVG3VCNVOR2NF,ADDVC2,AD2,P6);
  or OR2_4(ADDVG3VCNVOR1NF,AD2,P6);
  or OR3_2(ADDVG2VCNVOR2NF,ADDVC1,AD1,P5);
  or OR2_5(ADDVG2VCNVOR1NF,AD1,P5);
  or OR2_6(ADDVG1VPVOR1NF,AD0,P4);
  nand NAND3_0(READYN,CT0,CT1N,CT2);
  nand NAND2_0(CNTVCON2,CT2,CNTVCO1);
  nand NAND2_1(CNTVG3VZ,CNTVG3VG2VOR1NF,CNTVG3VZ1);
  nand NAND2_2(CNTVG3VZ1,CT2,CNTVG3VD1);
  nand NAND2_3(CNTVCON1,CT1,CNTVCO0);
  nand NAND2_4(CNTVG2VZ,CNTVG2VG2VOR1NF,CNTVG2VZ1);
  nand NAND2_5(CNTVG2VZ1,CT1,CNTVG2VD1);
  nand NAND2_6(CNTVG1VZ,CNTVG1VG2VOR1NF,CNTVG1VZ1);
  nand NAND2_7(CNTVG1VZ1,CT0,CNTVG1VD1);
  nand NAND2_8(ADDVG1VP,ADDVG1VPVOR1NF,ADDVG1VCN);
  nand NAND2_9(ADDVG1VCN,AD0,P4);
  nand NAND2_10(AD0N,P0,AX0);
  nand NAND2_11(AD1N,P0,AX1);
  nand NAND2_12(AD2N,P0,AX2);
  nand NAND2_13(AD3N,P0,AX3);
  nand NAND2_14(ACVG4VD1,ACVPCN,SM3);
  nand NAND2_15(ACVG3VD1,ACVPCN,SM2);
  nand NAND2_16(ACVG2VD1,ACVPCN,SM1);
  nand NAND2_17(ACVG1VD1,ACVPCN,SM0);
  nor NOR2_0(ADSH,READY,IINIIT);
  nor NOR3_0(IINIIT,CT0,CT1,CT2);
  nor NOR2_1(CNTVCO2,CNTVG3VQN,CNTVCON1);
  nor NOR2_2(CNTVG3VD,CNTVG3VZ,START);
  nor NOR2_3(CNTVG3VD1,READY,CNTVCON1);
  nor NOR2_4(CNTVCO1,CNTVG2VQN,CNTVCON0);
  nor NOR2_5(CNTVG2VD,CNTVG2VZ,START);
  nor NOR2_6(CNTVG2VD1,READY,CNTVCON0);
  nor NOR2_7(CNTVG1VD,CNTVG1VZ,START);
  nor NOR2_8(ADDVG4VSN,ADDVG4VCNVAD4NF,ADDVG4VCNVAD3NF);
  nor NOR2_9(ADDVG4VCN,ADDVG4VCNVAD2NF,ADDVG4VCNVAD1NF);
  nor NOR2_10(ADDVG3VSN,ADDVG3VCNVAD4NF,ADDVG3VCNVAD3NF);
  nor NOR2_11(ADDVG3VCN,ADDVG3VCNVAD2NF,ADDVG3VCNVAD1NF);
  nor NOR2_12(ADDVG2VSN,ADDVG2VCNVAD4NF,ADDVG2VCNVAD3NF);
  nor NOR2_13(ADDVG2VCN,ADDVG2VCNVAD2NF,ADDVG2VCNVAD1NF);
  nor NOR2_14(SMVG5VX,SMVG5VG1VAD2NF,SMVG5VG1VAD1NF);
  nor NOR2_15(SMVG4VX,SMVG4VG1VAD2NF,SMVG4VG1VAD1NF);
  nor NOR2_16(SMVG3VX,SMVG3VG1VAD2NF,SMVG3VG1VAD1NF);
  nor NOR2_17(SMVG2VX,SMVG2VG1VAD2NF,SMVG2VG1VAD1NF);
  nor NOR2_18(MRVG4VD,MRVG4VDVAD2NF,MRVG4VDVAD1NF);
  nor NOR2_19(MRVG3VD,MRVG3VDVAD2NF,MRVG3VDVAD1NF);
  nor NOR2_20(MRVG2VD,MRVG2VDVAD2NF,MRVG2VDVAD1NF);
  nor NOR2_21(MRVG1VD,MRVG1VDVAD2NF,MRVG1VDVAD1NF);
  nor NOR2_22(BMVG5VX,BMVG5VG1VAD2NF,BMVG5VG1VAD1NF);
  nor NOR2_23(BMVG4VX,BMVG4VG1VAD2NF,BMVG4VG1VAD1NF);
  nor NOR2_24(BMVG3VX,BMVG3VG1VAD2NF,BMVG3VG1VAD1NF);
  nor NOR2_25(BMVG2VX,BMVG2VG1VAD2NF,BMVG2VG1VAD1NF);
  nor NOR2_26(AMVG5VX,AMVG5VG1VAD2NF,AMVG5VG1VAD1NF);
  nor NOR2_27(AMVG4VX,AMVG4VG1VAD2NF,AMVG4VG1VAD1NF);
  nor NOR2_28(AMVG3VX,AMVG3VG1VAD2NF,AMVG3VG1VAD1NF);
  nor NOR2_29(AMVG2VX,AMVG2VG1VAD2NF,AMVG2VG1VAD1NF);

endmodule
