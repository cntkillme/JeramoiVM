#include <iostream>
#include "type_list.hpp"

int main()
{
	using a1 = type_list<>;
	using b1 = type_list<int>;

	using a2 = a1::template push_back<int>;
	using b2 = b1::template push_back<int>;

	std::cout << "a1 size: " << a1::size << "\n";
	std::cout << "b1 size: " << b1::size << "\n";
	std::cout << "a2 size: " << a2::size << "\n";
	std::cout << "b2 size: " << b2::size << "\n";

	return 0;
}
