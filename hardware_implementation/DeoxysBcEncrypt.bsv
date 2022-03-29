package DeoxysBcEncrypt;

import Vector::*;

import AddSubtweakey::*;
import DeoxysBcRounds::*;

interface DeoxysBcEncryptIfc;

	method Vector#(16, Bit#(8)) getResult(Vector#(32, Bit#(8)) key, Vector#(16, Bit#(8)) tweak, Vector#(16, Bit#(8)) plaintext);

endinterface

module mkDeoxysBcEncrypt(DeoxysBcEncryptIfc);

	AddSubtweakeyIfc astk <- mkAddSubtweakey;
	Vector#(16, DeoxysBcRoundIfc#(1)) rounds <- mapM(mkDeoxysBcRound, map(fromInteger, genVector));

	method Vector#(16, Bit#(8)) getResult(Vector#(32, Bit#(8)) key, Vector#(16, Bit#(8)) tweak, Vector#(16, Bit#(8)) plaintext);
		Vector#(16, Bit#(8)) internal_state = plaintext;
		Vector#(16, Bit#(8)) tk3 = unpack(pack(key)[127:0]);
		Vector#(16, Bit#(8)) tk2 = unpack(pack(key)[255:128]);
		Vector#(16, Bit#(8)) tk1 = tweak;

		Vector#(4, Vector#(16, Bit#(8))) round_result = newVector;
		round_result[3] = internal_state;
		round_result[2] = tk1;
		round_result[1] = tk2;
		round_result[0] = tk3;

		round_result[3] = astk.addSubtweakeyRound0(round_result[3], round_result[2], round_result[1], round_result[0]);
		for (int i=0; i < 16; i=i+1)
			round_result = rounds[i].execute(round_result[3], round_result[2], round_result[1], round_result[0]);

		return round_result[3];
	endmethod

endmodule


endpackage