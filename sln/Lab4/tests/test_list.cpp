#include "pch.h"
#include "../../../include/list.h"
#include <vector>
#include <chrono>
#include <random>
using namespace std;

TEST(List, can_create_empty_list) {
	ASSERT_NO_THROW(List<int> list);
}

TEST(List, can_create_with_certain_size) {
	ASSERT_NO_THROW(List<int> list(3));
}

TEST(List, can_create_via_initializer_list) {
	ASSERT_NO_THROW(List<int> list{2});
}

TEST(List, can_create_via_copy_constructor) {
	List<int> list1{1,2,3,4,5};
	ASSERT_NO_THROW(List<int> list2(list1); ASSERT_EQ(list1, list2));
}

TEST(List, can_create_via_move_constructor) {
	List<int> list1{1,2,3,4,5};
	ASSERT_NO_THROW(List<int> list2 = move(list1));
}

TEST(List, can_create_via_range_constructor) {
	vector<int> vec{1,1,2,3,5,8,13,21};
	ASSERT_NO_THROW(List<int> list(vec.begin(), vec.end()));
}

// Перед проверкой корректности конструкторов и операторов присваивания нужно проверить
// корректность итераторов, чтобы иметь доступ к элементам списка, а также метод, выдающий размер списка
TEST(List, size_method_is_correct) {
	List<int> list1{1,2,3}, list2, list3(42);

	EXPECT_EQ(list1.size(), 3);
	EXPECT_EQ(list2.size(), 0);
	EXPECT_EQ(list3.size(), 42);

}
TEST(List, can_create_before_begin_iterator) {
	List<int> list1{3342346};
	ASSERT_NO_THROW(auto it = list1.before_begin());
}

TEST(List, can_create_correct_begin_iterator) {
	List<int> list1{3342346};
	ASSERT_NO_THROW(auto it = list1.begin(); EXPECT_EQ(*it, 3342346));
}

TEST(List, can_create_end_iterator) {
	List<int> list1{3342346,2,7};
	ASSERT_NO_THROW(auto it = list1.end());
}

TEST(List, can_create_cbefore_begin_iterator) {
	List<int> list1{3342346};
	ASSERT_NO_THROW(auto it = list1.cbefore_begin());
}

TEST(List, can_create_correct_cbegin_iterator) {
	List<int> list1{3342346};
	ASSERT_NO_THROW(auto it = list1.cbegin(); EXPECT_EQ(*it, 3342346));
}

TEST(List, can_create_cend_iterator) {
	List<int> list1{3342346,2,7};
	ASSERT_NO_THROW(auto it = list1.cend());
}

TEST(List, iterator_prefix_increment_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.before_begin(), it2 = list.begin();

	++it1, ++it2;
	EXPECT_EQ(*it1, 3342346);
	EXPECT_EQ(*it2, 2);
}

TEST(List, iterator_postfix_increment_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.begin(), it2 = next(list.begin());

	EXPECT_EQ(*(it1++), 3342346);
	EXPECT_EQ(*(it2++), 2);
	EXPECT_EQ(*it2, 7);
}

TEST(List, iterator_comparing_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.before_begin(), it2 = list.begin(), it3 = list.end();

	EXPECT_EQ(++it1 == it2, true);
	EXPECT_EQ(it2 == it3, false);
	EXPECT_EQ(it1 != it2, false);
	EXPECT_EQ(it2 != it3, true);
}

TEST(List, const_iterator_prefix_increment_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.cbefore_begin(), it2 = list.cbegin();

	++it1, ++it2;
	EXPECT_EQ(*it1, 3342346);
	EXPECT_EQ(*it2, 2);
}

TEST(List, const_iterator_postfix_increment_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.cbegin(), it2 = next(list.cbegin());

	EXPECT_EQ(*(it1++), 3342346);
	EXPECT_EQ(*(it2++), 2);
	EXPECT_EQ(*it2, 7);
}

