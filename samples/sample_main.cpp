#include "../include/arithmetic.h"
#include <map>
using namespace std;

// Проверка на пробелы в строке
bool is_empty_string(string str) {
	str.erase(remove_if(str.begin(), str.end(), [](char ch){return (ch == ' ' || ch == '\t');}), str.end());
	return str == "";
}

// Проверка корректности названия - оно должно содержать только буквы и символ '_' (возвращает True, если имя подходит)
bool check_name(const string &name) {
	return find_if(name.begin(), name.end(), [](char ch){return (ch < 'a' || ch > 'z') && (ch > 'Z' || ch < 'A') && (ch != '_');}) == name.end();
}

void start_message() {
	cout << "POLINOMIAL CALCULATOR\n";
	cout << "This is a polinomial calculator.\nYou can create polinomial, perform addition, subtraction and multiplication, and delete them.\n\n";
	cout << "Available commands:\n\n";
	cout << "1) create 'name' = 'value' - creates a polinomial with name 'name' and value 'value'. You can use only created polinomial in operations.\n";
	cout <<	"   'name' may have only english letters and symbol '_'. The rules for 'value' are written below in command 2. Example: create f = 5*x\n";
	cout << "   OR create 'name' - creates a polinomial with name 'name' and default vakue 0. Example: create f\n\n";
	cout << "2) set 'name' = 'value' - assigns polinomial 'value' to existing polinomial 'name'. If polinomial 'name' doesn't exist, the error message will be displayed.\n";
	cout << "   Rules of 'value':\n";
	cout << "   a) Polinomial is a sequence of monomials separated by signs '+' or '-'. Sign before the first monomial may be omitted.\n";
	cout << "      Monomials may be separated by spaces. Examples: +3x^2, 1 + x + x^2, 1 - xyz + x^2y^2z^2, x+y+z, -x+1.\n";
	cout << "   b) Monomial has a coefficient and variable-part after. Coefficient is a number, variable part has variables or powers of variables.\n";
	cout << "      Only variables 'x', 'y', 'z' are allowed. Power is an integer between 0 and 9.\n";
	cout << "      If 'power' >= 10, the error message will be displayed. You may separate the parts by spaces and sign '*' (see examples).\n";
	cout << "      Examples: 3x^2, 5.0*xyz, 8 x ^ 2 y z, 13x*y*z*x, y, -x (missing coefficient or '-' sign before monomial are interpreted as 1 or -1) \n";
	cout << "   c) Expression must be an explicit polinomial i.e. it mustn't contain any letters expect available variables (for example, set f = g*h is forbidden).\n";
	cout << "   Example: set f = 1 - x + x^2 - x^3 + x^4\n\n";
	cout << "3) calculate 'poli_name' = 'expression' - calculates 'expression' and assigns the result to existing polinomial 'poli_name'.\n";
	cout << "   'expression' contains numbers, names of polinomials and operations with them ('+', '-', '*'). 'expression' must be a correct arithmetic expression.\n";
	cout << "   Example: calculate f = 3*g + h*u*w + 8 + 5.0*21.2\n\n";
	cout << "4) print 'poli_name' - prints a value of polinomial 'poli_name' on the screen. Example: print plnml\n\n";
	cout << "5) print_all - prints all existing polinomials and their values.\n\n";
	cout << "6) delete 'poli_name' - deletes a polinomial 'poli_name'. This means you can't do operations with the polinomial with the very name until you create it again.\n";
	cout << "   Example: delete f\n\n";
	cout << "7) exit - exits the application.\n\n";

	cout << "Enjoy!\n";
}

void menu() {
	start_message();

	map<string, Polinomial> polinomial;
	string command, name, value, dummy, expression;

	while (true) {
		cout << ">>> ";
		cin >> command;

		try {

			if (command == "create") {
				cin >> name;
				// Проверка на корректность имени
				if (check_name(name) == false) 
					cout << "'" << name << "': invalid name" << endl << endl;
				// Проверка на существование многочлена с таким же именем
				else if (polinomial.find(name) != polinomial.end()) 
					cout << "'" << name << "': polinomial already exists" << endl << endl;

				else {
					getline(cin, value);
					// Если нет знака и символов кроме пробелов, создаётся пустой многочлен
					if (value.find_first_of('=') == string::npos && is_empty_string(value))
						polinomial[name] = Polinomial();
					// Если есть знак '=', создаём многочлен из строки, идущей за ним
					else if (value.find_first_of('=') != string::npos) {
						value.erase(find(value.begin(), value.end(), '='));
						try {
							polinomial[name] = Polinomial(value);
						}
						catch (const string &exc) {
							throw string("'" + name + "': " + exc);
						}
					}

					else
						throw string("Incorrect command - missing '=' with non-empty value of polinomial");

					cout << "'" << name << "': polinomial has been created successfully" << endl << endl;
				}
			}

			else if (command == "set") {
				cin >> name >> dummy; // лишнее считывание - для знака '='
				getline(cin, value);

				if (polinomial.find(name) == polinomial.end())
					cout << "'" << name << "': polinomial doesn't exist" << endl << endl;
				// Если отсутствует знак '=', команда неверная
				else if (dummy != "=") {
					cout << "Invalid command" << endl << endl;
				}

				else {
					// Многочлен при работе конструктора не знает своего имени, поэтому в исключении для удобства его нужно добавить
					try {
						polinomial[name] = Polinomial(value);
					}
					catch (const string &exc) {
						throw string("'" + name + "': " + exc);
					}

					cout << "'" + name + "': value has been set successfully" << endl << endl;
				}
			}

			else if (command == "calculate") {
				cin >> name >> dummy;
				getline(cin, expression);

				if (polinomial.find(name) == polinomial.end())
					cout << "'" << name << "': polinomial doesn't exist" << endl << endl;

				else if (dummy != "=") {
					cout << "Invalid command" << endl << endl;
				}

				else {
					try {
						polinomial[name] = Expression(expression, polinomial).compute(polinomial);
					}
					catch (const string &exc) {
						throw string("'" + name + "': " + exc);
					}

					cout << name << " = " << polinomial[name] << endl << endl;
				}
			}

			else if (command == "print") {
				cin >> name;
				getline(cin, dummy);

				if (polinomial.find(name) == polinomial.end())
					cout << "'" << name << "': polinomial doesn't exist" << endl << endl;

				else if (!is_empty_string(dummy))
					cout << "Invalid command" << endl << endl;
				else
					cout << name << " = " << polinomial[name] << endl << endl;
			}

			else if (command == "print_all") {
				if (polinomial.empty())
					cout << "No polinomials exist" << endl << endl;
				else {
					cout << "All polinomials:" << endl;

					for (auto &p : polinomial)
						cout << "'" << p.first << "' = " << p.second << endl;
					cout << endl;
				}
			}

			else if (command == "delete") {
				cin >> name;
				getline(cin, dummy);

				if (!is_empty_string(dummy))
					cout << "Invalid command" << endl << endl;

				else if (polinomial.find(name) == polinomial.end())
					cout << "'" << name << "': polinomial doesn't exist" << endl << endl;

				else {
					polinomial.erase(name);
					cout << "'" << name << "': polinomial has been deleted successfully" << endl << endl;
				}
			}

			else if (command == "exit") {
				cout << "Exiting..." << endl << endl;
				exit(0);
			}

			else {
				// Дочитываем строку до конца, чтобы на одну команду не было несколько сообщений
				getline(cin, dummy);

				throw string("Invalid command");
			}
		}
		catch (const string &exc) {
			cout << exc << endl << endl;
		}
	}
}

int main() {
	menu();
}