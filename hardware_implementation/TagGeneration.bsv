package TagGeneration;

import Vector::*;

import DeoxysBcEncrypt::*;

interface FinalizeTagIfc;

	method Vector#(16, Bit#(8)) finalizeTag(Vector#(32, Bit#(8)) key, Vector#(15, Bit#(8)) nonce, Vector#(16, Bit#(8)) tag);

endinterface

interface ComputePartialTagIfc #(numeric type idxbits);

	method Vector#(16, Bit#(8)) calcPartialTag(Vector#(32, Bit#(8)) key, UInt#(idxbits) index, Vector#(16, Bit#(8)) plaintext);

endinterface

interface ComputePartialTagMsgIfc;
	interface ComputePartialTagIfc#(120) routine;
endinterface

interface ComputePartialTagAddIfc;
	interface ComputePartialTagIfc#(120) routine;
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

(* synthesize *)
module mkComputePartialTagMsg(ComputePartialTagMsgIfc);

	Bit#(4) ord_bits = 4'b0000;
	DeoxysBcEncryptIfc encryption <- mkDeoxysBcEncrypt;

	interface ComputePartialTagIfc routine;
		method Vector#(16, Bit#(8)) calcPartialTag(Vector#(32, Bit#(8)) key, UInt#(120) index, Vector#(16, Bit#(8)) plaintext);
			Vector#(16, Bit#(8)) tweak = newVector;
			Vector#(16, Bit#(8)) i = unpack(pack(zeroExtend(index)));
			tweak[00] = i[00];
			tweak[01] = i[01];
			tweak[02] = i[02];
			tweak[03] = i[03];
			tweak[04] = i[04];
			tweak[05] = i[05];
			tweak[06] = i[06];
			tweak[07] = i[07];
			tweak[08] = i[08];
			tweak[09] = i[09];
			tweak[10] = i[10];
			tweak[11] = i[11];
			tweak[12] = i[12];
			tweak[13] = i[13];
			tweak[14] = i[14];
			tweak[15] = {ord_bits, 4'b0000};

			Vector#(16, Bit#(8)) result = encryption.getResult(key, tweak, plaintext);

			return reverse(result);
		endmethod
	endinterface

endmodule

(* synthesize *)
module mkComputePartialTagAdd(ComputePartialTagAddIfc);

	Bit#(4) ord_bits = 4'b0010;
	DeoxysBcEncryptIfc encryption <- mkDeoxysBcEncrypt;

	interface ComputePartialTagIfc routine;
		method Vector#(16, Bit#(8)) calcPartialTag(Vector#(32, Bit#(8)) key, UInt#(120) index, Vector#(16, Bit#(8)) plaintext);
			Vector#(16, Bit#(8)) tweak = newVector;
			Vector#(16, Bit#(8)) i = unpack(pack(zeroExtend(index)));
			tweak[00] = i[00];
			tweak[01] = i[01];
			tweak[02] = i[02];
			tweak[03] = i[03];
			tweak[04] = i[04];
			tweak[05] = i[05];
			tweak[06] = i[06];
			tweak[07] = i[07];
			tweak[08] = i[08];
			tweak[09] = i[09];
			tweak[10] = i[10];
			tweak[11] = i[11];
			tweak[12] = i[12];
			tweak[13] = i[13];
			tweak[14] = i[14];
			tweak[15] = {ord_bits, 4'b0000};

			Vector#(16, Bit#(8)) result = encryption.getResult(key, tweak, plaintext);

			return reverse(result);
		endmethod
	endinterface

endmodule

endpackage