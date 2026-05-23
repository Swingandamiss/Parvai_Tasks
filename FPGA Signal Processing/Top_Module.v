module top_module(
    input clk,                 // 50MHz system clock
    input rst,                 
    input [11:0] sensor_in,    // 12-bit sensor input
    output [15:0] avg_out      // 16-bit moving average output
);
wire clk_1; //100Hz clock
Clock_Divider div(clk,rst,clk_1);
moving_avg_filter avg_filter(clk,rst,sensor_in,avg_out);
endmodule
