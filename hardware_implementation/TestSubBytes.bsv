package TestSubBytes;

import Vector::*;
import SubBytes::*;

(* synthesize *)
module mkTestSubBytes();

	SubBytesIfc sb <- mkByteSubstitution;

	Reg#(int) testNr <- mkReg(0);
	int length = 3;
	Vector#(16, Bit#(8)) testCase[3] = {replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f)};
	Vector#(16, Bit#(8)) expectation[3] = {replicate(0), unpack(128'h1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f), unpack(128'hd49ef045555c94af57c6869212bb5276)};

	Reg#(Bool) testSuccessful <- mkReg(True);
	Reg#(Bool) done <- mkReg(False);
	Reg#(Bool) check <- mkReg(False);
	Reg#(Bool) stop <- mkReg(False);



	rule testInput if (!check && testNr < length);
		sb.substitute(testCase[testNr]);
		check <= True;
	endrule

	rule checkTest if (check && testNr < length);
		Vector#(16, Bit#(8)) result = sb.getResult();
		Vector#(16, Bit#(8)) expected = expectation[testNr];
		$display("SubBytes Test %0d: %0h should be %0h", testNr, result, expected);
		testSuccessful <= testSuccessful && (pack(result) == pack(expected));
		testNr <= testNr + 1;
		check <= False;
	endrule

	rule fin if (!stop && testNr == length);
		$display("SubBytes Test succeeded: %s", testSuccessful ? "Success!" : "Failed!");
		stop <= True;
	endrule

endmodule

endpackage