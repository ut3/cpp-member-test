/* SPDX-License-Identifier: GPL-2.0-only
 *  
 * A minimum viable example of testing for member existence using
 * SFINAE, and of using that test to alter execution flow via 
 * std::enable_if.
 *
 * Copyright 2020 J Rick Ramstetter, rick.ramstetter@gmail.com
 *
 * This file is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * This code may be dual-licensed under additional commercial
 * licenses by its author, J Rick Ramstetter. 
 */

#include <iostream>
#include <type_traits>

template <typename T, typename = int>
struct TestHasSpecial : public std::false_type
{};

template <typename T>
struct TestHasSpecial<T, decltype(T::special, 0)> : public std::true_type
{
};

struct Yes { int special; Yes() : special(111) { } };
struct No { };

void Print(int value) { std::cout << "Print() called with " << value << std::endl; };

template <class T, typename std::enable_if<TestHasSpecial<T>{}, int>::type = 0>
void PrintSpecialOrDefault(const T& obj) {
	Print(obj.special);
}

template <class T, typename std::enable_if<!TestHasSpecial<T>{}, int>::type = 0>
void PrintSpecialOrDefault(const T& obj) {
	constexpr auto defaultVal = 222;
	Print(defaultVal);
}

int main(void) {
	std::cout << "Yes=" << TestHasSpecial<Yes>::value << ", No=" << TestHasSpecial<No>::value << std::endl;
	Yes yes;
	No no;
	PrintSpecialOrDefault(yes);
	PrintSpecialOrDefault(no);
}
