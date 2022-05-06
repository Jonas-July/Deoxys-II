package TestTagGeneration;

import Vector::*;

import TestIfc::*;

import TagGeneration::*;

(* synthesize *)
module mkTestTagGeneration(TestIfc);


	FinalizeTagIfc ft <- mkFinalizeTag;
	ComputePartialTagMsgIfc msgGen <- mkComputePartialTagMsg;
	ComputePartialTagAddIfc addGen <- mkComputePartialTagAdd;

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

	Vector#(16, Bit#(8)) msg[8] = {unpack(128'h000102030405060708090a0b0c0d0e0f), unpack(128'h101112131415161718191a1b1c1d1e1f), unpack(128'hfec3653211fa75306a83486e5c34ecfe), unpack(128'h63870c97251a73e4b9033ae374809711),
				       unpack(128'hb211ed5d293a592e466a81170f1d8575), unpack(128'h0b5ca025ccd4579947edbae9ec132bfb), unpack(128'h1a7233ad79fae30006a6699f14389386), unpack(128'h1b975226ed9d3cfb8a240be232fbf4e8)};
	Vector#(16, Bit#(8)) expectMsg[8] = { unpack(128'ha0122562c6af9b0a3710abee3218fbc6), unpack(128'h3d6afb9f8f1395ac17f23fb0c1c73bec), unpack(128'h223bde8c5a11aeb2ee33ba35c16aa4a5), unpack(128'h9cdfef5f21a86a3c3230e667516adf3d),
					      unpack(128'h145cc08fef4cd96a9b866e1e6ec884e6), unpack(128'h4e8762042ef5d11b24fea98e4b9a7bd8), unpack(128'hbea44d5c8db2377644ac3b18f0e916da), unpack(128'hb4227dfafd58fb78ae0d05e627bb56df)};

	Vector#(16, Bit#(8)) add[8] = {unpack(128'h000102030405060708090a0b0c0d0e0f), unpack(128'h101112131415161718191a1b1c1d1e1f), unpack(128'h8f3b5ab1affa65e6c0e3680eb33a6ec8), unpack(128'h2424ab1ce5a40b8654e13d845c29b138),
				       unpack(128'h96a1466a75fc875acba4527ded37ed00), unpack(128'hc600a357c9a6e586c74cf3d85cd3258c), unpack(128'h813218f319d12b82480e5124ff19ec00), unpack(128'hbda1fbb8bd25eeb3de9fcbf3296deba2)};
	Vector#(16, Bit#(8)) expectAdd[8] = { unpack(128'hc3c0818d8066d6642dfc4ccc9cc5478a), unpack(128'h9d14446200a84c75b8454274fe94c627), unpack(128'h0ae262c5ef45fa39b52de3a714451a10), unpack(128'hc3f53b8d57e6990ff617010b8b0c0c1c),
					      unpack(128'h25179e914b1d91e4965b784da4be4fa6), unpack(128'h4eb38086f66d8e3e919d7b5990f7a6bc), unpack(128'head42a275cb6533b69c22881208521a7), unpack(128'h11f649500fce8f535a92d5e7f91ea115)};



	/*
		Configuration of the test
	*/
	String testName = "TagGeneration Test";
	int length = 24;
	Bool finish = False;
	Bool displayTests = False;

	function Bool runTestFinalize(int testIndex);
		Vector#(16, Bit#(8)) result = ft.finalizeTag(key[testIndex], nonce[testIndex], tag[testIndex]);
		Vector#(16, Bit#(8)) expected = expectation[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Bool runTestTagMsg(int testIndex);
		UInt#(32) index = testIndex >= 0 ? unpack(pack(testIndex))
						 : 0;
		Vector#(16, Bit#(8)) result = msgGen.routine.calcPartialTag(key[testIndex], zeroExtend(index), msg[testIndex]);
		Vector#(16, Bit#(8)) expected = expectMsg[testIndex];
		return (pack(result) == pack(expected));
	endfunction

	function Bool runTestTagAdd(int testIndex);
		UInt#(32) index = testIndex >= 0 ? unpack(pack(testIndex))
						 : 0;
		Vector#(16, Bit#(8)) result = addGen.routine.calcPartialTag(key[testIndex], zeroExtend(index), add[testIndex]);
		Vector#(16, Bit#(8)) expected = expectAdd[testIndex];
		return (pack(result) == pack(expected));
	endfunction


	function Bool runTest(int testIndex);
		return    testIndex <  8 ? runTestFinalize(testIndex       % 8)
			: testIndex < 16 ? runTestTagMsg( (testIndex -  8) % 8)
					 : runTestTagAdd( (testIndex - 16) % 8);
	endfunction

	function Action displayTestData(String testName, int testIndex);
		UInt#(32) index = testIndex >= 0 ? unpack(pack(testIndex))
						 : 0;
		return
			action
				$display("[%s]Test Number: ", testName, testIndex);
				$display("[%s]Test Value : %h, %h, %h", testName, key[testIndex], index, msg[testIndex]);
				$display("[%s]Test Result: %h", testName, msgGen.routine.calcPartialTag(key[testIndex], zeroExtend(index), msg[testIndex]));
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