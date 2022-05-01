package TagGeneration;

import Vector::*;

import DeoxysBcEncrypt::*;

interface FinalizeTagIfc;

	method Vector#(16, Bit#(8)) finalizeTag(Vector#(32, Bit#(8)) key, Vector#(15, Bit#(8)) nonce, Vector#(16, Bit#(8)) tag);

endinterface

(* synthesize *)
module mkFinalizeTag(FinalizeTagIfc);

	DeoxysBcEncryptIfc encryption <- mkDeoxysBcEncrypt;

	method Vector#(16, Bit#(8)) finalizeTag(Vector#(32, Bit#(8)) key, Vector#(15, Bit#(8)) nonce, Vector#(16, Bit#(8)) tag);
		Vector#(16, Bit#(8)) tweak = newVector;
		tweak[00] = nonce[00];
		tweak[01] = nonce[01];
		tweak[02] = nonce[02];
		tweak[03] = nonce[03];
		tweak[04] = nonce[04];
		tweak[05] = nonce[05];
		tweak[06] = nonce[06];
		tweak[07] = nonce[07];
		tweak[08] = nonce[08];
		tweak[09] = nonce[09];
		tweak[10] = nonce[10];
		tweak[11] = nonce[11];
		tweak[12] = nonce[12];
		tweak[13] = nonce[13];
		tweak[14] = nonce[14];
		tweak[15] = 16;

		Vector#(16, Bit#(8)) result = encryption.getResult(key, tweak, tag);

		return result;
	endmethod

endmodule

endpackage