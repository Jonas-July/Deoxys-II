package ShiftRows;

import Vector::*;

interface ShiftRowsIfc;

	method Vector#(16, Bit#(8)) shiftRows(Vector#(16, Bit#(8)) val);

endinterface

(* synthesize *)
module mkShiftRows(ShiftRowsIfc);


	method Vector#(16, Bit#(8)) shiftRows(Vector#(16, Bit#(8)) val);
		Vector#(16, Bit#(8)) new_arr = newVector;
		new_arr[00] = val[0];
		new_arr[01] = val[5];
		new_arr[02] = val[10];
		new_arr[03] = val[15];
		new_arr[04] = val[4];
		new_arr[05] = val[9];
		new_arr[06] = val[14];
		new_arr[07] = val[3];
		new_arr[08] = val[8];
		new_arr[09] = val[13];
		new_arr[10] = val[2];
		new_arr[11] = val[7];
		new_arr[12] = val[12];
		new_arr[13] = val[1];
		new_arr[14] = val[6];
		new_arr[15] = val[11];
		return new_arr;
	endmethod

endmodule

endpackage