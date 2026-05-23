module filter_tb;
reg clk;
reg rst;
reg [11:0] sensor_in;
wire [15:0] avg_out;
top_module dut(clk,rst,sensor_in,avg_out);
always #5 clk = ~clk;
initial
begin
    clk = 0;
    rst = 1;
    sensor_in = 0;
    #20;
    rst = 0;
    sensor_in = 12'd10;  #10;
    sensor_in = 12'd20;  #10;
    sensor_in = 12'd30;  #10;
    sensor_in = 12'd40;  #10;
    sensor_in = 12'd50;  #10;
    sensor_in = 12'd60;  #10;
    sensor_in = 12'd70;  #10;
    sensor_in = 12'd80;  #10;
    sensor_in = 12'd90;  #10;
    sensor_in = 12'd100; #10;
    sensor_in = 12'd110; #10;
    sensor_in = 12'd120; #10;
    sensor_in = 12'd130; #10;
    sensor_in = 12'd140; #10;
    sensor_in = 12'd150; #10;
    sensor_in = 12'd160; #10;
    sensor_in = 12'd170; #10;
    sensor_in = 12'd180; #10;
    sensor_in = 12'd190; #10;
    sensor_in = 12'd200; #10;
    #100;
    $finish;
end
initial
begin
    $monitor("Time=%0t Sensor=%d Average=%d",
              $time, sensor_in, avg_out);
end
endmodule
