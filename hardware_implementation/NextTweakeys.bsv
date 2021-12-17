package NextTweakeys;

import Vector::*;

interface NextTkIfc;

	method Vector#(16, Bit#(8)) nextTweakey(Vector#(16, Bit#(8)) oldTk);

endinterface

(* synthesize *)
module mkNextTk1(NextTkIfc);


	method Vector#(16, Bit#(8)) nextTweakey(Vector#(16, Bit#(8)) oldTk);
		Vector#(16, Bit#(8)) new_arr = newVector;
		// h = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};
		new_arr[00] = oldTk[01];
		new_arr[01] = oldTk[06];
		new_arr[02] = oldTk[11];
		new_arr[03] = oldTk[12];

		new_arr[04] = oldTk[05];
		new_arr[05] = oldTk[10];
		new_arr[06] = oldTk[15];
		new_arr[07] = oldTk[00];

		new_arr[08] = oldTk[09];
		new_arr[09] = oldTk[14];
		new_arr[10] = oldTk[03];
		new_arr[11] = oldTk[04];

		new_arr[12] = oldTk[13];
		new_arr[13] = oldTk[02];
		new_arr[14] = oldTk[07];
		new_arr[15] = oldTk[08];

		return new_arr;
	endmethod

endmodule

(* synthesize *)
module mkNextTk2(NextTkIfc);

	function Bit#(8) lfsr2(Bit#(8) val);
		return {val[6], val[5], val[4], val[3], val[2], val[1], val[0], val[7] ^ val[5]};
	endfunction

	method Vector#(16, Bit#(8)) nextTweakey(Vector#(16, Bit#(8)) oldTk);
		Vector#(16, Bit#(8)) new_arr = newVector;
		// h = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};
		new_arr[00] = oldTk[01];
		new_arr[01] = oldTk[06];
		new_arr[02] = oldTk[11];
		new_arr[03] = oldTk[12];

		new_arr[04] = oldTk[05];
		new_arr[05] = oldTk[10];
		new_arr[06] = oldTk[15];
		new_arr[07] = oldTk[00];

		new_arr[08] = oldTk[09];
		new_arr[09] = oldTk[14];
		new_arr[10] = oldTk[03];
		new_arr[11] = oldTk[04];

		new_arr[12] = oldTk[13];
		new_arr[13] = oldTk[02];
		new_arr[14] = oldTk[07];
		new_arr[15] = oldTk[08];

		return map(lfsr2, new_arr);
	endmethod

endmodule

(* synthesize *)
module mkNextTk3(NextTkIfc);

	function Bit#(8) lfsr3(Bit#(8) val);
		return {val[0] ^ val[6], val[7], val[6], val[5], val[4], val[3], val[2], val[1]};
	endfunction

	method Vector#(16, Bit#(8)) nextTweakey(Vector#(16, Bit#(8)) oldTk);
		Vector#(16, Bit#(8)) new_arr = newVector;
		// h = {1, 6, 11, 12, 5, 10, 15, 0, 9, 14, 3, 4, 13, 2, 7, 8};
		new_arr[00] = oldTk[01];
		new_arr[01] = oldTk[06];
		new_arr[02] = oldTk[11];
		new_arr[03] = oldTk[12];

		new_arr[04] = oldTk[05];
		new_arr[05] = oldTk[10];
		new_arr[06] = oldTk[15];
		new_arr[07] = oldTk[00];

		new_arr[08] = oldTk[09];
		new_arr[09] = oldTk[14];
		new_arr[10] = oldTk[03];
		new_arr[11] = oldTk[04];

		new_arr[12] = oldTk[13];
		new_arr[13] = oldTk[02];
		new_arr[14] = oldTk[07];
		new_arr[15] = oldTk[08];

		return map(lfsr3, new_arr);
	endmethod

endmodule

endpackage