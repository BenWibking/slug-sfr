#ifndef SFR_H_
#define SFR_H_

#include <cstdio>
#include "slug_wrapper.h"

// template magic

// Iterate over 2 tuples simultaneously, e.g.
//  for_each2(tuple1, tuple2, [](auto&& i, auto&& s){ std::cout << i << '-' << s << '\n'; });
//  [credit: https://www.fluentcpp.com/2019/03/08/stl-algorithms-on-tuples/]

template <class Tuple1, class Tuple2, class F, std::size_t... I>
F for_each2_impl(Tuple1 &&t1, Tuple2 &&t2, F &&f, std::index_sequence<I...>)
{
  return (void)std::initializer_list<int>{(std::forward<F>(f)(I, std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))), 0)...}, f;
}
template <class Tuple1, class Tuple2, class F>
constexpr decltype(auto) for_each2(Tuple1 &&t1, Tuple2 &&t2, F &&f)
{
  return for_each2_impl(std::forward<Tuple1>(t1), std::forward<Tuple2>(t2), std::forward<F>(f),
                       std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple1>>::value>{});
}

// Get a string representation of a type at compile-time.
//  [credit: https://stackoverflow.com/a/56766138]

#include <string_view>
template <typename T>
constexpr auto type_name() noexcept {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}

// function declarations

auto test_slug() -> bool;

#endif // SFR_H_
