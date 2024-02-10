// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from eufs_msgs:msg/FullState.idl
// generated code does not contain a copyright notice

#ifndef EUFS_MSGS__MSG__DETAIL__FULL_STATE__TRAITS_HPP_
#define EUFS_MSGS__MSG__DETAIL__FULL_STATE__TRAITS_HPP_

#include "eufs_msgs/msg/detail/full_state__struct.hpp"
#include <rosidl_runtime_cpp/traits.hpp>
#include <stdint.h>
#include <type_traits>

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"

namespace rosidl_generator_traits
{

template<>
inline const char * data_type<eufs_msgs::msg::FullState>()
{
  return "eufs_msgs::msg::FullState";
}

template<>
inline const char * name<eufs_msgs::msg::FullState>()
{
  return "eufs_msgs/msg/FullState";
}

template<>
struct has_fixed_size<eufs_msgs::msg::FullState>
  : std::integral_constant<bool, has_fixed_size<std_msgs::msg::Header>::value> {};

template<>
struct has_bounded_size<eufs_msgs::msg::FullState>
  : std::integral_constant<bool, has_bounded_size<std_msgs::msg::Header>::value> {};

template<>
struct is_message<eufs_msgs::msg::FullState>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // EUFS_MSGS__MSG__DETAIL__FULL_STATE__TRAITS_HPP_
