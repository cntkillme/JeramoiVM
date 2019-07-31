#pragma once

/// A value_list holds a sequence of values and provides helpers to utilize the value_list.
template<auto... Values>
struct value_list;

using index_t = unsigned long long; ///< The index type, used to specify the position of a value in a value_list.

namespace value_list_implementation
{
	template<index_t Index, auto Head, auto... Tail>
	struct get : get<Index - 1, Tail...> { };

	template<auto Head, auto... Tail>
	struct get<0, Head, Tail...>
	{
		static constexpr auto value = Head;
	};

	template<auto... Values>
	struct append
	{
		using type = value_list<Values...>;
	};

	template<typename ValueList1, typename ValueList2>
	struct join;

	template<auto... Values, auto... OtherValues>
	struct join<value_list<Values...>, value_list<OtherValues...>> : append<Values..., OtherValues...> { };

	template<index_t Index, index_t Count, auto... Values>
	struct subsequence
	{
		using type = typename join<value_list<get<Index, Values...>::value>,
			typename subsequence<Index + 1, Count - 1, Values...>::type>::type;
	};

	template<index_t Index, auto... Values>
	struct subsequence<Index, 0, Values...>
	{
		using type = value_list<>;
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
		static inline constexpr auto value = HELPER_NOT_AVAILABLE_FOR_EMPTY_VALUE_LIST();

		/// A value_list consisting of a subsequence of values from [Index, Index + Count).
		template<index_t Index, index_t Count>
		using subsequence = typename value_list_implementation::template subsequence<Index, Count>::type;

		// An identical value_list except with extra values inserted at the back.
		template<auto... OtherValues>
		using append = typename value_list_implementation::template append<Values..., OtherValues...>::type;

		/// A value_list consisting of the values and the values of another value_list appended.
		template<typename ValueList>
		using join = typename value_list_implementation::template join<value_list<Values...>, ValueList>::type;

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
		using empty_value_list<Values...>::size;

		template<index_t Index>
		static inline constexpr auto value = value_list_implementation::template get<Index, Values...>::value;

		template<index_t Index, auto Value>
		using replace = typename subsequence<0, Index>::template push_back<Value>
			::template join<subsequence<Index + 1, size - Index>>;

		using pop_front = subsequence<1, size - 1>;

		using pop_back = subsequence<0, size - 1>;
	};
}

template<auto... Values>
struct value_list : value_list_implementation::empty_value_list<Values...> { };

template<auto Head, auto... Rest>
struct value_list<Head, Rest...> : value_list_implementation::populated_value_list<Head, Rest...> { };
