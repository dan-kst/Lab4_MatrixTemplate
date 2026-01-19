#ifndef STORAGE_POLICIES_HPP
#define STORAGE_POLICIES_HPP

#include "UniqueTypeWrap.hpp"
#include <concepts>
#include <map>
#include <vector>

namespace Core::StoragePolicy
{
struct RowsTag
{
};
struct ColsTag
{
};

using Row = Wrap::UniqueTypeWrap<std::size_t, RowsTag>;
using Column = Wrap::UniqueTypeWrap<std::size_t, ColsTag>;

/**
 * @brief Dense storage policy using a flat vector.
 */
template <typename T, typename ALLOC = std::allocator<T> > class Dense
{
public:
  Dense (Row row, Column col, const T &init, const ALLOC &alloc)
      : m_data_ (static_cast<std::size_t> (row)
                     * static_cast<std::size_t> (col),
                 init, alloc),
        m_cols_ (col)
  {
  }

  auto
  operator() (Row row, Column col) -> T &
  {
    return m_data_[static_cast<std::size_t> (row)
                       * static_cast<std::size_t> (m_cols_)
                   + static_cast<std::size_t> (col)];
  }

  auto
  operator() (Row row, Column col) const -> const T &
  {
    return m_data_[static_cast<std::size_t> (row)
                       * static_cast<std::size_t> (m_cols_)
                   + static_cast<std::size_t> (col)];
  }

private:
  std::vector<T, ALLOC> m_data_;
  Column m_cols_;
};

/**
 * @brief Sparse storage policy using a map.
 */
template <typename T, typename ALLOC = std::allocator<T> > class Sparse
{
public:
  Sparse (Row row, Column col, const T &init, const ALLOC & /*unused*/)
      : m_cols_ (col), m_defaultValue_ (init)
  {
    (void)row;
  }

  auto
  operator() (Row row, Column col) -> T &
  {
    return m_data_[static_cast<std::size_t> (row)
                       * static_cast<std::size_t> (m_cols_)
                   + static_cast<std::size_t> (col)];
  }

  auto
  operator() (Row row, Column col) const -> const T &
  {
    auto it = m_data_.find (static_cast<std::size_t> (row)
                                * static_cast<std::size_t> (m_cols_)
                            + static_cast<std::size_t> (col));
    if (it == m_data_.end ())
      {
        return m_defaultValue_;
      }
    return it->second;
  }

private:
  std::map<std::size_t, T> m_data_;
  Column m_cols_;
  T m_defaultValue_;
};
} // namespace Core::StoragePolicy

#endif // STORAGE_POLICIES_HPP
