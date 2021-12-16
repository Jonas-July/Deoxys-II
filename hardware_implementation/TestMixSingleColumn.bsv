package TestMixSingleColumn;

import Vector::*;

import TestIfc::*;

import MixSingleColumn::*;

(* synthesize *)
module mkTestMixSingleColumn(TestIfc);

	MixSingleColumnIfc msc <- mkMixSingleColumn;

	Vector#(4, Bit#(8)) testCase[23] = {replicate(0), replicate(1), unpack(32'h455313db), unpack(32'h5c220af2), unpack(32'hc6c6c6c6), unpack(32'hd5d4d4d4), unpack(32'h4c31262d),
						unpack(32'h4d007364), unpack(32'h317155a2), unpack(32'h2b38e14c), unpack(32'h212082cc),
						unpack(32'h1fb51e57), unpack(32'h2d8a6785), unpack(32'h47cce53f), unpack(32'h28f4ada3),
						unpack(32'hc97f642e), unpack(32'he2654cd8), unpack(32'h8a07af3d), unpack(32'h830382ee),
						unpack(32'hec90533c), unpack(32'hfe890097), unpack(32'hca97854d), unpack(32'h8dd3a791)};
	Vector#(4, Bit#(8)) expectation[23] = {replicate(0), replicate(1), unpack(32'hbca14d8e), unpack(32'h9d58dc9f), unpack(32'hc6c6c6c6), unpack(32'hd6d7d5d5), unpack(32'hf8bd7e4d),
						unpack(32'h45c0cf10), unpack(32'hbb46aae0), unpack(32'h5ba0f6b3), unpack(32'haf6d921f),
						unpack(32'h6c19b026), unpack(32'h239ae31f), unpack(32'he690e6c1), unpack(32'hf785cd6d),
						unpack(32'he0f4ae46), unpack(32'h85630df8), unpack(32'he019fb1d), unpack(32'hb5f477da),
						unpack(32'h447bddf1), unpack(32'hcc87e942), unpack(32'h4ab83453), unpack(32'hdd072795)};

	/*
		Configuration of the test
	*/
	String testName = "MixSingleColumn Test";
	int length = 23;
	Bool finish = False;
	Bool displayTests = True;

	function Bool runTest(int testIndex);

		Vector#(4, Bit#(8)) result = msc.mixColumn(testCase[testIndex]);
		Vector#(4, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h", testName, testCase[testIndex][0][3:0]);
				$display("[%s]Test Result: %h", testName, msc.mixColumn(testCase[testIndex]));
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