TEST(List, const_iterator_comparing_is_correct) {
	List<int> list{3342346,2,7};
	auto it1 = list.cbefore_begin(), it2 = list.cbegin(), it3 = list.cend();

	EXPECT_EQ(++it1 == it2, true);
	EXPECT_EQ(it2 == it3, false);
	EXPECT_EQ(it1 != it2, false);
	EXPECT_EQ(it2 != it3, true);
}

// Корректность конструкторов и операторов присваивания
TEST(List, initializer_list_constructor_is_correct) {
	List<int> list{1,3,6,10,15,21,28};
	vector<int> vec{1,3,6,10,15,21,28};
	auto list_it = list.begin();

	for (size_t i = 0; i < list.size(); ++i) {
		EXPECT_EQ(*list_it, vec[i]);
		++list_it;
	}
}

// Корректность конструкторов и операторов присваивания

// "Генератор случайных векторов"
vector<int> random_int_vector() {
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1,500);
	size_t sz = dist(gen);

	vector<int> vec(sz);
	for (size_t j = 0; j < sz; ++j)
		vec[j] = gen();

	return vec;
}

TEST(List, range_constructor_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();

		List<int> list(vec.begin(), vec.end());

		auto it = list.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, copy_constructor_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();

		List<int> list1(vec.begin(), vec.end());
		List<int> list2 = list1;

		auto it = list2.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, move_constructor_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();

		List<int> list1(vec.begin(), vec.end());
		List<int> list2 = std::move(list1);

		auto it = list2.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, copy_assignment_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();

		List<int> list1(vec.begin(), vec.end()), list2;
		list2 = list1;

		auto it = list2.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, move_assignment_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();

		List<int> list1(vec.begin(), vec.end()), list2;
		list2 = std::move(list1);

		auto it = list2.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, eq_operator_is_correct) {
	List<int> list1{3,1,4,1,5,9}, list2{3,1,4,1,5,9},
		list3{3,1,4,1,5,9}, list4{2,7,1,8,2,8};

	EXPECT_EQ(list1 == list2, true);
	EXPECT_EQ(list3 == list4, false);
}

TEST(List, neq_operator_is_correct) {
	List<int> list1{3,1,4,1,5,9}, list2{3,1,4,1,5,9},
		list3{3,1,4,1,5,9}, list4{2,7,1,8,2,8};

	EXPECT_EQ(list1 != list2, false);
	EXPECT_EQ(list3 != list4, true);
}

TEST(List, empty_is_correct) {
	List<int> list1{1}, list2;

	EXPECT_EQ(list1.empty(), false);
	EXPECT_EQ(list2.empty(), true);
}

TEST(List, clear_is_correct) {
	List<int> list1{1,2,3,4,5}, list2;
	ASSERT_NO_THROW(list1.clear(); list2.clear());

	EXPECT_EQ(list1.empty(), true);
	EXPECT_EQ(list2.empty(), true);
}

TEST(List, swap_is_correct) {
	List<int> list1{3,5,3,1}, list2{8,6,11};
	List<int> list1_copy = list1, list2_copy = list2;

	ASSERT_NO_THROW(swap(list1_copy,list2_copy));

	EXPECT_EQ(list1_copy, list2);
	EXPECT_EQ(list2_copy, list1);
}

TEST(List, reverse_is_correct) {
	List<int> list1{1,2,3,4}, list2{1}, list3;
	List<int> list1_r{4,3,2,1}, list2_r{1}, list3_r;

	ASSERT_NO_THROW(list1.reverse());
	ASSERT_NO_THROW(list2.reverse());
	ASSERT_NO_THROW(list3.reverse());

	EXPECT_EQ(list1, list1_r);
	EXPECT_EQ(list2, list2_r);
	EXPECT_EQ(list3, list3_r);
}

TEST(List, merge_is_correct) {
	List<int> list1{1,3,5,7}, list2{2,4,6,8}, list3{1,2,3,4,5,6,7,8};

	ASSERT_NO_THROW(list1.merge(list2));
	EXPECT_EQ(list1, list3);
}

