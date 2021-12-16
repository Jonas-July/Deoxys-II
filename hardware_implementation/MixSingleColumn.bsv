package MixSingleColumn;

import Vector::*;

interface MixSingleColumnIfc;

	method Vector#(4, Bit#(8)) mixColumn(Vector#(4, Bit#(8)) val);

endinterface

(* synthesize *)
module mkMixSingleColumn(MixSingleColumnIfc);

	Bit#(1) zero = 0;

	// The mixing takes 3 XOR delays
	method Vector#(4, Bit#(8)) mixColumn(Vector#(4, Bit#(8)) val);
		Vector#(4, Bit#(8)) new_arr = newVector;

		Bit#(1) carry01 = val[0][7] ^ val[1][7];
		Bit#(8) simpleMultiply01 = (val[0] ^ val[1]) << 1;
		Bit#(8) mod01 = {zero, zero, zero, carry01, carry01, zero, carry01, carry01};
		new_arr[0] = simpleMultiply01 ^ mod01 ^ val[1] ^ val[2] ^ val[3];

		Bit#(1) carry12 = val[1][7] ^ val[2][7];
		Bit#(8) simpleMultiply12 = (val[1] ^ val[2]) << 1;
		Bit#(8) mod12 = {zero, zero, zero, carry12, carry12, zero, carry12, carry12};
		new_arr[1] = simpleMultiply12 ^ mod12 ^ val[0] ^ val[2] ^ val[3];

		Bit#(1) carry23 = val[2][7] ^ val[3][7];
		Bit#(8) simpleMultiply23 = (val[2] ^ val[3]) << 1;
		Bit#(8) mod23 = {zero, zero, zero, carry23, carry23, zero, carry23, carry23};
		new_arr[2] = simpleMultiply23 ^ mod23 ^ val[0] ^ val[1] ^ val[3];

		Bit#(1) carry30 = val[3][7] ^ val[0][7];
		Bit#(8) simpleMultiply30 = (val[3] ^ val[0]) << 1;
		Bit#(8) mod30 = {zero, zero, zero, carry30, carry30, zero, carry30, carry30};
		new_arr[3] = simpleMultiply30 ^ mod30 ^ val[0] ^ val[1] ^ val[2];

		return new_arr;
	endmethod

endmodule

endpackage