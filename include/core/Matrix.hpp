#ifndef MATRIX_HPP
#define MATRIX_HPP

#include "ExpressionTemplate.hpp"
#include "StoragePolicy.hpp"
#include <concepts>
#include <iostream>
#include <ranges>

namespace Core
{
template <typename T>
concept MatrixElement = std::semiregular<T> && requires (T a, T b)
{
  {
    a < b
  } -> std::convertible_to<bool>;
  { std::cout << a };
};

/**
 * @brief The unified Matrix class.
 * @tparam T The element type.
 * @tparam STORAGE Type of storage (Dense or Sparse).
 * @tparam ALLOC The allocator type.
 */
template <MatrixElement T,
          template <typename, typename> class STORAGE = StoragePolicy::Dense,
          typename ALLOC = std::allocator<T> >
class Matrix : public MatrixExpression<Matrix<T, STORAGE, ALLOC> >
{
public:
  using value_type = T;
  using allocator_type = ALLOC;
  using size_type = std::size_t;

  // --- Constructors ---

  /**
   * @brief Standard constructor using wrapped types for safety.
   */
  Matrix (StoragePolicy::Row rows, StoragePolicy::Column cols,
          const T &initial = T (), const ALLOC &alloc = ALLOC ())
      : m_rows_ (static_cast<size_type> (rows)),
        m_cols_ (static_cast<size_type> (cols)),
        m_storage_ (rows, cols, initial, alloc)
  {
  }

  /**
   * @brief Expression Constructor
   * @param expr The expression to evaluate.
   * @param alloc Allocator instance.
   */
  template <typename E>
  explicit Matrix (const MatrixExpression<E> &expr,
                   const ALLOC &alloc = ALLOC ())
      : m_rows_ (expr.rows ()), m_cols_ (expr.cols ()),
        m_storage_ (StoragePolicy::Row (expr.rows ()),
                    StoragePolicy::Column (expr.cols ()), T (), alloc)
  {
    for (size_type row = 0; row < m_rows_; ++row)
      {
        for (size_type col = 0; col < m_cols_; ++col)
          {
            // Internal calculation using size_t
            (*this) (StoragePolicy::Row{ row }, StoragePolicy::Column{ col })
                = expr (row, col);
          }
      }
  }

  // --- Core Operations ---
  /**
   * @brief Inserts an element with bounds checking.
   */
  void
  insert (size_type row, size_type col, const T &val)
  {
    if (row >= m_rows_ || col >= m_cols_)
      {
        throw std::out_of_range ("Matrix index out of bounds");
      }
    (*this) (StoragePolicy::Row (row), StoragePolicy::Column (col)) = val;
  }

  /**
   * @brief Checks if a row or column is ordered.
   * @param index The index of the row or column.
   * @param isRow True for row, false for column.
   * @param ascending True for ascending, false for descending.
   */
  [[nodiscard]] auto
  isOrdered (size_type index, bool isRow, bool ascending = true) const -> bool
  {
    auto getVal = [&] (size_type col) -> const T & {
      return isRow ? (*this) (index, col) : (*this) (col, index);
    };

    size_type limit = isRow ? m_cols_ : m_rows_;
    for (size_type i = 1; i < limit; ++i)
      {
        if (ascending)
          {
            if (getVal (i) < getVal (i - 1))
              {
                return false;
              }
          }
        else
          {
            if (getVal (i - 1) < getVal (i))
              {
                return true;
              }
          }
      }
    return true;
  }

  [[nodiscard]] auto
  operator() (size_type row, size_type col) -> T &
  {
    return m_storage_ (StoragePolicy::Row{ row },
                       StoragePolicy::Column{ col });
  }
  [[nodiscard]] auto
  operator() (size_type row, size_type col) const -> const T &
  {
    return m_storage_ (StoragePolicy::Row{ row },
                       StoragePolicy::Column{ col });
  }

  [[nodiscard]] auto
  rows () const noexcept -> size_type
  {
    return m_rows_;
  }
  [[nodiscard]] auto
  cols () const noexcept -> size_type
  {
    return m_cols_;
  }

private:
  size_type m_rows_;
  size_type m_cols_;
  STORAGE<T, ALLOC> m_storage_;
};
}

#endif // MATRIX_HPP
