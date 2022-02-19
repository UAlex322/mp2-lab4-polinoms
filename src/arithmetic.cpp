// ���������� ������� � ������� ��� ���������� �������������� ���������

#include <map>
#include <vector>
#include <stdexcept>
#include <stack>
#include <iostream>
#include "../include/arithmetic.h"
using namespace std;

Expression::Expression(const string &expr, map<string, Polinomial> &names) {
	_tokens = parse(expr);

	string verdict = check(names);
	if (_tokens.empty() || verdict != "") {
		verdict = "Error: expression is incorrect!\n" + verdict;
		if (_tokens.empty())
			verdict += "Error: empty expression\n";

		throw verdict;
	}

	make_postfix_notation();
}

vector<Expression::Token> Expression::parse(const string &expr) {
	vector<Token> tokens;
	size_t first_digit_pos = -1;	// ������� ������ ����� �������� ������������ �����; -1, ���� � ������ ������ �� ����������� �����
	size_t first_letter_pos = -1;	// ������� ������ ����� ������� ����������� ����������; -1, ���� � ������ ������ �� ����������� ����������
	bool was_dot = false;			// ���� �� ������� ����� ��������������� �����

	for (size_t i = 0; i < expr.size(); ++i) {
		if (expr[i] == ' ' && first_digit_pos == -1 && first_letter_pos == -1) continue;

		else if ('a' <= expr[i] && expr[i] <= 'z' || 'A' <= expr[i] && expr[i] <= 'Z') {
			if (first_letter_pos == -1)
				first_letter_pos = i;
		}

		else if ('0' <= expr[i] && expr[i] <= '9') {
			if (first_digit_pos == -1)
				first_digit_pos = i;
		}

		else if (first_digit_pos != -1 && expr[i] == '.' && !was_dot)
			was_dot = true;

		else {
			was_dot = false;

			if (first_digit_pos != -1) {
				tokens.push_back(Token(NUM, "", Polinomial(expr.substr(first_digit_pos, i-first_digit_pos))));
				first_digit_pos = -1;
			}

			if (first_letter_pos != -1) {
				tokens.push_back(Token(NUM, expr.substr(first_letter_pos, i-first_letter_pos)));
				first_letter_pos = -1;
			}

			if (expr[i] == '(')
				tokens.push_back(Token(OPEN_BR));

			else if (expr[i] == ')')
				tokens.push_back(Token(CLOSE_BR));

			else if (expr[i] == '-' && (tokens.empty() || tokens.back().type == OPEN_BR || tokens.back().type == OP))
				tokens.push_back(Token(OP, "-u"));

			else if (expr[i] != ' ')
				tokens.push_back(Token(OP, string(1, expr[i])));
		}
	}

	if (first_digit_pos != -1) {
		tokens.push_back(Token(NUM, "", Polinomial(expr.substr(first_digit_pos, expr.size()-first_digit_pos))));
	}

	if (first_letter_pos != -1) {
		tokens.push_back(Token(NUM, expr.substr(first_letter_pos, expr.size()-first_letter_pos)));
	}

	return tokens;
}

string Expression::check(map<string, Polinomial> &names) {
	token_type last_state = OPEN_BR; // ��� ���������� ���������� ������
	int correct_brackets = 0;
	string errors;

	for (const Token &token : _tokens) {
		switch (token.type) {
			case NUM:
				if (last_state == NUM || last_state == CLOSE_BR)
					return "Error: expression is incorrect";
				if (token.name != "" && names.find(token.name) == names.end())
					errors += ("'" + token.name + "' : polinomial doesn't exist");
				break;

			case OP:
				if (prior.find(token.name) == prior.end() || last_state == OP && prior[token.name] < 0 || last_state == OPEN_BR && prior[token.name] < 0)
					return "Error: expression is incorrect";
				break;

			case OPEN_BR:
				if (last_state == NUM || last_state == CLOSE_BR)
					return "Error: expression is incorrect";

				++correct_brackets;

				break;

			case CLOSE_BR:
				if (last_state == OP || last_state == OPEN_BR || correct_brackets < 1)
					return "Error: expression is incorrect";

				--correct_brackets;

				break;
		}

		last_state = token.type;
	}

	if (!_tokens.empty() && _tokens.back().type == OP && prior.find(_tokens.back().name) != prior.end() && prior[_tokens.back().name] > 0)
		return "Error: expression is incorrect";

	return (correct_brackets == 0) ? errors: "Error: expression is incorrect";

}

void Expression::make_postfix_notation() {
	vector<Token> postfix;
	stack<Token> ops;

	for (const Token &token: _tokens) {
		string name = token.name;
		switch (token.type) {
			case NUM:
				if (token.name != "")
					variable_positions[name].push_back(postfix.size());
				postfix.push_back(token);

				break;

			case OP:
				if (!ops.empty() && (prior[name] < 0 && prior[ops.top().name] >= prior[name] || prior[name] > 0 && prior[ops.top().name] > prior[name]))
					while (!ops.empty() && ops.top().type != OPEN_BR && (prior[name] < 0 && prior[ops.top().name] >= prior[name] || prior[name] > 0 && prior[ops.top().name] > prior[name])) {
						postfix.push_back(ops.top());
						ops.pop();
					}
				ops.push(token);

				break;

			case OPEN_BR:
				ops.push(token);

				break;

			case CLOSE_BR:
				while (!ops.empty() && ops.top().type != OPEN_BR) {
					postfix.push_back(ops.top());
					ops.pop();
				}
				ops.pop();

				break;
		}
	}

	while (!ops.empty()) {
		postfix.push_back(ops.top());
		ops.pop();
	}

	_tokens = postfix;
}

Polinomial Expression::compute(map<string, Polinomial> values) {
	vector<Token> copy_tokens = _tokens; // ����� ������ ������ ��� ����������, ����� �� ������� �������� ������
	stack<Polinomial> operands;
	Polinomial result, op1, op2;

	if (!variable_positions.empty()) {
		for (auto &var : variable_positions) {

			for (auto &pos : variable_positions[var.first])
				copy_tokens[pos].pol = values[var.first];
		}
	}

	for (Token &token: copy_tokens) {
		switch (token.type) {
			case NUM:
				operands.push(token.pol);
				break;

			case OP:
				string name = token.name;

				op1 = operands.top();
				operands.pop();

				if (name == "-u")
					result = -op1;

				else {
					op2 = operands.top();
					operands.pop();

					if (name == "+")
						result = op2 + op1;
					else if (name == "-")
						result = op2 - op1;
					else if (name == "*")
						result = op1 * op2;
				}

				operands.push(result);
				break;
		}
	}

	return operands.top();
}

bool Expression::has_no_variables() {
	return variable_positions.empty();
}