#pragma once
#include "list.h"
#include <string>
#include <limits>
#include <initializer_list>

class Polinomial {
private:

	// �������� (�����)
	struct Monomial {
		double value;
		size_t power;

		Monomial(double value = 0.0, size_t power = 0): power(power), value(value) {}
		
		Monomial operator*(const Monomial &mon) const {
			Monomial result = *this;

			// ������� ��� ��������� ���������� ������ 9
			if (power/100 + mon.power/100 > 9 || power/10%10 + mon.power/10%10 > 9 || power%10 + mon.power%10 > 9)
				throw std::string("error: the power of monomial is too big");
			result.power = power + mon.power;
			result.value = value * mon.value;

			return result;
		}

		// ��� ������� ���������
		bool operator==(const Monomial &mon) const {
			return abs(value-mon.value)<std::numeric_limits<double>::epsilon() && power == mon.power;
		}
		bool operator!=(const Monomial &mon) const {
			return abs(value-mon.value)>std::numeric_limits<double>::epsilon() || power != mon.power;
		}
		// ��� ������� ������� ��� ����������
		bool operator<=(const Monomial &mon) const {
			return power <= mon.power;
		}
	};

	void reduction(List<Monomial> &list); // ���������� �������� ��������� � ���������� � �������������� ����������
	void parse(const std::string &s); // ������� ������ � �����������
	List<Monomial> list;

public:

	// ��������������� ��������� ��� �������� ������� ����� ���������� ��������, � �� ������� ������
	struct Mono {
		double value;
		size_t pow1, pow2, pow3;

		Mono(double value = 0.0, size_t pow1 = 0, size_t pow2 = 0, size_t pow3 = 0):
			value(value), pow1(pow1), pow2(pow2), pow3(pow3) {}
	};

	Polinomial();
	Polinomial(const Polinomial &copy);
	Polinomial(Polinomial &&move) noexcept;
	Polinomial(const std::string &str);
	Polinomial(const std::initializer_list<Mono> &init);
	Polinomial& operator=(const Polinomial &copy);
	Polinomial& operator=(Polinomial &&move) noexcept;

	Polinomial& operator*=(const double value);
	Polinomial operator*(const double value);

	Polinomial& operator+=(const Polinomial &pol);
	Polinomial operator+(const Polinomial &pol) const;
	Polinomial& operator-=(const Polinomial &pol);
	Polinomial operator-(const Polinomial &pol) const;
	Polinomial operator-() const;
	Polinomial& operator*=(const Polinomial &pol);
	Polinomial operator*(const Polinomial &pol) const;

	bool operator==(const Polinomial &pol) const;
	bool operator!=(const Polinomial &pol) const;

	friend std::istream& operator>>(std::istream &ist, Polinomial &pol);
	friend std::ostream& operator<<(std::ostream &ost, const Polinomial &pol);
};

Polinomial operator*(const double value, const Polinomial &pol);

using Mono = Polinomial::Mono;