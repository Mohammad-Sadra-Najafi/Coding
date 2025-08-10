#include <iostream>
#include <stack>
#include <sstream>
#include <cmath>
#include <map>
#include <cctype>

int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

double applyOp(double a, double b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::runtime_error("Division by zero!");
            return a / b;
        case '^': return std::pow(a, b);
    }
    throw std::runtime_error("Unknown operator");
}

double evaluate(const std::string &tokens) {
    std::stack<double> values;
    std::stack<char> ops;

    for (size_t i = 0; i < tokens.length(); i++) {
        if (isspace(tokens[i])) continue;

        // Number (possibly multi-digit)
        if (isdigit(tokens[i]) || tokens[i] == '.') {
            std::string num;
            while (i < tokens.length() && (isdigit(tokens[i]) || tokens[i] == '.')) {
                num += tokens[i++];
            }
            values.push(std::stod(num));
            i--;
        }
        // Open parenthesis
        else if (tokens[i] == '(') {
            ops.push(tokens[i]);
        }
        // Close parenthesis
        else if (tokens[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            if (!ops.empty()) ops.pop();
        }
        // Operator
        else {
            while (!ops.empty() && precedence(ops.top()) >= precedence(tokens[i])) {
                double b = values.top(); values.pop();
                double a = values.top(); values.pop();
                char op = ops.top(); ops.pop();
                values.push(applyOp(a, b, op));
            }
            ops.push(tokens[i]);
        }
    }

    while (!ops.empty()) {
        double b = values.top(); values.pop();
        double a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOp(a, b, op));
    }

    return values.top();
}

int main() {
    std::cout << "Advanced C++ Calculator\n";
    std::cout << "Supports +, -, *, /, ^ and parentheses ()\n";

    std::string expression;
    while (true) {
        std::cout << "\nEnter expression (or 'exit'): ";
        std::getline(std::cin, expression);
        if (expression == "exit") break;

        try {
            double result = evaluate(expression);
            std::cout << "Result: " << result << "\n";
        } catch (const std::exception &e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }
}