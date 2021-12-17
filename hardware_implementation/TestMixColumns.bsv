package TestMixColumns;

import Vector::*;

import TestIfc::*;

import MixColumns::*;

(* synthesize *)
module mkTestMixColumns(TestIfc);

	MixColumnsIfc mcs <- mkMixColumns;

	Vector#(16, Bit#(8)) testCase[4] = {replicate(0), replicate(1), unpack(128'h455313db455313db455313db455313db), unpack(128'h4d0073642d8a67858a07af3d8dd3a791)};
	Vector#(16, Bit#(8)) expectation[4] = {replicate(0), replicate(1), unpack(128'hbca14d8ebca14d8ebca14d8ebca14d8e), unpack(128'h45c0cf10239ae31fe019fb1ddd072795)};

	/*
		Configuration of the test
	*/
	String testName = "MixColumns Test";
	int length = 4;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(16, Bit#(8)) result = mcs.mixColumns(testCase[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h", testName, testCase[testIndex]);
				$display("[%s]Test Result: %h", testName, mcs.mixColumns(testCase[testIndex]));
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