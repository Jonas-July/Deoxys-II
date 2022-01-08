package TestSubBytes;

import Vector::*;

import TestIfc::*;

import SubBytes::*;

(* synthesize *)
module mkTestSubBytes(TestIfc);

	SubBytesIfc#(16) sb <- mkByteSubstitution(16);

	Vector#(16, Bit#(8)) testCase[3] = {replicate(0), replicate(1), unpack(128'h202122232425262728292a2b2c2d2e2f)};
	Vector#(16, Bit#(8)) expectation[3] = {replicate(0), unpack(128'h1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f1f), unpack(128'hd49ef045555c94af57c6869212bb5276)};

	/*
		Configuration of the test
	*/
	String testName = "SubBytes Test";
	int length = 3;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(16, Bit#(8)) result = sb.substitute(testCase[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h", testName, testCase[testIndex]);
				$display("[%s]Test Result: %h", testName, sb.substitute(testCase[testIndex]));
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

/*
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
	/
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

//*/

endmodule

endpackage