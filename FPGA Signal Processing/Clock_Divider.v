module Clock_Divider(
    input clk,
    input rst,
    output reg clk_1
);
reg [31:0] count;
always @(posedge clk or posedge rst)
begin
    if(rst)
    begin
        count <= 0;
        clk_1 <= 0;
    end
    else
    begin
        if(count == 250000)
        begin
            clk_1 <= ~clk_1;
            count <= 0;
        end
        else
            count <= count + 1;
    end
end
endmodule
