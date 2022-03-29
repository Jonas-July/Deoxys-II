package TestDeoxysBcRounds;

import Vector::*;

import TestIfc::*;

import DeoxysBcRounds::*;

(* synthesize *)
module mkTestDeoxysBcRound(TestIfc);

	DeoxysBcRoundIfc#(1) round1 <- mkDeoxysBcRound(1);

	Vector#(16, Bit#(8)) state[4] = {replicate(0), replicate(0), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) tk1[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) tk2[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) tk3[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(4, Vector#(16, Bit#(8))) expectation[4] = {	unpack(512'h6261676b3d3d3d3d6363636363636363000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000),
								unpack(512'h6360666a3c3c3c3c6262626262626262010101010101010101010101010101010202020202020202020202020202020280808080808080808080808080808080),
								unpack(512'h2b26212c27222d28232e29242f2a25202b26212c27222d28232e29242f2a25202b26212c27222d28232e29242f2a25202b26212c27222d28232e29242f2a2520),
								unpack(512'h1b06f1ecd7c2bda8938e79645f4a35201b06f1ecd7c2bda8938e79645f4a35201b06f1ecd7c2bda8938e79645f4a35201b06f1ecd7c2bda8938e79645f4a3520)};

	/*
		Configuration of the test
	*/
	String testName = "DeoxysBcRound Test";
	int length = 2;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(4, Vector#(16, Bit#(8))) result = round1.execute(state[testIndex], tk1[testIndex], tk2[testIndex], tk3[testIndex]);
		Vector#(4, Vector#(16, Bit#(8))) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Values : %h, %h, %h, %h", testName, state[testIndex], tk1[testIndex], tk2[testIndex], tk3[testIndex]);
				$display("[%s]Test Result: %h", testName, round1.execute(state[testIndex], tk1[testIndex], tk2[testIndex], tk3[testIndex]));
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