package TestTagGeneration;

import Vector::*;

import TestIfc::*;

import TagGeneration::*;

(* synthesize *)
module mkTestTagGeneration(TestIfc);


	FinalizeTagIfc ft <- mkFinalizeTag;

	Vector#(32, Bit#(8)) key  [8] = {unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f), unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					 unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f), unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					 unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f), unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f),
					 unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f), unpack(256'h101112131415161718191a1b1c1d1e1f202122232425262728292a2b2c2d2e2f)};
	Vector#(15, Bit#(8)) nonce[8] = {unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  ),
					 unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  ), unpack(120'h202122232425262728292a2b2c2d2e  )};
	Vector#(16, Bit#(8)) tag  [8] = {unpack(128'h00000000000000000000000000000000), unpack(128'h5ed4c5ef80ce9a1195b90eb8625181ad), unpack(128'h0bf0d87a09bbffff0882d6875cdee5fe), unpack(128'h9d78defd49bc0ea620e2945ef3dfc02a),
					 unpack(128'hd70ee31947d161c9a568ccd8135225a3), unpack(128'h5eb85f70c9dad8c20d1ed8926f1a87a0), unpack(128'hc7d3b96de7fb4b29684a207e0dd5c434), unpack(128'h5804e59f72f38f417bdab9d8a2f49aa6)};
	Vector#(16, Bit#(8)) expectation[8] = {unpack(128'h7c1dfe9d95095397de0c2f7177bd972b), unpack(128'hd7aea37b4db9bd47f1715a56e58a7054), unpack(128'hf1baa13454d159ffc18c20c49d687732), unpack(128'hfa9234a9c271ab9eff724b3aec3ace92),
					       unpack(128'hcc5edf39ee722025d0198f30578da75f), unpack(128'h900da68424bb1d450abaac10bff94965), unpack(128'he980bf0830b3240b19783c93e3103f0b), unpack(128'h62e215c1f8a3f26c528be118c2f016a6)};

	/*
		Configuration of the test
	*/
	String testName = "TagGeneration Test";
	int length = 8;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTest(int testIndex);

		Vector#(16, Bit#(8)) result = ft.finalizeTag(key[testIndex], nonce[testIndex], tag[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Action displayTestData(String testName, int testIndex);
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h, %h, %h", testName, key[testIndex], nonce[testIndex], tag[testIndex]);
				$display("[%s]Test Result: %h", testName, ft.finalizeTag(key[testIndex], nonce[testIndex], tag[testIndex]));
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