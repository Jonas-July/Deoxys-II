package AddSubtweakey;

import Vector::*;

interface AddSubtweakeyIfc;

	method Vector#(16, Bit#(8)) addSubtweakey(Vector#(16, Bit#(8)) val, Vector#(16, Bit#(8)) tk1, Vector#(16, Bit#(8)) tk2, Vector#(16, Bit#(8)) tk3, int round);

endinterface

(* synthesize *)
module mkAddSubtweakey(AddSubtweakeyIfc);

	Bit#(8) rcon[17] = { 8'h4c, 8'h3d, 8'hdf, 8'h00, 8'ha5, 8'hf4, 8'h56, 8'h09, 8'hb7, 8'hd0, 8'h1e, 8'h99, 8'h8c, 8'ha6, 8'hf2, 8'h5a, 8'h11 };

	method Vector#(16, Bit#(8)) addSubtweakey(Vector#(16, Bit#(8)) val, Vector#(16, Bit#(8)) tk1, Vector#(16, Bit#(8)) tk2, Vector#(16, Bit#(8)) tk3, int round);
		Bit#(8) constant = rcon[round];
		Vector#(16, Bit#(8)) new_arr = unpack(pack(val) ^ pack(tk1) ^ pack(tk2) ^ pack(tk3) ^ pack({8'h62, 8'h61, 8'h67, 8'h6b, constant, constant, constant, constant, 8'h63, 8'h63, 8'h63, 8'h63, 8'h63, 8'h63, 8'h63, 8'h63}));
		return new_arr;
	endmethod

endmodule

endpackage