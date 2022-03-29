package TestDeoxysBcEncrypt;

import Vector::*;

import TestIfc::*;

import DeoxysBcEncrypt::*;

(* synthesize *)
module mkTestDeoxysBcEncrypt(TestIfc);

	DeoxysBcEncryptIfc encryption <- mkDeoxysBcEncrypt;

	Vector#(32, Bit#(8)) keys[4] = {replicate(0), replicate(0), unpack(256'h2f2e2d2c2b2a292827262524232221202f2e2d2c2b2a29282726252423222120), unpack(256'h5f8ebdec1b4a79a8d706356493c2f1205f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) tweaks[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) plaintexts[4] = {replicate(0), replicate(1), unpack(128'h2f2e2d2c2b2a29282726252423222120), unpack(128'h5f8ebdec1b4a79a8d706356493c2f120)};
	Vector#(16, Bit#(8)) expectation[4] = {	unpack(128'h2f2e2d2c2b2a29282726252423222120),
						unpack(128'h2f2e2d2c2b2a29282726252423222120),
						unpack(128'h2f2e2d2c2b2a29282726252423222120),
						unpack(128'h2f2e2d2c2b2a29282726252423222120)};

	/*
		Configuration of the test
	*/
	String testName = "DeoxysBcEncrypt Test";
	int length = 2;
	Bool finish = False;
	Bool displayTests = True;

	function Bool runTest(int testIndex);

		Vector#(16, Bit#(8)) result = encryption.getResult(keys[testIndex], tweaks[testIndex], plaintexts[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Values : %h, %h, %h", testName, keys[testIndex], tweaks[testIndex], plaintexts[testIndex]);
				$display("[%s]Test Result: %h", testName, encryption.getResult(keys[testIndex], tweaks[testIndex], plaintexts[testIndex]));
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