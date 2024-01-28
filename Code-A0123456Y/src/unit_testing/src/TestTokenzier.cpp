
#include "Tokenizer.h"

#include "catch.hpp"
using namespace std;

// Each cpp is a set of test cases for a specific component.
namespace TestTokenizer {
    void require(bool b) {
        REQUIRE(b);
    }

    // The name of a test case should be unique and meaningful.
    TEST_CASE("CheckTokenizeQuery") {
        // create the input string
        string testInput = "procedure p;Select p";

        // run the tokenizer method
        Tokenizer tk;
        vector<string> tokens;
        tk.tokenize(testInput, tokens);

        // create the test output string from the tokens
        string testOutput;

        for (unsigned int i = 0; i < tokens.size(); i++) {
            testOutput.append(tokens.at(i) + "$");
        }

        // create the expected output string
        string expectedOutput = "procedure$p$;$Select$p$";

        // compare the testOutput with expected output
        require(testOutput == expectedOutput);

        // The test output should match with the expected output
        // and hence the assertion would be true.
    }

    TEST_CASE("CheckTokenizeProgram") {
        string testInput = "procedure echo { read num1; index = 1001; print index; print num1; }";

        Tokenizer tk;
        vector<string> tokens;
        tk.tokenize(testInput, tokens);

        string testOutput;

        for (unsigned int i = 0; i < tokens.size(); i++) {
            testOutput.append(tokens.at(i) + "$");
        }

        string expectedOutput = "procedure$echo${$read$num1$;$index$=$1001$;$print$index$;$print$num1$;$}$";

        require(testOutput == expectedOutput);
    }
}
