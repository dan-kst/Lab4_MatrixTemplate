#ifndef UNIQUE_TYPE_WRAP_HPP
#define UNIQUE_TYPE_WRAP_HPP

namespace Core::Wrap
{
template <typename T, typename TAG> struct UniqueTypeWrap
{
  T value;

  // Explicit constructor prevents accidental conversions
  explicit UniqueTypeWrap (T val) : value (val) {}

  // Allow conversion back to the underlying type when needed
  operator T () const { return value; }
};
}
#endif
