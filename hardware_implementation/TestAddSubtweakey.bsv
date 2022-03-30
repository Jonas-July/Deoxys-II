package TestAddSubtweakey;

import Vector::*;

import TestIfc::*;

import AddSubtweakey::*;

(* synthesize *)
module mkTestAddSubtweakey(TestIfc);

	AddSubtweakeyIfc astk <- mkAddSubtweakey;

	Vector#(16, Bit#(8)) testInternalState[4] = 	{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK1[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK2[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	Vector#(16, Bit#(8)) testTK3[4] = 		{replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f), unpack(128'h20f1c293643506d7a8794a1becbd8e5f)};
	int		   testRound[4] = {0, 1, 2, 3};
	Vector#(16, Bit#(8)) expectation[4] = {unpack(128'h00000000000000002f2f2f2f08040201), unpack(128'h63636363636363633d3d3d3d6b676162), unpack(128'h6363636363636363dfdfdfdf6b676162), unpack(128'h6363636363636363000000006b676162)};

	/*
		Configuration of the test
	*/
	String testName = "AddSubtweakey Test";
	int length = 4;
	Bool finish = False;
	Bool displayTests = False;

	function Vector#(16, Bit#(8)) getResult(int testIndex);
		Vector#(16, Bit#(8)) result = newVector;
		if (testRound[testIndex] == 0) 	result = astk.addSubtweakeyRound0(testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex]);
		else 				result = astk.addSubtweakey(testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex], testRound[testIndex]);
		return result;
	endfunction

	function Bool runTest(int testIndex);
		Vector#(16, Bit#(8)) result = getResult(testIndex);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Values: %h, %h, %h, %h", testName, testInternalState[testIndex], testTK1[testIndex], testTK2[testIndex], testTK3[testIndex]);
				$display("[%s]Test Result: %h", testName, getResult(testIndex));
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