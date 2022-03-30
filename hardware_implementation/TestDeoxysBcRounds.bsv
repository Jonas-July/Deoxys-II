package TestDeoxysBcRounds;

import Vector::*;

import TestIfc::*;

import DeoxysBcRounds::*;

(* synthesize *)
module mkTestDeoxysBcRound(TestIfc);

	DeoxysBcRoundIfc#(1) round1 <- mkDeoxysBcRound(1);

	Vector#(16, Bit#(8)) testInternalState[4] = 	{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK1[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK2[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK3[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(4, Vector#(16, Bit#(8))) expectation[4] = {	unpack(512'h63636363636363633d3d3d3d6b676162000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000),
								unpack(512'hffffffffffffffffa1a1a1a1f7fbfdfe010101010101010101010101010101010202020202020202020202020202020280808080808080808080808080808080),
								unpack(512'h697166fa85cb44b9595bcb436d37def327282d222b2c21262f20252a2324292e4f515b455759434d5f414b554749535d93149611951690139710921591129417),
								unpack(512'h45db75db724063c233ffddbe0af7da10d7a8bdc21becf1065f20354a9364798eaf507a8536d8e20cbe416b9427c9f31d6b54dee18df678032f109aa5c9b23c47)};

	/*
		Configuration of the test
	*/
	String testName = "DeoxysBcRound Test";
	int length = 4;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(4, Vector#(16, Bit#(8))) result = round1.execute(testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex]);
		Vector#(4, Vector#(16, Bit#(8))) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Values: %h, %h, %h, %h", testName, testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex]);
				$display("[%s]Test Result: %h", testName, round1.execute(testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex]));
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