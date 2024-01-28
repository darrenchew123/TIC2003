#include "stdafx.h"
#include "../source/Database.h"
#include <iostream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DatabaseTests
{
	// Each cpp is a set of test cases for a specific component.
	TEST_CLASS(TestDatabase)
	{
	public:
		// Each test method is a separate test case. The name should be unique and meaningful.
		TEST_METHOD(CheckDatabaseProcedure)
		{
			// initialize the database and insert a procedure
			Database::initialize();
			Database::insertProcedure("echo1");
			Database::insertProcedure("echo2");
			
			// retrieve the procedures from the database
			vector<string> dbResults;
			Database::getProcedures(dbResults);
		
			// create the test output string from the procedures retrieved
			string testOutput;
			for (unsigned int i = 0; i < dbResults.size(); i++)
			{
				testOutput.append(dbResults.at(i) + "$");
			}

			// create the expected output string
			string expectedOutput = "echo1$echo2$";

			// Logger messages can be viewed in the Test Explorer 
			// under "open additional output for this result" for each test case
			Logger::WriteMessage("Test Output: ");
			Logger::WriteMessage(testOutput.c_str());
			Logger::WriteMessage("Expected Output: ");
			Logger::WriteMessage(expectedOutput.c_str());

			// compare the test output with expected output
			Assert::IsTrue(testOutput == expectedOutput);

			// The test output should match with the expected output 
			// and hence the assertion would be true.
		}
	
	// Some private helper functions can be added below.
	private:
	};
}