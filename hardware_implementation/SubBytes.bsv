package SubBytes;

import Vector::*;

import InversionAndMultiplication::*;


interface SubBytesIfc;

	method Action substitute(Vector#(16, Bit#(8)) in);
	method Vector#(16, Bit#(8)) getResult();

endinterface


module mkByteSubstitution(SubBytesIfc);

	InversionAndMultiplicationIfc iam <- mkInversionAndMultiplication;
	InversionAndMultiplicationIfc iam2 <- mkInversionAndMultiplication;

	Reg#(UInt#(5)) counter <- mkReg(0); 
	Reg#(Vector#(16, Bit#(8))) current <- mkReg(newVector);

	rule computeNext if (counter < 16);
		current[counter] <= iam.invertAndMultiply(current[counter]);
	endrule

	rule count if (counter < 16);
		counter <= counter + 1;
	endrule

	method Action substitute(Vector#(16, Bit#(8)) in) if (counter == 16);
		current <= in;
		counter <= 0;
	endmethod

	method Vector#(16, Bit#(8)) getResult() if (counter == 16);
		return current;
	endmethod

endmodule


endpackage