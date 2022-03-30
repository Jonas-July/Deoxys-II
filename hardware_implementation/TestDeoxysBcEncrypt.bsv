package TestDeoxysBcEncrypt;

import Vector::*;

import TestIfc::*;

import DeoxysBcEncrypt::*;

(* synthesize *)
module mkTestDeoxysBcEncrypt(TestIfc);

	DeoxysBcEncryptIfc encryption <- mkDeoxysBcEncrypt;

	Vector#(32, Bit#(8)) keys[4] = {unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f)};
	Vector#(16, Bit#(8)) tweaks[4] =      {unpack(128'h10202122232425262728292a2b2c2d2e), unpack(128'h20000000000000000000000000000000), unpack(128'h20000000000000000000000000000001), unpack(128'h10202122232425262728292a2b2c2d2e)};
	Vector#(16, Bit#(8)) plaintexts[4] =  {unpack(128'h00000000000000000000000000000000), unpack(128'h000102030405060708090a0b0c0d0e0f), unpack(128'h101112131415161718191a1b1c1d1e1f), unpack(128'h5ed4c5ef80ce9a1195b90eb8625181ad)};
	Vector#(16, Bit#(8)) expectation[4] = {	unpack(128'h7c1dfe9d95095397de0c2f7177bd972b),
						unpack(128'h8a47c59ccc4cfc2d64d666808d81c0c3),
						unpack(128'h27c694fe744245b8754ca8006244149d),
						unpack(128'hd7aea37b4db9bd47f1715a56e58a7054)};

	/*
		Configuration of the test
	*/
	String testName = "DeoxysBcEncrypt Test";
	int length = 4;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(16, Bit#(8)) result = encryption.getResult(keys[testIndex], tweaks[testIndex], plaintexts[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Values: %h, %h, %h", testName, keys[testIndex], tweaks[testIndex], plaintexts[testIndex]);
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