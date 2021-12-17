
package Testing;

import Vector::*;

import TestIfc::*;

import TestInversionAndMultiplication::*;
import TestSubBytes::*;
import TestShiftRows::*;
import TestMixColumns::*;
import TestMixSingleColumn::*;
import TestNextTweakey::*;

(* synthesize *)
module mkTesting(TestIfc);

	TestIfc sub <- mkTestSubBytes;
	TestIfc inv <- mkTestInversionAndMultiplication;
	TestIfc shift <- mkTestShiftRows;
	TestIfc mixcols <- mkTestMixColumns;
	TestIfc mixsingle <- mkTestMixSingleColumn;
	TestIfc nextTK <- mkTestNextTweakey;

	TestIfc tests[6] = {sub, inv, shift, mixcols, mixsingle, nextTK};

	/*
		Configuration of the test
	*/
	String testName = "All tests";
	int length = 6;
	Bool finish = True;

	function Bool runTest(int testIndex);
		return tests[testIndex].getTestResult();
	endfunction

	/*
		Boilerplate code for testing single-stage
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

	method Bool getTestResult() if (stop);
		return testSuccessful;
	endmethod

endmodule

endpackage