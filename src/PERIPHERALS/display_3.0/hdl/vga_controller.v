`timescale 10ns / 1ns
//////////////////////////////////////////////////////////////////////////////////
// Company:
// Engineer:
//
// Create Date: 02/16/2017 10:10:12 PM
// Design Name:
// Module Name: vga_controller
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

module vga_controller(pixel_clk, Hsync, Vsync, active, Counter_X, Counter_Y);
	input pixel_clk;//should be 108MHz via clock wizard
	output active, Hsync, Vsync;
	output [11:0] Counter_X;
	output [11:0] Counter_Y;
	
	/*Some constants*/
    parameter FRAME_WIDTH = 1280;
    parameter FRAME_HEIGHT = 1024;
    parameter H_FP = 48; //H front porch
    parameter H_PW = 112; //H sync pulse width
    parameter H_MAX = 1688; //H total period
    parameter V_FP = 1; //V front porch
    parameter V_PW = 3; //V sync pulse width
    parameter V_MAX = 1066; //V total period
    
    /*Some registers*/
    //H and V counters
    reg [11:0]h_counter_reg = 12'b0;
    reg [11:0]v_counter_reg = 12'b0;
    reg h_sync_reg = 1'b0;
    reg v_sync_reg = 1'b0;
    
    //Pipe counters and sync
    reg [11:0]h_counter_reg_delay;
    reg [11:0]v_counter_reg_delay;
    reg h_sync_reg_delay, v_sync_reg_delay;
    
    //Horizontal counter
    always @ (posedge pixel_clk) begin
        if (h_counter_reg == (H_MAX - 1)) begin
            h_counter_reg <= 0;
        end
        else begin
            h_counter_reg <= h_counter_reg + 1;
        end
    end
    
    //Vertical counter
    always @ (posedge pixel_clk) begin
        if (h_counter_reg == (H_MAX - 1)) begin
            if (v_counter_reg == (V_MAX - 1)) begin
                v_counter_reg <= 0;
            end
            else begin
                v_counter_reg <= v_counter_reg + 1;
            end
        end
    end
    
    //Horizontal sync
    always @ (posedge pixel_clk) begin
        if((h_counter_reg >= (H_FP + FRAME_WIDTH - 1)) && (h_counter_reg < (H_FP + FRAME_WIDTH + H_PW - 1))) begin
            h_sync_reg <= 1;
        end
        else begin
            h_sync_reg <= 0;
        end
    end
    
    //Vertical sync
    always @ (posedge pixel_clk) begin
        if((v_counter_reg >= (V_FP + FRAME_HEIGHT - 1)) && (v_counter_reg < (V_FP + FRAME_HEIGHT + V_PW - 1))) begin
            v_sync_reg <= 1;
        end
        else begin
            v_sync_reg <= 0;
        end
    end
    
    // Notifies whether the current position is in the display area
    // This is one clock cycle ahead
    assign active = ((h_counter_reg < FRAME_WIDTH) && (v_counter_reg < FRAME_HEIGHT));
    
    //Updates pipe register values
    always @ (posedge pixel_clk) begin
        h_counter_reg_delay <= h_counter_reg;
        v_counter_reg_delay <= v_counter_reg;
        h_sync_reg_delay <= h_sync_reg;
        v_sync_reg_delay <= v_sync_reg;
    end
    
    // Position values are one clock cycle ahead of the sync signals
    // due to output colours being registers
    assign Hsync = h_sync_reg_delay;
    assign Vsync = v_sync_reg_delay;
    assign Counter_X = h_counter_reg;
    assign Counter_Y = v_counter_reg;
    
endmodule