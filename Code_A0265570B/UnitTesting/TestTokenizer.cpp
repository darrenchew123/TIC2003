#include "stdafx.h"
#include "../source/Tokenizer.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace TokenizerTests
{
	// Each cpp is a set of test cases for a specific component.
	TEST_CLASS(TestTokenizer)
	{
	public:
		// Each test method is a separate test case. The name should be unique and meaningful.
		TEST_METHOD(CheckTokenizeQuery) 
		{
			// create the input string
			string testInput = "procedure p;Select p";

			// run the tokenizer method
			Tokenizer tk;
			vector<string> tokens;
			tk.tokenize(testInput, tokens);

			// create the test output string from the tokens
			string testOutput; 

			for (unsigned int i = 0; i < tokens.size(); i++)
			{
				testOutput.append(tokens.at(i) + "$");
			}

			// create the expected output string
			string expectedOutput = "procedure$p$;$Select$p$"; 

			// Logger messages can be viewed in the Test Explorer 
			// under "open additional output for this result" for each test case
			Logger::WriteMessage("Test Output: "); 
			Logger::WriteMessage(testOutput.c_str());
			Logger::WriteMessage("Expected Output: ");
			Logger::WriteMessage(expectedOutput.c_str());

			// compare the testOutput with expected output
			Assert::IsTrue(testOutput == expectedOutput);
 
			// The test output should match with the expected output 
			// and hence the assertion would be true.
		}

		TEST_METHOD(CheckTokenizeProgram)
		{
			string testInput = "procedure echo { read num1; index = 1001; print index; print num1; }";

			Tokenizer tk;
			vector<string> tokens;
			tk.tokenize(testInput, tokens);

			string testOutput;

			for (unsigned int i = 0; i < tokens.size(); i++)
			{
				testOutput.append(tokens.at(i) + "$");
			}

			string expectedOutput = "procedure$echo${$read$num1$;$index$=$1001$;$print$index$;$print$num1$;$}$";

			Logger::WriteMessage("Test Output: ");
			Logger::WriteMessage(testOutput.c_str());
			Logger::WriteMessage("Expected Output: ");
			Logger::WriteMessage(expectedOutput.c_str());

			Assert::IsTrue(testOutput == expectedOutput);
		}

	// Some private helper functions can be added below.
	private:
	};
}