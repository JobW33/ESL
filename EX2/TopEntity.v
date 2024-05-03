module TopEntity(
input clk,
output reg led = 0
);
reg [31:0]count = 0;
always @(posedge clk) begin
if(count == 99999999) begin //Time is up
count <= 0; //Reset count register
led <= ~led; //Toggle led (in each second)
end else begin
count <= count + 1; //Counts 100MHz clock
end
end
endmodule