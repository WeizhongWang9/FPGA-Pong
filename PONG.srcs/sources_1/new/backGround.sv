`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 11/26/2021 04:03:18 PM
// Design Name: 
// Module Name: backGround
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


module backGround   #(KEY_COLOR =0  // chroma key
   )
   (
   input  logic clk,
   input  logic [10:0] x, y,     // treated as x-/y-axis
   output logic [11:0] bar_rgb 
   );
   
   logic middleLine;
   logic [11:0]sprite_rgb;
   // body 
   // instantiate rgb-to-garyscale conversion circuit
    
    always_comb begin
        sprite_rgb = KEY_COLOR;
        
        middleLine = (x == 319 | x == 320) & y[3] == 1'b1 & y > 130 & y < 370;
        if(middleLine)
           sprite_rgb = 12'b111111111111;
    end
    
    assign bar_rgb = sprite_rgb;
   
endmodule
