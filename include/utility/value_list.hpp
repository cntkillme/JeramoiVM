#pragma once

/// A value_list holds a sequence of values and provides helpers to utilize the value_list.
template<auto... Values>
struct value_list;

using index_t = unsigned long long; ///< The index type, used to specify the position of a value in a value_list.

namespace value_list_implementation
{
	template<index_t Index, auto Head, auto... Tail>
	struct get_impl : get_impl<Index - 1, Tail...> { };

	template<auto Head, auto... Tail>
	struct get_impl<0, Head, Tail...>
	{
		static constexpr auto value = Head;
	};

	template<auto... Values>
	struct append_impl
	{
		using type = value_list<Values...>;
	};

	template<typename ValueList1, typename ValueList2>
	struct join_impl;

	template<auto... Values, auto... OtherValues>
	struct join_impl<value_list<Values...>, value_list<OtherValues...>> : append_impl<Values..., OtherValues...> { };

	template<index_t Index, index_t Count, auto... Values>
	struct subsequence_impl
	{
		using type = typename join_impl<value_list<get_impl<Index, Values...>::value>,
			typename subsequence_impl<Index + 1, Count - 1, Values...>::type>::type;
	};

	template<index_t Index, auto... Values>
	struct subsequence_impl<Index, 0, Values...>
	{
		using type = value_list<>;
	};
}

namespace value_list_implementation
{
	template<typename Type, index_t Count, Type Default, typename ValueList>
	struct make_value_list_impl :
		make_value_list_impl<Type, Count - 1, Default, typename ValueList::template push_back<Default>> { };

	template<typename Type, Type Default, typename ValueList>
	struct make_value_list_impl<Type, 0, Default, ValueList>
	{
		using type = ValueList;
	};
}

namespace value_list_implementation
{
	struct HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST { };

	template<auto... Values>
	struct empty_value_list
	{
		/// The number of values.
		static constexpr index_t size = sizeof...(Values);

		/// The value at the given Index.
		template<index_t>
		static inline constexpr auto get = HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST();

		/// A value_list consisting of a subsequence of values from [Index, Index + Count).
		template<index_t Index, index_t Count>
		using subsequence = typename subsequence_impl<Index, Count, Values...>::type;

		// An identical value_list except with extra values inserted at the back.
		template<auto... OtherValues>
		using append = typename append_impl<Values..., OtherValues...>::type;

		/// A value_list consisting of the values and the values of another value_list appended.
		template<typename ValueList>
		using join = typename join_impl<value_list<Values...>, ValueList>::type;

		/// An identical value_list except the value at the given Index is replaced with Value.
		template<index_t Index, auto Value>
		using replace = HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST;

		/// An identical value_list except with another Value inserted at Index.
		template<index_t Index, auto Value>
		using insert = typename subsequence<0, Index>::template push_back<Value>
			::template join<subsequence<Index + 1, size - Index>>;

		/// An identical value_list except with another Value inserted in the front.
		template<auto Value>
		using push_front = value_list<Value, Values...>;

		/// An identical value_list except with another Value inserted at the back.
		template<auto Value>
		using push_back = value_list<Values..., Value>;

		/// An identical value_list except with the value in the front removed.
		using pop_front = HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST;

		/// An identical value_list except with the value in the back removed.
		using pop_back = HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST;
	};

	template<auto... Values>
	struct populated_value_list : empty_value_list<Values...>
	{
		template<index_t Index>
		static inline constexpr auto get = get_impl<Index, Values...>::value;

		template<index_t Index, auto Value>
		using replace = typename empty_value_list<Values...>::template subsequence<0, Index>
			::template push_back<Value>::template join<typename empty_value_list<Values...>
			::template subsequence<Index + 1, empty_value_list<Values...>::size - Index - 1>>;

		using pop_front = typename empty_value_list<Values...>
			::template subsequence<1, empty_value_list<Values...>::size - 1>;

		using pop_back = typename empty_value_list<Values...>
			::template subsequence<0, empty_value_list<Values...>::size - 1>;
	};
}

template<auto... Values>
struct value_list : value_list_implementation::empty_value_list<Values...> { };

template<auto Head, auto... Rest>
struct value_list<Head, Rest...> : value_list_implementation::populated_value_list<Head, Rest...> { };

template<typename Type, index_t Count, Type Default = Type()>
using make_value_list = typename value_list_implementation
	::template make_value_list_impl<Type, Count, Default, value_list<>>::type;
