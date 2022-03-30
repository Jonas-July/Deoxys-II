package DeoxysBcRounds;

import Vector::*;

import AddSubtweakey::*;
import SubBytes::*;
import ShiftRows::*;
import MixColumns::*;

import NextTweakeys::*;

interface DeoxysBcRoundIfc #(numeric type round);

	method Vector#(4, Vector#(16, Bit#(8))) execute(Vector#(16, Bit#(8)) state, Vector#(16, Bit#(8)) tk1, Vector#(16, Bit#(8)) tk2, Vector#(16, Bit#(8)) tk3);

endinterface

module mkDeoxysBcRound #(parameter int round)(DeoxysBcRoundIfc#(round))
	provisos (Add#(1, unused, round),  // round >= 1
		  Add#(round, unused2, 16)); // round <= 16

	AddSubtweakeyIfc astk <- mkAddSubtweakey;
	SubBytesIfc#(16) sb <- mkByteSubstitution(16);
	ShiftRowsIfc sr <- mkShiftRows;
	MixColumnsIfc mcs <- mkMixColumns;

	NextTkIfc ntk1 <- mkNextTk1;
	NextTkIfc ntk2 <- mkNextTk2;
	NextTkIfc ntk3 <- mkNextTk3;

	method Vector#(4, Vector#(16, Bit#(8))) execute(Vector#(16, Bit#(8)) state, Vector#(16, Bit#(8)) tk1, Vector#(16, Bit#(8)) tk2, Vector#(16, Bit#(8)) tk3);
		Vector#(16, Bit#(8)) tmp_state = sb.substitute(state);
		tmp_state = sr.shiftRows(tmp_state);
		tmp_state = mcs.mixColumns(tmp_state);
		tk1 = ntk1.nextTweakey(tk1);
		tk2 = ntk2.nextTweakey(tk2);
		tk3 = ntk3.nextTweakey(tk3);
		tmp_state = astk.addSubtweakey(tmp_state, tk1, tk2, tk3, round);

		Vector#(4, Vector#(16, Bit#(8))) vec = newVector;
		vec[3] = tmp_state;
		vec[2] = tk1;
		vec[1] = tk2;
		vec[0] = tk3;
		return vec;
	endmethod

endmodule


endpackage