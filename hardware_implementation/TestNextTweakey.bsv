package TestNextTweakey;

import Vector::*;

import TestIfc::*;

import NextTweakeys::*;

(* synthesize *)
module mkTestNextTweakey(TestIfc);

	NextTkIfc ntk1 <- mkNextTk1;
	NextTkIfc ntk2 <- mkNextTk2;
	NextTkIfc ntk3 <- mkNextTk3;

	Vector#(16, Bit#(8)) testCase[12] = {	replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120),
						replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120),
						replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) expectation[12] = {replicate(0), replicate(1), unpack(128'h2827222d24232e29202f2a252c2b2621), unpack(128'ha8d7c2bd64938e79205f4a35ec1b06f1),
						replicate(0), replicate(2), unpack(128'h514f455b49475d53415f554b59574d43), unpack(128'h50af857ac9271df341be946bd8360ce2),
						replicate(0), replicate(8'h80), unpack(128'h14931196129117941097159216951390), unpack(128'h546be1deb2c9473c102fa59af68d0378)};

	/*
		Configuration of the test
	*/
	String testName = "Next Tweakey Test";
	int length = 12;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);
		NextTkIfc uut =   testIndex / 4 == 0 ? ntk1
				: testIndex / 4 == 1 ? ntk2
						     : ntk3;
		Vector#(16, Bit#(8)) result = uut.nextTweakey(testCase[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		NextTkIfc uut =   testIndex / 4 == 0 ? ntk1
				: testIndex / 4 == 1 ? ntk2
						     : ntk3;
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h", testName, testCase[testIndex]);
				$display("[%s]Test Result: %h", testName, uut.nextTweakey(testCase[testIndex]));
			endaction;
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