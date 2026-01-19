#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <iostream>
#include "ExpressionTemplate.hpp"

namespace Core
{
/**
 * @brief Concept to ensure Matrix elements are printable and comparable.
 */
template <typename T>
concept MatrixElement = std::semiregular<T> && requires (T a, T b)
{
  {
    a < b
  } -> std::convertible_to<bool>;
  { std::cout << a };
};

/**
 * @brief A templated 2D Matrix class using a flat storage model.
 */
template <MatrixElement T, typename Alloc = std::allocator<T> >
class Matrix : public MatrixExpression<Matrix<T, Alloc> >
{
public:
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;

  // --- Constructors ---
  /**
   * @brief Expression Constructor.
   * This is where the actual computation happens for operations like A + B + C.
   */
  template <typename E>
  Matrix (const MatrixExpression<E> &expr)
      : m_rows (expr.rows ()), m_cols (expr.cols ()),
        m_data (expr.rows () * expr.cols ())
  {
    for (size_t r = 0; r < m_rows; ++r)
      {
        for (size_t c = 0; c < m_cols; ++c)
          {
            (*this) (r, c) = expr (r, c);
          }
      }
  }

  Matrix (size_type rows, size_type cols,
          const value_type &initial = value_type (),
          const allocator_type &alloc = allocator_type ())
      : m_rows (rows), m_cols (cols), m_data (rows * cols, initial, alloc)
  {
  }

  // --- Core Operations ---
  /**
   * @brief Inserts or updates an element at specified coordinates.
   */
  void
  insert (size_type row, size_type col, const T &value)
  {
    if (row >= m_rows || col >= m_cols)
      {
        throw std::out_of_range ("Matrix indices out of bounds");
      }
    m_data[row * m_cols + col] = value;
  }
  /**
   * @brief Sorts a specific row or column.
   */
  void
  sort (size_type index, bool isRow)
  {
    if (isRow)
      {
        auto row_begin = m_data.begin () + (index * m_cols);
        std::stable_sort (row_begin, row_begin + m_cols);
      }
    else
      {
        // Sorting columns in flat storage is more expensive
        std::vector<T> col_data;
        for (size_type r = 0; r < m_rows; ++r)
          col_data.push_back ((*this) (r, index));
        std::sort (col_data.begin (), col_data.end ());
        for (size_type r = 0; r < m_rows; ++r)
          (*this) (r, index) = col_data[r];
      }
  }
  /**
   * @brief Checks if a row or column is ordered.
   * @param index The index of the row or column.
   * @param isRow True for row, false for column.
   * @param ascending True for ascending, false for descending.
   */
  bool
  isOrdered (size_type index, bool isRow, bool ascending = true) const
  {
    auto get_val = [&] (size_type i) -> const T & {
      return isRow ? (*this) (index, i) : (*this) (i, index);
    };

    size_type limit = isRow ? m_cols : m_rows;
    for (size_type i = 1; i < limit; ++i)
      {
        if (ascending)
          {
            if (get_val (i) < get_val (i - 1))
              return false;
          }
        else
          {
            if (get_val (i - 1) < get_val (i))
              return false;
          }
      }
    return true;
  }

  // --- Accessors ---
  T &
  operator() (size_type row, size_type col)
  {
    return m_data[row * m_cols + col];
  }
  const T &
  operator() (size_type row, size_type col) const
  {
    return m_data[row * m_cols + col];
  }

  size_type
  rows () const
  {
    return m_rows;
  }
  size_type
  cols () const
  {
    return m_cols;
  }

  // --- Iterators ---
  auto
  begin ()
  {
    return m_data.begin ();
  }
  auto
  end ()
  {
    return m_data.end ();
  }
  auto
  begin () const
  {
    return m_data.begin ();
  }
  auto
  end () const
  {
    return m_data.end ();
  }

private:
  size_type m_rows;
  size_type m_cols;
  std::vector<T, Alloc> m_data;
};
}

#endif // MATRIX_HPP
