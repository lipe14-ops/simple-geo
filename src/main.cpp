#include "parsing/expressions.h"
#include <iostream>

using namespace std;
using namespace expr;

int main() {
    cout << "=== Expression Parser - Refactored ===" << endl << endl;

    try {
        // Test 1: Parentheses
        cout << "--- Test 1: With Parentheses ---" << endl;
        cout << "Expression: (2+3)*4" << endl << "Tree: ";
        auto expr1 = parse<float>("(2+3)*4");
        expr1->print();
        cout << endl << "Result: " << expr1->eval(0) << " (expected: 20)" << endl << endl;

        // Test 2: Variable
        cout << "--- Test 2: With Variable ---" << endl;
        cout << "Expression: 2*x+3, with x=5" << endl << "Tree: ";
        auto expr2 = parse<float>("2*x+3");
        expr2->print();
        cout << endl << "Result: " << expr2->eval(5) << " (expected: 13)" << endl << endl;

        // Test 3: Parentheses with variable
        cout << "--- Test 3: Parentheses with Variable ---" << endl;
        cout << "Expression: (x+2)*3, with x=4" << endl << "Tree: ";
        auto expr3 = parse<float>("(x+2)*3");
        expr3->print();
        cout << endl << "Result: " << expr3->eval(4) << " (expected: 18)" << endl << endl;

        // Test 4: Multiple parentheses
        cout << "--- Test 4: Multiple Parentheses ---" << endl;
        cout << "Expression: (2+3)*(4+1)" << endl << "Tree: ";
        auto expr4 = parse<float>("(2+3)*(4+1)");
        expr4->print();
        cout << endl << "Result: " << expr4->eval(0) << " (expected: 25)" << endl << endl;

        // Test 5: Nested expressions
        cout << "--- Test 5: Nested Expressions ---" << endl;
        cout << "Expression: ((x+1)*2)^2, with x=1.5" << endl << "Tree: ";
        auto expr5 = parse<float>("((x+1)*2)^2");
        expr5->print();
        cout << endl << "Result: " << expr5->eval(1.5) << " (expected: 36)" << endl << endl;

        // Test 6: Complex expression
        cout << "--- Test 6: Complex Expression ---" << endl;
        cout << "Expression: x^2 + 2*x + 1, with x=3" << endl << "Tree: ";
        auto expr6 = parse<float>("x^2+2*x+1");
        expr6->print();
        cout << endl << "Result: " << expr6->eval(3) << " (expected: 16)" << endl;

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}