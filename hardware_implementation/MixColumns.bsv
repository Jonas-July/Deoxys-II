package MixColumns;

import Vector::*;

import MixSingleColumn::*;

interface MixColumnsIfc;

	method Vector#(16, Bit#(8)) mixColumns(Vector#(16, Bit#(8)) val);

endinterface

(* synthesize *)
module mkMixColumns(MixColumnsIfc);

	MixSingleColumnIfc msc0 <- mkMixSingleColumn;
	MixSingleColumnIfc msc1 <- mkMixSingleColumn;
	MixSingleColumnIfc msc2 <- mkMixSingleColumn;
	MixSingleColumnIfc msc3 <- mkMixSingleColumn;

	method Vector#(16, Bit#(8)) mixColumns(Vector#(16, Bit#(8)) val);
		Vector#(16, Bit#(8)) new_arr = newVector;

		Vector#(4, Bit#(8)) temp = newVector;
		Vector#(4, Bit#(8)) res = newVector;
		temp[0] = val[0];
		temp[1] = val[1];
		temp[2] = val[2];
		temp[3] = val[3];
		res = msc0.mixColumn(temp);
		new_arr[0] = res[0];
		new_arr[1] = res[1];
		new_arr[2] = res[2];
		new_arr[3] = res[3];

		Vector#(4, Bit#(8)) temp2 = newVector;
		Vector#(4, Bit#(8)) res2 = newVector;
		temp2[0] = val[4];
		temp2[1] = val[5];
		temp2[2] = val[6];
		temp2[3] = val[7];
		res2 = msc1.mixColumn(temp2);
		new_arr[4] = res2[0];
		new_arr[5] = res2[1];
		new_arr[6] = res2[2];
		new_arr[7] = res2[3];

		Vector#(4, Bit#(8)) temp3 = newVector;
		Vector#(4, Bit#(8)) res3 = newVector;
		temp3[0] = val[8];
		temp3[1] = val[9];
		temp3[2] = val[10];
		temp3[3] = val[11];
		res3 = msc2.mixColumn(temp3);
		new_arr[8] = res3[0];
		new_arr[9] = res3[1];
		new_arr[10] = res3[2];
		new_arr[11] = res3[3];

		Vector#(4, Bit#(8)) temp4 = newVector;
		Vector#(4, Bit#(8)) res4 = newVector;
		temp4[0] = val[12];
		temp4[1] = val[13];
		temp4[2] = val[14];
		temp4[3] = val[15];
		res4 = msc3.mixColumn(temp4);
		new_arr[12] = res4[0];
		new_arr[13] = res4[1];
		new_arr[14] = res4[2];
		new_arr[15] = res4[3];

		return new_arr;
	endmethod

endmodule

endpackage