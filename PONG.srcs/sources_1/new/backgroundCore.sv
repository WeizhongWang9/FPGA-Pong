`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/24/2021 06:50:13 PM
// Design Name: 
// Module Name: backgroundCore
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


module backgroundCore
  #(
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

   logic wr_en, wr_ram, wr_reg, wr_ctrl, wr_bypass, wr_x0, wr_y0,wr_x1, wr_y1,wr_itemColor, wr_length,wr_width;
   logic [CD-1:0] sprite_rgb, chrom_rgb;
   logic [10:0] x0_reg, y0_reg;
   logic [10:0] x1_reg, y1_reg;
   logic [8:0] itemColor_reg;
   logic bypass_reg;
   logic [10:0]length_reg;
   logic [10:0]width_reg;
   logic [CD-1:0]spriteOutRGBPaddle1,spriteOutRGBPaddle2, middleLine;

   // body
   // instantiate sprite generator
   
   
   
   paddle #(.CD(CD), .ADDR(ADDR), .KEY_COLOR(KEY_COLOR)) upperBoundry
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

   
   paddle #(.CD(CD), .ADDR(ADDR), .KEY_COLOR(KEY_COLOR)) lowerBoundry
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
   
   backGround   #(.KEY_COLOR(KEY_COLOR))
   (.clk(clk),
    .x(x), .y(y),   // x-and  y-coordinate    
    .bar_rgb(middleLine) 
   
   );
   
   
   assign sprite_rgb = spriteOutRGBPaddle1 | spriteOutRGBPaddle2 | middleLine;
       
   // register  
   always_ff @(posedge clk, posedge reset)
      if (reset) begin
         x0_reg <= 0;
         y0_reg <= 0;
         x1_reg <= 0;
         y1_reg <= 0;         

         length_reg <= 0;
         width_reg<=0;

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
         if (wr_itemColor)
            itemColor_reg <= wr_data[8:0];
         if (wr_length)
            length_reg <= wr_data[10:0];
         if (wr_bypass)
            bypass_reg <= wr_data[0];
         if (wr_width)
            width_reg <= wr_data[4:0];

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
   assign wr_itemColor = wr_reg && (addr[3:0]==4'b0111);
   assign wr_length = wr_reg && (addr[3:0]==4'b1000);
   assign wr_width = wr_reg && (addr[3:0]==4'b1001);
   // chrome-key blending and multiplexing
   assign chrom_rgb = (sprite_rgb != KEY_COLOR) ? sprite_rgb : si_rgb;
   assign so_rgb = (bypass_reg) ? si_rgb : chrom_rgb;
   
endmodule
