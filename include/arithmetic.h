// объ¤вление функций и классов дл¤ вычислени¤ арифметических выражений
#pragma once
#include <string>
#include <vector>
#include <map>
#include "polinomial.h"
using namespace std;

class Expression {
	// Приоритет операций (левоассоциативные имеют отрицательное значение, правоассоциативные - положительное)
	map<string, char> prior{{"+", -2}, {"-", -2}, {"*", -1}, {"-u", 1}};


	// Типы токенов: 1 - число, 2 - операция, 3 - открывающая скобка, 4 - закрывающая скобка
	enum token_type {NUM = 1, OP, OPEN_BR, CLOSE_BR};

	struct Token {
		token_type type;		// имеет одно из определённых выше значений
		Polinomial pol;	// значение многочлена, если токен хранит многочлен
		string name;	// символ операции, если хранится операция, или название переменной, если хранится число

		explicit Token(token_type type, const string& name = "", const Polinomial &pol = Polinomial()): type(type), pol(pol), name(name) {}

		bool operator==(const Token &token) {
			return (type == token.type && name == token.name);
		}
		bool operator!=(const Token &token) {
			return !(*this == token);
		}
	};

	vector<Token> parse(const string &expr);	  // выражение -> токены
	string check(map<string, Polinomial> &names); // проверка выражения на корректность
	void make_postfix_notation();				  // инфиксная нотация -> постфиксная запись

	// DATA
	vector<Token> _tokens;							// набор лексем в постфиксной записи
	map<string, vector<int>> variable_positions;	// пары из названий переменных и индексов лексем в постфиксной записи, в которых они находятся

public:

	Expression(const string &expr, map<string, Polinomial> &names);
	Polinomial compute(map<string, Polinomial> values);	// вычисление выражения в постфиксной записи
	bool has_no_variables();							// проверка на наличие переменных в выражении
};