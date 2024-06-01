`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/24/2021 03:51:41 PM
// Design Name: 
// Module Name: playgroundCore
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

module playgroundCore    #(
    parameter CD = 12,      // color depth
              ADDR = 10,    // number of address bits
              KEY_COLOR =0  // chroma key
   )
   (
   input logic clk, reset,
   input  logic [10:0] x, y,
   // video slot interface
   input  logic cs,      
   input  logic write,  
   input  logic [13:0] addr,    
   input  logic [31:0] wr_data,
   // stream interface
   input  logic [11:0] si_rgb,
   output logic [11:0] so_rgb
    
  );

   logic wr_en, wr_ram, wr_reg, wr_ctrl, wr_bypass, wr_x0, wr_y0,wr_x1, wr_y1,wr_x2, wr_y2,wr_itemColor, wr_length,wr_width,wr_ballsize;
   logic [CD-1:0] sprite_rgb, chrom_rgb;
   logic [10:0] x0_reg, y0_reg;
   logic [10:0] x1_reg, y1_reg;
   logic [10:0] x2_reg, y2_reg;
   logic [8:0] itemColor_reg;
   logic bypass_reg;
   logic [10:0]length_reg;
   logic [10:0]width_reg;
   logic [10:0]ballSize;
   logic [CD-1:0]spriteOutRGBPaddle1,spriteOutRGBPaddle2,spriteOutRGBBall;

   // body
   // instantiate sprite generator
   
   
   
   paddle #(.CD(CD), .ADDR(ADDR), .KEY_COLOR(KEY_COLOR)) paddle1
   (
    .clk(clk),
    .x(x), .y(y),   // x-and  y-coordinate    
    .x0(x0_reg), .y0(y0_reg), // origin of sprite 
    .height(length_reg), //length of paddle
    .width(width_reg),
    .rgb(itemColor_reg[2:0]), //color of paddle

    // pixel output
    .pixelOut_rgb(spriteOutRGBPaddle1)
   );

   
   paddle #(.CD(CD), .ADDR(ADDR), .KEY_COLOR(KEY_COLOR)) paddle2
   (
    .clk(clk),
    .x(x), .y(y),   // x-and  y-coordinate    
    .x0(x1_reg), .y0(y1_reg), // origin of sprite 
    .height(length_reg), //length of paddle
    .width(width_reg),
    .rgb(itemColor_reg[5:3]), //color of paddle

    // pixel output
    .pixelOut_rgb(spriteOutRGBPaddle2)
   );
   
   paddle #(.CD(CD), .ADDR(ADDR), .KEY_COLOR(KEY_COLOR)) ball
   (
    .clk(clk),
    .x(x), .y(y),   // x-and  y-coordinate    
    .x0(x2_reg), .y0(y2_reg), // origin of sprite 
    .height(ballSize), //length of paddle
    .width(ballSize),
    .rgb(itemColor_reg[8:6]), //color of paddle

    // pixel output
    .pixelOut_rgb(spriteOutRGBBall)
   );
   
   assign sprite_rgb = spriteOutRGBPaddle1 | spriteOutRGBPaddle2 | spriteOutRGBBall;
       
   // register  
   always_ff @(posedge clk, posedge reset)
      if (reset) begin
         x0_reg <= 0;
         y0_reg <= 0;
         x1_reg <= 0;
         y1_reg <= 0;         
         x2_reg <= 0;
         y2_reg <= 0;
         length_reg <= 0;
         width_reg<=0;
         ballSize<=0;
         itemColor_reg <= 0;
         bypass_reg <= 0;
      end   
      else begin
         if (wr_x0)
            x0_reg <= wr_data[10:0];
         if (wr_y0)
            y0_reg <= wr_data[10:0];         
         if (wr_x1)
            x1_reg <= wr_data[10:0];
         if (wr_y1)
            y1_reg <= wr_data[10:0];         
         if (wr_x2)
            x2_reg <= wr_data[10:0];
         if (wr_y2)
            y2_reg <= wr_data[10:0];
         if (wr_itemColor)
            itemColor_reg <= wr_data[8:0];
         if (wr_length)
            length_reg <= wr_data[10:0];
         if (wr_bypass)
            bypass_reg <= wr_data[0];
         if (wr_width)
            width_reg <= wr_data[4:0];
         if (wr_ballsize)
            ballSize <= wr_data[10:0];
      end      
   // decoding 
   assign wr_en = write & cs;
   assign wr_ram = ~addr[13] && wr_en;
   assign wr_reg = addr[13] && wr_en;
   assign wr_bypass = wr_reg && (addr[3:0]==4'b0000);
   assign wr_x0 = wr_reg && (addr[3:0]==4'b0001);
   assign wr_y0 = wr_reg && (addr[3:0]==4'b0010);
   assign wr_x1 = wr_reg && (addr[3:0]==4'b0011);
   assign wr_y1 = wr_reg && (addr[3:0]==4'b0100);
   assign wr_x2 = wr_reg && (addr[3:0]==4'b0101);
   assign wr_y2 = wr_reg && (addr[3:0]==4'b0110);
   assign wr_itemColor = wr_reg && (addr[3:0]==4'b0111);
   assign wr_length = wr_reg && (addr[3:0]==4'b1000);
   assign wr_width = wr_reg && (addr[3:0]==4'b1001);
   assign wr_ballsize = wr_reg && (addr[3:0]==4'b1010);
   // chrome-key blending and multiplexing
   assign chrom_rgb = (sprite_rgb != KEY_COLOR) ? sprite_rgb : si_rgb;
   assign so_rgb = (bypass_reg) ? si_rgb : chrom_rgb;
   
endmodule
