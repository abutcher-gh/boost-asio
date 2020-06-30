//
// execution/executor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2020 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BOOST_ASIO_EXECUTION_EXECUTOR_HPP
#define BOOST_ASIO_EXECUTION_EXECUTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <boost/asio/detail/config.hpp>
#include <boost/asio/detail/type_traits.hpp>
#include <boost/asio/execution/execute.hpp>
#include <boost/asio/execution/invocable_archetype.hpp>
#include <boost/asio/traits/equality_comparable.hpp>

#if defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_FREE_TRAIT) \
  && defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT) \
  && defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)
# define BOOST_ASIO_HAS_DEDUCED_EXECUTION_IS_EXECUTOR_TRAIT 1
#endif // defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_FREE_TRAIT)
       //   && defined(BOOST_ASIO_HAS_DEDUCED_EXECUTE_MEMBER_TRAIT)
       //   && defined(BOOST_ASIO_HAS_DEDUCED_EQUALITY_COMPARABLE_TRAIT)

#include <boost/asio/detail/push_options.hpp>

namespace boost {
namespace asio {
namespace execution {
namespace detail {

template <typename T, typename F>
struct is_executor_of_impl :
  integral_constant<bool,
    conditional<true, true_type,
        typename result_of<typename decay<F>::type&()>::type
      >::type::value
      && is_constructible<typename decay<F>::type, F>::value
      && is_move_constructible<typename decay<F>::type>::value
      && can_execute<T, F>::value
#if defined(BOOST_ASIO_HAS_NOEXCEPT)
      && is_nothrow_copy_constructible<T>::value
      && is_nothrow_destructible<T>::value
#else // defined(BOOST_ASIO_HAS_NOEXCEPT)
      && is_copy_constructible<T>::value
      && is_destructible<T>::value
#endif // defined(BOOST_ASIO_HAS_NOEXCEPT)
      && traits::equality_comparable<T>::is_valid
      && traits::equality_comparable<T>::is_noexcept
  >
{
};

template <typename T, typename = void>
struct executor_shape
{
  typedef std::size_t type;
};

template <typename T>
struct executor_shape<T,
    typename void_type<
      typename T::shape_type
    >::type>
{
  typedef typename T::shape_type type;
};

} // namespace detail

/// The is_executor trait detects whether a type T satisfies the
/// execution::executor concept.
/**
 * Class template @c is_executor is a UnaryTypeTrait that is derived from @c
 * true_type if the type @c T meets the concept definition for an executor,
 * otherwise @c false_type.
 */
template <typename T>
struct is_executor :
#if defined(GENERATING_DOCUMENTATION)
  integral_constant<bool, automatically_determined>
#else // defined(GENERATING_DOCUMENTATION)
  detail::is_executor_of_impl<T, invocable_archetype>
#endif // defined(GENERATING_DOCUMENTATION)
{
};

#if defined(BOOST_ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T>
BOOST_ASIO_CONSTEXPR const bool is_executor_v = is_executor<T>::value;

#endif // defined(BOOST_ASIO_HAS_VARIABLE_TEMPLATES)

#if defined(BOOST_ASIO_HAS_CONCEPTS)

template <typename T>
BOOST_ASIO_CONCEPT executor = is_executor<T>::value;

#define BOOST_ASIO_EXECUTION_EXECUTOR ::boost::asio::execution::executor

#else // defined(BOOST_ASIO_HAS_CONCEPTS)

#define BOOST_ASIO_EXECUTION_EXECUTOR typename

#endif // defined(BOOST_ASIO_HAS_CONCEPTS)

/// The is_executor_of trait detects whether a type T satisfies the
/// execution::executor_of concept for some set of value arguments.
/**
 * Class template @c is_executor_of is a type trait that is derived from @c
 * true_type if the type @c T meets the concept definition for a executor for
 * value arguments @c Vs, otherwise @c false_type.
 */
template <typename T, typename F>
struct is_executor_of :
#if defined(GENERATING_DOCUMENTATION)
  integral_constant<bool, automatically_determined>
#else // defined(GENERATING_DOCUMENTATION)
  integral_constant<bool,
    is_executor<T>::value && detail::is_executor_of_impl<T, F>::value
  >
#endif // defined(GENERATING_DOCUMENTATION)
{
};

#if defined(BOOST_ASIO_HAS_VARIABLE_TEMPLATES)

template <typename T, typename F>
BOOST_ASIO_CONSTEXPR const bool is_executor_of_v =
  is_executor_of<T, F>::value;

#endif // defined(BOOST_ASIO_HAS_VARIABLE_TEMPLATES)

#if defined(BOOST_ASIO_HAS_CONCEPTS)

template <typename T, typename F>
BOOST_ASIO_CONCEPT executor_of = is_executor_of<T, F>::value;

#define BOOST_ASIO_EXECUTION_EXECUTOR_OF(f) \
  ::boost::asio::execution::executor_of<f>

#else // defined(BOOST_ASIO_HAS_CONCEPTS)

#define BOOST_ASIO_EXECUTION_EXECUTOR_OF typename

#endif // defined(BOOST_ASIO_HAS_CONCEPTS)

/// The executor_shape trait detects the type used by an executor to represent
/// the shape of a bulk operation.
/**
 * Class template @c executor_shape is a type trait with a nested type alias
 * @c type whose type is @c T::shape_type if @c T::shape_type is valid,
 * otherwise @c std::size_t.
 */
template <typename T>
struct executor_shape
#if !defined(GENERATING_DOCUMENTATION)
  : detail::executor_shape<T>
#endif // !defined(GENERATING_DOCUMENTATION)
{
#if defined(GENERATING_DOCUMENTATION)
 /// @c T::shape_type if @c T::shape_type is valid, otherwise @c std::size_t.
 typedef automatically_determined type;
#endif // defined(GENERATING_DOCUMENTATION)
};

#if defined(BOOST_ASIO_HAS_ALIAS_TEMPLATES)

template <typename T>
using executor_shape_t = typename executor_shape<T>::type;

#endif // defined(BOOST_ASIO_HAS_ALIAS_TEMPLATES)

} // namespace execution
} // namespace asio
} // namespace boost

#include <boost/asio/detail/pop_options.hpp>

#endif // BOOST_ASIO_EXECUTION_EXECUTOR_HPP
