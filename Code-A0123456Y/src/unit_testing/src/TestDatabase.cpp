
#include "Database.h"

#include "catch.hpp"
using namespace std;

// Each cpp is a set of test cases for a specific component.
namespace TestDatabase {
    void require(bool b) {
        REQUIRE(b);
    }

    // The name of a test case should be unique and meaningful.
    TEST_CASE("CheckDatabaseProcedure") {
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

        // compare the test output with expected output
        require(testOutput == expectedOutput);

        // The test output should match with the expected output
        // and hence the assertion would be true.
    }
}



