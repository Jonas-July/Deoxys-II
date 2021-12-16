package TestMixColumns;

import Vector::*;

import TestIfc::*;

//import ShiftRows::*;

(* synthesize *)
module mkTestMixColumns(TestIfc);

	//ShiftRowsIfc sr <- mkShiftRows;

	Vector#(16, Bit#(8)) testCase[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) expectation[4] = {replicate(0), replicate(1), unpack(128'h2b26212c27222d28232e29242f2a2520), unpack(128'h1b06f1ecd7c2bda8938e79645f4a3520)};

	/*
		Configuration of the test
	*/
	String testName = "MixColumns Test";
	int length = 0;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		//Vector#(16, Bit#(8)) result = sr.shiftRows(testCase[testIndex]);
		//Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return True;//(pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return action endaction;
	endfunction

	/*
		Boilerplate code for testing single-stage with displaying data
	*/
	Reg#(int) testNr <- mkReg(0);

	Reg#(Bool) testSuccessful <- mkReg(True);
	Reg#(Bool) stop <- mkReg(False);

	rule testing if (testNr < length);
		testSuccessful <= testSuccessful && runTest(testNr);
		testNr <= testNr + 1;
	endrule

	rule fin if (!stop && testNr == length);
		$display("%s succeeded: %s", testName, testSuccessful ? "Success!" : "Failed!");
		stop <= True;
		endrule

	rule end_test if (stop && finish);
		$finish();
	endrule

	rule display if (testNr < length && displayTests);
		displayTestData(testName, testNr);
	endrule

	method Bool getTestResult() if (stop);
		return testSuccessful;
	endmethod

endmodule

endpackage