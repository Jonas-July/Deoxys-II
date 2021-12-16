package MixSingleColumn;

import Vector::*;

interface MixSingleColumnIfc;

	method Vector#(4, Bit#(8)) mixColumn(Vector#(4, Bit#(8)) val);

endinterface

(* synthesize *)
module mkMixSingleColumn(MixSingleColumnIfc);


	method Vector#(4, Bit#(8)) mixColumn(Vector#(4, Bit#(8)) val);
		Vector#(4, Bit#(8)) new_arr = newVector;
		Bit#(8) mod01 = 0;
		mod01[0] = 1 & (val[0][7] ^ val[1][7]);
		mod01[1] = 1 & (val[0][7] ^ val[1][7]);
		mod01[3] = 1 & (val[0][7] ^ val[1][7]);
		mod01[4] = 1 & (val[0][7] ^ val[1][7]);
		new_arr[0] = (((val[0] ^ val[1])<<1) ^ mod01) ^ val[1] ^ val[2] ^ val[3];
		Bit#(8) mod12 = 0;
		mod12[0] = 1 & (val[1][7] ^ val[2][7]);
		mod12[1] = 1 & (val[1][7] ^ val[2][7]);
		mod12[3] = 1 & (val[1][7] ^ val[2][7]);
		mod12[4] = 1 & (val[1][7] ^ val[2][7]);
		new_arr[1] = (((val[1] ^ val[2])<<1) ^ mod12) ^ val[0] ^ val[2] ^ val[3];
		Bit#(8) mod23 = 0;
		mod23[0] = 1 & (val[2][7] ^ val[3][7]);
		mod23[1] = 1 & (val[2][7] ^ val[3][7]);
		mod23[3] = 1 & (val[2][7] ^ val[3][7]);
		mod23[4] = 1 & (val[2][7] ^ val[3][7]);
		new_arr[2] = (((val[2] ^ val[3])<<1) ^ mod23) ^ val[0] ^ val[1] ^ val[3];
		Bit#(8) mod30 = 0;
		mod30[0] = 1 & (val[3][7] ^ val[0][7]);
		mod30[1] = 1 & (val[3][7] ^ val[0][7]);
		mod30[3] = 1 & (val[3][7] ^ val[0][7]);
		mod30[4] = 1 & (val[3][7] ^ val[0][7]);
		new_arr[3] = (((val[3] ^ val[0])<<1) ^ mod30) ^ val[0] ^ val[1] ^ val[2];
		return new_arr;
	endmethod

endmodule

endpackage