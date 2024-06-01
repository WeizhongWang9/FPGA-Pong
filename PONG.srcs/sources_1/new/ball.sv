`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/24/2021 03:47:51 PM
// Design Name: 
// Module Name: ball
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


module ball #(
    parameter CD = 12,      // color depth
              ADDR = 10,    // number of address bits
              KEY_COLOR =0  // chroma key
   )
   (
    input  logic clk,
    input  logic [10:0] x, y,   // x-and  y-coordinate    
    input  logic [10:0] x0, y0, // origin of sprite 
    input  logic [2:0] rgb, //color of paddle

    // pixel output
    output logic [CD-1:0] pixelOut_rgb
   );
   
   // localparam declaration
   localparam H_SIZE = 1; // vertical size of sprite
   localparam V_SIZE = 1; // vertical size of sprite
   
   // signal delaration
   logic signed [11:0] xr, yr;  // relative x/y position
   logic in_region;

   logic [CD-1:0] full_rgb, out_rgb;
   logic [CD-1:0] out_rgb_d1_reg;
   
   // body

   assign full_rgb = ~{rgb[2],rgb[2],rgb[2],rgb[2],rgb[1],rgb[1],rgb[1],rgb[1],rgb[0],rgb[0],rgb[0],rgb[0]};

   // relative coordinate calculation
   assign xr = $signed({1'b0, x}) - $signed({1'b0, x0});
   assign yr = $signed({1'b0, y}) - $signed({1'b0, y0});

   // in-region comparison and multiplexing 
   assign in_region = (0<= xr) && (xr<H_SIZE) && (0<=yr) && (yr<V_SIZE);
   assign out_rgb = in_region ? full_rgb : KEY_COLOR;
   // output with a-stage delay line
   always_ff @(posedge clk) 
      out_rgb_d1_reg <= out_rgb;
   assign pixelOut_rgb = out_rgb_d1_reg;
endmodule
