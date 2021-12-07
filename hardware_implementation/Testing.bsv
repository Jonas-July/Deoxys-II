
package Testing;

import InversionAndMultiplication::*;

(* synthesize *)
module mkTestInversionAndMultiplication(Empty);

	InversionAndMultiplicationIfc iam <- mkInversionAndMultiplication;
	Reg#(Bit#(8)) testNr <- mkReg(0);

	Reg#(Bool) testSuccessful <- mkReg(True);
				       //0    1    2    3    4    5    6    7    8    9    A    B    C    D    E    F
	Bit#(8) expectation[256] = {      0,  31,  20,  24, 145,   8,  12, 166,  83,  98,   4,  72, 157, 180, 200,  21, 
					169, 225, 170,  30, 153,  58,  36, 147, 206, 183, 193, 204, 255, 199,  17, 163, 
					212, 158, 240,  69,  85,  92, 148, 175,  87, 198, 134, 146,  18, 187,  82, 118,
					103, 164,  64, 160, 123, 245, 102, 249, 100, 113, 227, 129, 136,  68, 209,  22, 
					106, 224,  79, 121, 120,  13,  57, 195,  49,  88, 181, 208,  74, 128,  76, 231, 
					 48, 178,  99, 142,  67, 159, 210,  56,   9, 168, 221,  90,  41,  47,  59, 172, 
					179, 140, 201, 152,  32,  46,  80, 230,  38, 154,  97,  28,  51,  95, 252, 203, 
					 50, 192,  35, 236, 241, 254,  91, 150, 223, 213, 185,  66, 115, 156, 144, 177, 
					174, 111, 112, 143,  60, 244,  39, 116, 167, 196,  29,  94,   7,  62, 122,  16, 
					  3, 226,  44, 191,  65,  73, 243, 235,  37, 141, 219, 119, 189,  61, 104, 184, 
					131,  81,  89, 105,  42, 101,  71,  63, 161, 176, 207,   1, 242, 246, 135,  26, 
					132, 171,  84,  14, 238, 182,  45, 202,  15,  53, 151, 137,   6,  25, 205, 107, 
					217,  27,  70,  77, 127, 197, 215, 165, 139, 190,  23, 124,  40, 222, 232, 233, 
					 19,  93, 214,   5,  43,  96, 149, 109,   2,  86,  52, 218, 229, 162, 126, 253, 
					130, 155, 251, 114,  10, 186, 237, 247, 248, 125, 228, 138, 173,  54,  75, 188,
					239, 194, 234, 110, 220, 133,  33,  11,  34, 250,  78, 108, 211,  55, 216, 117 };


	rule test if (testNr < 255);
		let result <- iam.invertAndMultiply(testNr);
		let expected = expectation[testNr];
		$display("Test %0d: %0d should be %0d", testNr, result, expected);
		testNr <= testNr + 1;
		testSuccessful <= testSuccessful && (result == expected);
	endrule

	rule fin if (testNr == 255);
		let result <- iam.invertAndMultiply(testNr);
		let expected = expectation[testNr];
		$display("Test %0d: %0d should be %0d", testNr, result, expected);
		testSuccessful <= testSuccessful && (result == expected);
		$display("Test succeeded: %s", testSuccessful ? "Success!" : "Failed!");
		$finish();
	endrule

endmodule

endpackage