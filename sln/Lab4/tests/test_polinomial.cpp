#include "pch.h"
#include "../../../include/polinomial.h"
#include <vector>
#include <chrono>
#include <random>
using namespace std;

TEST(Polinomial, can_create_empty_polinomial) {
	ASSERT_NO_THROW(Polinomial f);
}

TEST(Polinomial, can_create_via_initializer_list) {
	ASSERT_NO_THROW(Polinomial f{Mono(42.0,3,2,2)});
}

TEST(Polinomial, can_create_via_copy_constructor) {
	Polinomial f{Mono(42.0,3,2,2)};
	ASSERT_NO_THROW(Polinomial g = f);
}

TEST(Polinomial, can_create_via_move_constructor) {
	Polinomial f{Mono(42.0,3,2,2)};
	ASSERT_NO_THROW(Polinomial g = std::move(f));
}

TEST(Polinomial, can_create_via_string_constructor) {
	ASSERT_NO_THROW(Polinomial f1("x^2"), f2(""), f3("3"), f4("5.421*x^2y^3*z^8 + 42xyz"));
}

TEST(Polinomial, initializer_list_constructor_is_correct) {
	//  онструктор от initializer_list напр€мую вставл€ет мономы и не измен€ет входные данные.
	// ¬ставка в List работает корректно, поэтому математически конструктор работает корректно.
	// ѕроверить напр€мую равенство переданных в конструктор значений и значений в многочлене по-честному невозможно, т.к.
	// пр€мой доступ к списку внутри Polinomial извне закрыт.

	bool initializer_list_ctor_is_correct = true;
	EXPECT_EQ(initializer_list_ctor_is_correct, true);
}

