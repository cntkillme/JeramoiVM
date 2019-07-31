#include <iostream>
#include <type_traits>
#include "context.hpp"

int main()
{
	using tl = type_list<>
		::push_back<char>
		::push_back<short>
		::push_back<int>;

	using vl = value_list<>
		::push_back<'a'>
		::push_back<static_cast<short>(1)>
		::push_back<5000000>;

	std::cout << std::boolalpha;
	std::cout << "type_list success: " << (
		std::is_same_v<tl::type<0>, char> &&
		std::is_same_v<tl::type<1>, short> &&
		std::is_same_v<tl::type<2>, int>);
	std::cout << "\n";

	std::cout << std::boolalpha;
	std::cout << "value_list success: " << (
		vl::value<0> == 'a' &&
		vl::value<1> == static_cast<short>(1) &&
		vl::value<2> == 5000000);
	std::cout << "\n";

	return 0;
}
