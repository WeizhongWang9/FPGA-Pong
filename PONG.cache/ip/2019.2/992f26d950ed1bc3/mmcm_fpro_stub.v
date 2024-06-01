// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// --------------------------------------------------------------------------------
// Tool Version: Vivado v.2019.2 (win64) Build 2708876 Wed Nov  6 21:40:23 MST 2019
// Date        : Wed Nov 24 13:47:24 2021
// Host        : DESKTOP-AMPCO0P running 64-bit major release  (build 9200)
// Command     : write_verilog -force -mode synth_stub -rename_top decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix -prefix
//               decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix_ mmcm_fpro_stub.v
// Design      : mmcm_fpro
// Purpose     : Stub declaration of top-level module interface
// Device      : xc7a100tcsg324-1
// --------------------------------------------------------------------------------

// This empty module with port declaration file causes synthesis tools to infer a black box for IP.
// The synthesis directives are for Synopsys Synplify support to prevent IO buffer insertion.
// Please paste the declaration into a Verilog source file or add the file as an additional source.
module decalper_eb_ot_sdeen_pot_pi_dehcac_xnilix(clk_100M, clk_25M, clk_40M, clk_67M, reset, locked, 
  clk_in_100M)
/* synthesis syn_black_box black_box_pad_pin="clk_100M,clk_25M,clk_40M,clk_67M,reset,locked,clk_in_100M" */;
  output clk_100M;
  output clk_25M;
  output clk_40M;
  output clk_67M;
  input reset;
  output locked;
  input clk_in_100M;
endmodule
