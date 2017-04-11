//Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
//--------------------------------------------------------------------------------
//Tool Version: Vivado v.2016.2 (win64) Build 1577090 Thu Jun  2 16:32:40 MDT 2016
//Date        : Sun Apr 02 21:48:23 2017
//Host        : ORION running 64-bit major release  (build 9200)
//Command     : generate_target game_wrapper.bd
//Design      : game_wrapper
//Purpose     : IP block netlist
//--------------------------------------------------------------------------------
`timescale 1 ps / 1 ps

module game_wrapper
   (anode,
    cathode,
    clock_rtl,
    led0,
    led1,
    pb,
    pbleds,
    pdm_clk,
    pdm_data,
    pdm_lrsel,
    pwm_data,
    pwm_en,
    reset_rtl,
    uart_rtl_rxd,
    uart_rtl_txd,
    vga_blu,
    vga_grn,
    vga_hs,
    vga_red,
    vga_vs);
  output [7:0]anode;
  output [7:0]cathode;
  input clock_rtl;
  output led0;
  output led1;
  input [4:0]pb;
  output [4:0]pbleds;
  output pdm_clk;
  input pdm_data;
  output pdm_lrsel;
  output pwm_data;
  output pwm_en;
  input reset_rtl;
  input uart_rtl_rxd;
  output uart_rtl_txd;
  output [3:0]vga_blu;
  output [3:0]vga_grn;
  output vga_hs;
  output [3:0]vga_red;
  output vga_vs;

  wire [7:0]anode;
  wire [7:0]cathode;
  wire clock_rtl;
  wire led0;
  wire led1;
  wire [4:0]pb;
  wire [4:0]pbleds;
  wire pdm_clk;
  wire pdm_data;
  wire pdm_lrsel;
  wire pwm_data;
  wire pwm_en;
  wire reset_rtl;
  wire uart_rtl_rxd;
  wire uart_rtl_txd;
  wire [3:0]vga_blu;
  wire [3:0]vga_grn;
  wire vga_hs;
  wire [3:0]vga_red;
  wire vga_vs;

  game game_i
       (.anode(anode),
        .cathode(cathode),
        .clock_rtl(clock_rtl),
        .led0(led0),
        .led1(led1),
        .pb(pb),
        .pbleds(pbleds),
        .pdm_clk(pdm_clk),
        .pdm_data(pdm_data),
        .pdm_lrsel(pdm_lrsel),
        .pwm_data(pwm_data),
        .pwm_en(pwm_en),
        .reset_rtl(reset_rtl),
        .uart_rtl_rxd(uart_rtl_rxd),
        .uart_rtl_txd(uart_rtl_txd),
        .vga_blu(vga_blu),
        .vga_grn(vga_grn),
        .vga_hs(vga_hs),
        .vga_red(vga_red),
        .vga_vs(vga_vs));
endmodule
