module moving_avg_filter(
    input clk,
    input rst,
    input [11:0] sensor_in,
    output reg [15:0] avg_out
);
reg [11:0] samples [0:7];
reg [15:0] sum;
integer i;
always @(posedge clk or posedge rst)
begin
    if(rst)
    begin
        sum <= 0;
        avg_out <= 0;
        for(i=0; i<8; i=i+1)
            samples[i] <= 0;
    end
    else
    begin
        // Removing oldest sample from sum
        sum <= sum - samples[7] + sensor_in;
        // Shifting samples
        samples[7] <= samples[6];
        samples[6] <= samples[5];
        samples[5] <= samples[4];
        samples[4] <= samples[3];
        samples[3] <= samples[2];
        samples[2] <= samples[1];
        samples[1] <= samples[0];
        samples[0] <= sensor_in;
        // Dividing by 8 using right shift
        avg_out <= (sum - samples[7] + sensor_in) >> 3;
    end
end
endmodule