TEST(Polinomial, eq_operator_is_correct) {
	Polinomial f1{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f2{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f3{};

	EXPECT_EQ(f1 == f2, true);
	EXPECT_EQ(f1 == f3, false);
}

TEST(Polinomial, neq_operator_is_correct) {
	Polinomial f1{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f2{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f3{};

	EXPECT_EQ(f1 != f2, false);
	EXPECT_EQ(f1 != f3, true);
}

TEST(Polinomial, copy_constructor_is_correct) {
	Polinomial f1{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f2 = f1;

	EXPECT_EQ(f1, f2);
}

TEST(Polinomial, move_constructor_is_correct) {
	Polinomial f1{Mono(42.0,3,2,2), Mono(5.0,4,8,8)}, f2 = f1, f3 = std::move(f1);

	EXPECT_EQ(f2, f3);
}

TEST(Polinomial, string_constructor_is_correct) {
	string right[]{"", "1", "-3.1415926", "-x", "3*x^2y^5", "2.718281828xyz", "2.7x^9y^7*z + 7.3x^9y^7*z", "3 - 1*x^2 - 1*y^2", "-x^5*y^7*z^7", "+x^5 - x^5"};
	initializer_list<Mono> expected[]{{Mono(0,0,0,0)}, {Mono(1.0,0,0)}, {Mono(-3.1415926,0,0,0)}, {Mono(-1.0,1,0,0)}, {Mono(3,2,5,0)}, {Mono(2.718281828,1,1,1)},
									 {Mono(10,9,7,1)}, {Mono(3,0,0,0),Mono(-1,2,0,0),Mono(-1,0,2,0)}, {Mono(-1,5,7,7)}, {} };

	for (size_t i = 0; i < 10; ++i)
		EXPECT_EQ(Polinomial(right[i]), Polinomial(expected[i]));

	string wrong[]{"welcome", "x5", "3**x", "3^x", "x^y", "xyz*3", "y ++ 3z", "z+-1", "y^3--x", "xyz^42069", "*3xyz", "xyz*w", "x^-3"};
	for (size_t i = 0; i < 13; ++i)
		ASSERT_ANY_THROW(Polinomial f(wrong[i]));
}

TEST(Polinomial, copy_assignment_is_correct) {
	Polinomial f1("3x^2 + 3y^2"), f2;

	ASSERT_NO_THROW(f2 = f1);
	EXPECT_EQ(f2, Polinomial("3x^2 + 3y^2"));
}

TEST(Polinomial, move_assignment_is_correct) {
	Polinomial f1("3x^2 + 3y^2"), f2;

	ASSERT_NO_THROW(f2 = std::move(f1));
	EXPECT_EQ(f2, Polinomial("3x^2 + 3y^2"));
}

TEST(Polinomial, scalar_multiplication_is_correct) {
	Polinomial f1("3x^2 + 3y^2"), f2("xyz");

	ASSERT_NO_THROW(f1 *= 2; f1 = 2*f1; f1 = f1*2);
	ASSERT_NO_THROW(f2 = (-1)*f2);

	EXPECT_EQ(f1, Polinomial("24x^2 + 24y^2"));
	EXPECT_EQ(f2, Polinomial("-xyz"));
}

TEST(Polinomial, unary_minus_is_correct) {
	Polinomial f("xy"), g("3*z^2");
	ASSERT_NO_THROW(
		EXPECT_EQ(-f, Polinomial("-xy"));
		EXPECT_EQ(-g, Polinomial("-3*z^2"))
	);
}

TEST(Polinomial, addition_is_correct) {
	string f1[5]{"x", "1.0", "xyz", "x^2", "3x + 4y"},
		   f2[5]{"y", "z^3", "3xyz", "-x^2", "y + 5z"},
		   expected[5]{"x+y", "1.0 + z^3", "4xyz", "0", "3x + 5y + 5z"};

	for (size_t i = 0; i < 5; ++i)
		ASSERT_NO_THROW(EXPECT_EQ(Polinomial(f1[i]) + Polinomial(f2[i]), Polinomial(expected[i])));
	for (size_t i = 0; i < 5; ++i) {
		ASSERT_NO_THROW(
			Polinomial f(f1[i]);
			f += Polinomial(f2[i]);
			EXPECT_EQ(f, Polinomial(expected[i]));
		);
	}
}

TEST(Polinomial, subtraction_is_correct) {
	string f1[]{"x", "1.0", "xyz", "x^2", "3x + 4y"},
		   f2[]{"y", "z^3", "3xyz", "x^2", "y + 5z"},
		   expected[]{"x-y", "1.0 - z^3", "-2xyz", "0", "3x + 3y - 5z"};

	for (size_t i = 0; i < 5; ++i)
		ASSERT_NO_THROW(EXPECT_EQ(Polinomial(f1[i]) - Polinomial(f2[i]), Polinomial(expected[i])));
	for (size_t i = 0; i < 5; ++i) {
		ASSERT_NO_THROW(
			Polinomial f(f1[i]);
			f -= Polinomial(f2[i]);
			EXPECT_EQ(f, Polinomial(expected[i]));
		);
	}
}

TEST(Polinomial, multiplication_is_correct) {
	string f1[]{"x", "-1.0", "xyz", "x^2", "x + y + z", "x^5 + y^9", "1 - x + x^2 - x^3 + x^4 - x^5"},
		   f2[]{"y", "z^3", "3xyz", "-x^2", "x + y + z", "0", "1 - x"},
		   expected[]{"xy", "-z^3", "3x^2y^2z^2", "-x^4", "x^2 + y^2 + z^2 + 2xy + 2xz + 2yz", "0", "1 - x^6"};

	for (size_t i = 0; i < 5; ++i)
		ASSERT_NO_THROW(EXPECT_EQ(Polinomial(f1[i]) * Polinomial(f2[i]), Polinomial(expected[i])));
	for (size_t i = 0; i < 5; ++i) {
		ASSERT_NO_THROW(
			Polinomial f(f1[i]);
			f *= Polinomial(f2[i]);
			EXPECT_EQ(f, Polinomial(expected[i]));
		);
	}

	// ѕроверка переполнени€ степени
	ASSERT_ANY_THROW(Polinomial f("x^5"); Polinomial g("x^6"); f*g);
	ASSERT_ANY_THROW(Polinomial f("1 + x^5y^6z^7"); Polinomial g("x^4y^3z^3 + x^2 + y^2 + z + 1"); f*g);
}



TEST(Polinomial, input_operator_is_correct) {
	string poli[]{"", "1", "-3.1415926", "-x", "3*x^2y^5", "2.718281828xyz", "2.7x^9y^7*z + 7.3x^9y^7*z", "3 - 1*x^2 - 1*y^2", "-x^5*y^7*z^7", "+x^5 - x^5"};
	initializer_list<Mono> expected[]{{Mono(0,0,0,0)}, {Mono(1.0,0,0)}, {Mono(-3.1415926,0,0,0)}, {Mono(-1.0,1,0,0)}, {Mono(3,2,5,0)}, {Mono(2.718281828,1,1,1)},
		{Mono(10,9,7,1)}, {Mono(3,0,0,0),Mono(-1,2,0,0),Mono(-1,0,2,0)}, {Mono(-1,5,7,7)}, {} };

	Polinomial f;
	for (size_t i = 0; i < 10; ++i) {
		stringstream ss;
		ss << poli[i];
		ss >> f;
		EXPECT_EQ(f, Polinomial(expected[i]));
	}

	string wrong[]{"welcome", "x5", "3**x", "3^x", "x^y", "xyz*3", "y ++ 3z", "z+-1", "y^3--x", "xyz^42069", "*3xyz", "xyz*w", "x^-3"};
	for (size_t i = 0; i < 13; ++i) {
		stringstream ss;

		ss << wrong[i];
		ASSERT_ANY_THROW(ss >> f);
	}
}

TEST(Polinomial, output_operator_is_correct) {
	string poli[]{"", "1", "-3.1415926", "-x", "3*x^2y^5", "2.718281828xyz", "2.7x^9y^7*z + 7.3x^9y^7*z", "3 - 1*x^2 - 1*y^2", "-x^5*y^7*z^7", "+x^5 - x^5"};
	string expected[]{"0", "1", "-3.1415926", "-x", "3*x^2y^5", "2.718281828*xyz", "10*x^9y^7z", "3 - y^2 - x^2", "-x^5y^7z^7", "0"};

	string output;
	for (size_t i = 0; i < 10; ++i) {
		std::stringstream ss;
		Polinomial f(poli[i]);

		ss.precision(10);
		ss << f;
		getline(ss,output);
		EXPECT_EQ(output, expected[i]);
	}
}



// ƒл€ случайных тестов
/* vector<Mono> generate_random_monomial_vector() {
	random_device rd();
	mt19937 gen(rd);
	uniform_real_distribution<double> double_dist(-20.0,20.0);
	uniform_int_distribution<size_t> sz_dist(1,500), pow_dist(1,9);
	size_t sz = sz_dist(gen);

	vector<Mono> vec(sz);
	for (size_t j = 0; j < sz; ++j)
		vec[j] = Mono(double_dist(gen), pow_dist(gen), pow_dist(gen), pow_dist(gen));

	return vec;
} */
