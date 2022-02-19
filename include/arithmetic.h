// ���������� ������� � ������� �� ��������� �������������� ���������
#pragma once
#include <string>
#include <vector>
#include <map>
#include "polinomial.h"
using namespace std;

class Expression {
	// ��������� �������� (����������������� ����� ������������� ��������, ������������������ - �������������)
	map<string, char> prior{{"+", -2}, {"-", -2}, {"*", -1}, {"-u", 1}};


	// ���� �������: 1 - �����, 2 - ��������, 3 - ����������� ������, 4 - ����������� ������
	enum token_type {NUM = 1, OP, OPEN_BR, CLOSE_BR};

	struct Token {
		token_type type;		// ����� ���� �� ����������� ���� ��������
		Polinomial pol;	// �������� ����������, ���� ����� ������ ���������
		string name;	// ������ ��������, ���� �������� ��������, ��� �������� ����������, ���� �������� �����

		explicit Token(token_type type, const string& name = "", const Polinomial &pol = Polinomial()): type(type), pol(pol), name(name) {}

		bool operator==(const Token &token) {
			return (type == token.type && name == token.name);
		}
		bool operator!=(const Token &token) {
			return !(*this == token);
		}
	};

	vector<Token> parse(const string &expr);	// ��������� -> ������
	string check(map<string, Polinomial> &names);								// �������� ��������� �� ������������
	void make_postfix_notation();				// ��������� ������� -> ����������� ������

	// DATA
	vector<Token> _tokens;							// ����� ������ � ����������� ������
	map<string, vector<int>> variable_positions;	// ���� �� �������� ���������� � �������� ������ � ����������� ������, � ������� ��� ���������

public:

	Expression(const string &expr, map<string, Polinomial> &names);
	Polinomial compute(map<string, Polinomial> values);	// ���������� ��������� � ����������� ������
	bool has_no_variables();							// �������� �� ������� ���������� � ���������
};