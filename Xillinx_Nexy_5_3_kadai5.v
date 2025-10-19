/* ��T�͉ۑ�@�\�t�g�E�F�A�ɂ��_�C�i�~�b�N�_����p����60�b�v */
/* Copyright(C) 2013 Cobac.Net, All rights reserved.            */

module KADAI5(
    input           CLK100, RST,
    output  [7:0]   nSEG,
    output  [3:0]   nAN,
    output          TXD
);

/* CLK(50MHz)�̍쐬 */
reg CLK;

always @( posedge CLK100 ) begin
    CLK <= ~CLK;
end

mcs mcs_0 (
    .Clk    (CLK),
    .Reset  (RST),
    .GPO1   (nSEG),
    .GPO2   (nAN),
    .UART_Tx(TXD)
);

endmodule
