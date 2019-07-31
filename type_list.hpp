#pragma once

/// A type_list holds a sequence of types and provides helpers to utilize the type_list.
template<typename... Types>
struct type_list;

using index_t = unsigned long long; ///< The index type, used to specify the position of a type in a type_list.

namespace type_list_implementation
{
	template<index_t Index, typename Head, typename... Tail>
	struct get : get<Index - 1, Tail...> { };

	template<typename Head, typename... Tail>
	struct get<0, Head, Tail...>
	{
		using type = Head;
	};

	template<typename... Types>
	struct append
	{
		using type = type_list<Types...>;
	};

	template<typename TypeList1, typename TypeList2>
	struct join;

	template<typename... Types, typename... OtherTypes>
	struct join<type_list<Types...>, type_list<OtherTypes...>> : append<Types..., OtherTypes...> { };

	template<index_t Index, index_t Count, typename... Types>
	struct subsequence
	{
		using type = typename join<type_list<typename get<Index, Types...>::type>,
			typename subsequence<Index + 1, Count - 1, Types...>::type>::type;
	};

	template<index_t Index, typename... Types>
	struct subsequence<Index, 0, Types...>
	{
		using type = type_list<>;
	};
}

namespace type_list_implementation
{
	struct HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST { };

	template<typename... Types>
	struct empty_type_list
	{
		/// The number of types.
		static constexpr index_t size = sizeof...(Types);

		/// The type at the given Index.
		template<index_t>
		using type = HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST;

		/// A type_list consisting of a subsequence of types from [Index, Index + Count).
		template<index_t Index, index_t Count>
		using subsequence = typename type_list_implementation::template subsequence<Index, Count>::type;

		// An identical type_list except with extra types inserted at the back.
		template<typename... OtherTypes>
		using append = typename type_list_implementation::template append<Types..., OtherTypes...>::type;

		/// A type_list consisting of the types and the types of another type_list appended.
		template<typename TypeList>
		using join = typename type_list_implementation::template join<type_list<Types...>, TypeList>::type;

		/// An identical type_list except the type at the given Index is replaced with Type.
		template<index_t Index, typename Type>
		using replace = HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST;

		/// An identical type_list except with another Type inserted at Index.
		template<index_t Index, typename Type>
		using insert = typename subsequence<0, Index>::template push_back<Type>
			::template join<subsequence<Index + 1, size - Index>>;

		/// An identical type_list except with another Type inserted in the front.
		template<typename Type>
		using push_front = type_list<Type, Types...>;

		/// An identical type_list except with another Type inserted at the back.
		template<typename Type>
		using push_back = type_list<Types..., Type>;

		/// An identical type_list except with the type in the front removed.
		using pop_front = HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST;

		/// An identical type_list except with the type in the back removed.
		using pop_back = HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST;
	};

	template<typename... Types>
	struct populated_type_list : empty_type_list<Types...>
	{
		using empty_type_list<Types...>::size;

		template<index_t Index>
		using type = typename type_list_implementation::template get<Index, Types...>::type;

		template<index_t Index, typename Type>
		using replace = typename subsequence<0, Index>::template push_back<Type>
			::template join<subsequence<Index + 1, size - Index>>;

		using pop_front = subsequence<1, size - 1>;

		using pop_back = subsequence<0, size - 1>;
	};
}

template<typename... Types>
struct type_list : type_list_implementation::empty_type_list<Types...> { };

template<typename Head, typename... Rest>
struct type_list<Head, Rest...> : type_list_implementation::populated_type_list<Head, Rest...> { };
