package TestSubBytes;

import Vector::*;

import TestIfc::*;

import SubBytes::*;

(* synthesize *)
module mkTestSubBytes(TestIfc);

	SubBytesIfc sb <- mkByteSubstitution;

	Vector#(16, Bit#(8)) testCase[3] = {replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f)};
	Vector#(16, Bit#(8)) expectation[3] = {replicate(0), unpack(128'h1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f), unpack(128'hd49ef045555c94af57c6869212bb5276)};

	/*
		Configuration of the test
	*/
	String testName = "SubBytes Test";
	int length = 3;
	Bool finish = False;

	function Action startTest(int testIndex);
		return
			action
				sb.substitute(testCase[testIndex]);
			endaction;
	endfunction

	function Bool checkTest(int testIndex);
		Vector#(16, Bit#(8)) result = sb.getResult();
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		//$display("SubBytes Test %0d: %0h should be %0h", testIndex, result, expected);
		return (pack(result) == pack(expected));
	endfunction

	/*
		Boilerplate code for testing multi-stage
	*/
	Reg#(int) testNr <- mkReg(0);

	Reg#(Bool) testSuccessful <- mkReg(True);
	Reg#(Bool) check <- mkReg(False);
	Reg#(Bool) stop <- mkReg(False);

	rule starting if (!check && testNr < length);
		startTest(testNr);
		check <= True;
	endrule

	rule checking if (check && testNr < length);
		testSuccessful <= testSuccessful && checkTest(testNr);
		testNr <= testNr + 1;
		check <= False;
	endrule

	rule fin if (!stop && testNr == length);
		$display("%s succeeded: %s", testName, testSuccessful ? "Success!" : "Failed!");
		stop <= True;
	endrule

	rule end_test if (stop && finish);
		$finish();
	endrule

	method Bool getTestResult() if (stop);
		return testSuccessful;
	endmethod

endmodule

endpackage