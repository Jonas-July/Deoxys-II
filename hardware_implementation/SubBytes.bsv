package SubBytes;

import Vector::*;

import InversionAndMultiplication::*;


interface SubBytesIfc #(numeric type bytes);

	method Vector#(bytes, Bit#(8)) substitute(Vector#(bytes, Bit#(8)) in);

endinterface

module mkByteSubstitution #(parameter int bytes)(SubBytesIfc#(bytes))
	provisos (Add#(0, unused, bytes)); // bytes >= 0

	Vector#(bytes, InversionAndMultiplicationIfc) inverter <- replicateM(mkInversionAndMultiplication);

	method Vector#(bytes, Bit#(8)) substitute(Vector#(bytes, Bit#(8)) in);
		Vector#(bytes, Bit#(8)) result = newVector;
		for (int i=0; i < bytes; i=i+1)
			result[i] = inverter[i].invertAndMultiply(in[i]);
		return result;
	endmethod

endmodule


endpackage