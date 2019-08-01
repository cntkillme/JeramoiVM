#pragma once

/// A type_list holds a sequence of types and provides helpers to utilize the type_list.
template<typename... Types>
struct type_list;

using index_t = unsigned long long; ///< The index type, used to specify the position of a type in a type_list.

namespace type_list_implementation
{
	template<index_t Index, typename Head, typename... Tail>
	struct get_impl : get_impl<Index - 1, Tail...> { };

	template<typename Head, typename... Tail>
	struct get_impl<0, Head, Tail...>
	{
		using type = Head;
	};

	template<typename... Types>
	struct append_impl
	{
		using type = type_list<Types...>;
	};

	template<typename TypeList1, typename TypeList2>
	struct join_impl;

	template<typename... Types, typename... OtherTypes>
	struct join_impl<type_list<Types...>, type_list<OtherTypes...>> : append_impl<Types..., OtherTypes...> { };

	template<index_t Index, index_t Count, typename... Types>
	struct subsequence_impl
	{
		using type = typename join_impl<type_list<typename get_impl<Index, Types...>::type>,
			typename subsequence_impl<Index + 1, Count - 1, Types...>::type>::type;
	};

	template<index_t Index, typename... Types>
	struct subsequence_impl<Index, 0, Types...>
	{
		using type = type_list<>;
	};
}

namespace type_list_implementation
{
	template<typename Type, index_t Count, typename TypeList>
	struct make_type_list_impl :
		make_type_list_impl<Type, Count - 1, typename TypeList::template push_back<Type>> { };

	template<typename Type, typename TypeList>
	struct make_type_list_impl<Type, 0, TypeList>
	{
		using type = TypeList;
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
		using get = HELPER_NOT_AVAILABLE_FOR_EMPTY_TYPE_LIST;

		/// A type_list consisting of a subsequence of types from [Index, Index + Count).
		template<index_t Index, index_t Count>
		using subsequence = typename subsequence_impl<Index, Count, Types...>::type;

		// An identical type_list except with extra types inserted at the back.
		template<typename... OtherTypes>
		using append = typename append_impl<Types..., OtherTypes...>::type;

		/// A type_list consisting of the types and the types of another type_list appended.
		template<typename TypeList>
		using join = typename join_impl<type_list<Types...>, TypeList>::type;

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
		template<index_t Index>
		using get = typename get_impl<Index, Types...>::type;

		template<index_t Index, typename Type>
		using replace = typename empty_type_list<Types...>::template subsequence<0, Index>
			::template push_back<Type>::template join<typename empty_type_list<Types...>
			::template subsequence<Index + 1, empty_type_list<Types...>::size - Index - 1>>;

		using pop_front = typename empty_type_list<Types...>
			::template subsequence<1, empty_type_list<Types...>::size - 1>;

		using pop_back = typename empty_type_list<Types...>
			::template subsequence<0, empty_type_list<Types...>::size - 1>;
	};
}

template<typename... Types>
struct type_list : type_list_implementation::empty_type_list<Types...> { };

template<typename Head, typename... Rest>
struct type_list<Head, Rest...> : type_list_implementation::populated_type_list<Head, Rest...> { };

template<typename Type, index_t Count>
using make_type_list = typename type_list_implementation::template make_type_list_impl<Type, Count, type_list<>>::type;