TEST(List, sort_is_correct) {
	// Конкретный тест
	List<int> list{6,2,5,1,8,4,7,3}, list_sort{1,2,3,4,5,6,7,8};

	ASSERT_NO_THROW(list.sort());
	EXPECT_EQ(list, list_sort);

	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();
		List<int> list(vec.begin(), vec.end());

		sort(vec.begin(), vec.end()), list.sort();

		auto it = list.begin();
		for (size_t j = 0; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}

TEST(List, insert_after_is_correct) {
	List<int> list1{1,2,3,4}, list2{}, list3{1,0};
	auto it1 = find(list1.begin(), list1.end(), 2), // в середине списка
		it2 = list2.before_begin(), // в начале списка
		it3 = find(list3.begin(), list3.end(), 0); // в конце списка

	ASSERT_NO_THROW(it1 = list1.insert_after(it1, 314159));
	ASSERT_NO_THROW(it2 = list2.insert_after(it2, 17));
	ASSERT_NO_THROW(it3 = list3.insert_after(it3, 6561));

	EXPECT_EQ(*it1, 314159);
	EXPECT_EQ(*it2, 17);
	EXPECT_EQ(*it3, 6561);
}

TEST(List, emplace_after_is_correct) {
	List<pair<int,int>> list1{{1,1},{2,2},{3,3},{4,4}}, list2{}, list3{{1,1},{0,0}};
	auto it1 = find(list1.begin(), list1.end(), pair<int,int>{2,2}), // в середине списка
		it2 = list2.before_begin(), // в начале списка
		it3 = find(list3.begin(), list3.end(), pair<int,int>{0,0}); // в конце списка

	ASSERT_NO_THROW(it1 = list1.emplace_after(it1,22,7));
	ASSERT_NO_THROW(it2 = list2.emplace_after(it2,223,71));
	ASSERT_NO_THROW(it3 = list3.emplace_after(it3,355,113));

	pair<int,int> p1(22,7), p2(223,71), p3(355,113);
	EXPECT_EQ(*it1, p1);
	EXPECT_EQ(*it2, p2);
	EXPECT_EQ(*it3, p3);
}

TEST(List, erase_after_is_correct) {
	List<int> list1{1,2,3,4}, list2{1};
	auto it1 = find(list1.begin(), list1.end(), 2), // в середине списка
		it2 = list2.before_begin(); // в начале списка

	ASSERT_NO_THROW(it1 = list1.erase_after(it1));
	ASSERT_NO_THROW(it2 = list2.erase_after(it2));

	EXPECT_EQ(*it1, 4);
	EXPECT_EQ(it2, list2.end());
}

TEST(List, push_front_is_correct) {
	List<int> list1, list2{9,8,7,6,5,4,3,2,1};

	for (int i = 1; i < 10; ++i)
		ASSERT_NO_THROW(list1.push_front(i));
	EXPECT_EQ(list1, list2);
}

TEST(List, emplace_front_is_correct) {
	List<pair<int,int>> list1, list2{ {5,5},{4,4},{3,3},{2,2},{1,1} };

	for (int i = 1; i < 6; ++i)
		ASSERT_NO_THROW(list1.emplace_front(i,i));
	EXPECT_EQ(list1, list2);
}

TEST(List, pop_front_is_correct) {
	// Рандомные тесты
	for (size_t i = 0; i < 10; ++i) {
		vector<int> vec = random_int_vector();
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1,vec.size());
		size_t num_of_erased = dist(gen);

		List<int> list(vec.begin(), vec.end());
		for (size_t i = 0; i < num_of_erased; ++i)
			list.pop_front();

		auto it = list.begin();
		for (size_t j = num_of_erased; j < vec.size(); ++j) {
			EXPECT_EQ(*it, vec[j]);
			++it;
		}
	}
}