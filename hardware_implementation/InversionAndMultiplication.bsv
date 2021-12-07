package InversionAndMultiplication;

interface InversionAndMultiplicationIfc;

	method Bit#(8) invertAndMultiply(Bit#(8) val);

endinterface


(* synthesize *)
module mkInversionAndMultiplication(InversionAndMultiplicationIfc);

	method Bit#(8) invertAndMultiply(Bit#(8) input_bit);

		// Isomorphic transformation
		Bit #(8) val;
		val[0] = (input_bit[1] ^ input_bit[3] ^ input_bit[4] ^ input_bit[5]);
		val[1] = (input_bit[0] ^ input_bit[2] ^ input_bit[6] ^ input_bit[7]);
		val[2] = (input_bit[0] ^ input_bit[3] ^ input_bit[7]               );
		val[3] = (input_bit[5]                                             );
		val[4] = (input_bit[1] ^ input_bit[2] ^ input_bit[4] ^ input_bit[5]);
		val[5] = (input_bit[0] ^ input_bit[2] ^ input_bit[4]               );
		val[6] = (input_bit[0] ^ input_bit[1] ^ input_bit[2] ^ input_bit[7]);
		val[7] = (input_bit[2] ^ input_bit[3] ^ input_bit[7]               );

		// Preparation
		Bit#(5) l = {val[3], val[2], val[1], val[0], 0};
		Bit#(5) h = {val[7], val[6], val[5], val[4], 0};

		// Stage 1
		Bit#(6) lTable = {      l[3] ^ l[4],
					l[2] ^ l[4], l[2] ^ l[3],
					l[1] ^ l[4], l[1] ^ l[3], l[1] ^ l[2] };
		Bit#(6) hTable = {      h[3] ^ h[4],
					h[2] ^ h[4], h[2] ^ h[3],
					h[1] ^ h[4], h[1] ^ h[3], h[1] ^ h[2] };

		Bit#(5) d = 0;
		d[0] = (hTable[0] | lTable[0]) ^ (hTable[5] | lTable[5]) ^    (h[2] | l[2])    ^ (h[3] & l[3]);
		d[1] = (hTable[0] | lTable[0]) ^ (hTable[1] & lTable[1]) ^    (h[3] | l[3])    ^ (h[4] | l[4]);
		d[2] = (hTable[1] | lTable[1]) ^ (hTable[2] & lTable[2]) ^ (hTable[3] | lTable[3]) ^ (h[4] | l[4]);
		d[3] = (hTable[2] | lTable[2]) ^ (hTable[3] | lTable[3]) ^ (hTable[4] & lTable[4]) ^ (h[1] | l[1]);
		d[4] = (hTable[4] | lTable[4]) ^ (hTable[5] | lTable[5]) ^    (h[1] | l[1])    ^ (h[2] & l[2]);

		// Stage 2
		Bit#(8) e = 0;
		e[0] = (d[1] | d[4]) & (d[2] | d[3]);
		e[1] = ((d[4] ^ 1) & (d[1] ^ d[2])) | ((d[0] & d[4]) & (d[2] | d[3]));
		e[2] = ((d[3] ^ 1) & (d[2] ^ d[4])) | ((d[0] & d[3]) & (d[1] | d[4]));
		e[3] = ((d[2] ^ 1) & (d[1] ^ d[3])) | ((d[0] & d[2]) & (d[1] | d[4]));
		e[4] = ((d[1] ^ 1) & (d[3] ^ d[4])) | ((d[0] & d[1]) & (d[2] | d[3]));

 		// Stage 3
		Bit#(10) fTable = {     e[3] ^ e[4],
					e[2] ^ e[4], e[2] ^ e[3],
					e[1] ^ e[4], e[1] ^ e[3], e[1] ^ e[2],
					e[0] ^ e[4], e[0] ^ e[3], e[0] ^ e[2], e[0] ^ e[1]};


		Bit#(8) res_h = 0;
		res_h[0] = (lTable[2] & fTable[6]) ^ (lTable[3] & fTable[7]);
		res_h[1] = (l[1] & fTable[0]) ^ (lTable[4] & fTable[8]);
		res_h[2] = (l[2] & fTable[1]) ^ (lTable[5] & fTable[9]);
		res_h[3] = (l[3] & fTable[2]) ^ (lTable[0] & fTable[4]);
		res_h[4] = (l[4] & fTable[3]) ^ (lTable[1] & fTable[5]);

		Bit#(5) res_l = 0;
		res_l[0] = (hTable[2] & fTable[6]) ^ (hTable[3] & fTable[7]);
		res_l[1] = (h[1] & fTable[0]) ^ (hTable[4] & fTable[8]);
		res_l[2] = (h[2] & fTable[1]) ^ (hTable[5] & fTable[9]);
		res_l[3] = (h[3] & fTable[2]) ^ (hTable[0] & fTable[4]);
		res_l[4] = (h[4] & fTable[3]) ^ (hTable[1] & fTable[5]);

		Bit#(8) result = 0;
		result[0] = (res_l[0] ^ res_l[1] ^ res_l[2] ^ res_l[3]  ^ res_h[0] ^ res_h[3]                      );
		result[1] = (res_l[2] ^ res_l[3]			^ res_h[0] ^ res_h[1] ^ res_h[2] ^ res_h[4]);
		result[2] = (res_l[0] ^ res_l[1] ^ res_l[2] ^ res_l[3]  ^ res_h[1] ^ res_h[3]                      );
		result[3] = (res_l[0] ^ res_l[4]			^ res_h[0] ^ res_h[1] ^ res_h[2] ^ res_h[4]);
		result[4] = (res_l[0] ^ res_l[1] ^ res_l[3] ^ res_l[4]  ^ res_h[0] ^ res_h[4]                      );
		result[5] = (res_l[0] ^ res_l[4]			^ res_h[0] ^ res_h[2] ^ res_h[3] ^ res_h[4]);
		result[6] = (res_l[2] ^ res_l[4]			^ res_h[2] ^ res_h[4]		      );
		result[7] = (res_l[0] ^ res_l[2]			^ res_h[0] ^ res_h[1] ^ res_h[3] ^ res_h[4]);

		return result;
	endmethod

endmodule

endpackage