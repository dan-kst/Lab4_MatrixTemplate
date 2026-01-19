#ifndef EXPRESSION_TEMPLATES_HPP
#define EXPRESSION_TEMPLATES_HPP

#include "core/StoragePolicy.hpp"
#include <cstddef>

namespace Core
{
/**
 * @brief Base class for all matrix-like expressions.
 */
template <typename E> struct MatrixExpression
{
private:
  friend E;
  MatrixExpression () = default;

public:
  [[nodiscard]] auto
  rows () const -> std::size_t
  {
    return static_cast<const E &> (*this).rows ();
  }

  [[nodiscard]] auto
  cols () const -> std::size_t
  {
    return static_cast<const E &> (*this).cols ();
  }

  auto
  operator() (std::size_t row, std::size_t col) const
  {
    return static_cast<const E &> (*this) (row, col);
  }

protected:
  ~MatrixExpression () = default;
};

/**
 * @brief Proxy for Addition: R = A + B
 * It stores references to the operands and calculates only when indexed.
 */
template <typename L, typename R>
class MatrixSum : public MatrixExpression<MatrixSum<L, R> >
{
public:
  MatrixSum (const L &lhs, const R &rhs) : m_lhs_ (lhs), m_rhs_ (rhs) {}

  [[nodiscard]] auto
  rows () const -> std::size_t
  {
    return m_lhs_.rows ();
  }

  [[nodiscard]] auto
  cols () const -> std::size_t
  {
    return m_lhs_.cols ();
  }
  auto
  operator() (std::size_t row, std::size_t col) const
  {
    return m_lhs_ (StoragePolicy::Row{ row }, StoragePolicy::Column{ col })
           + m_rhs_ (StoragePolicy::Row{ row }, StoragePolicy::Column{ col });
  }

private:
  const L &m_lhs_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
  const R &m_rhs_; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
};

/**
 * @brief Overload + for any two MatrixExpressions.
 */
template <typename L, typename R>
auto
operator+ (const MatrixExpression<L> &lhs, const MatrixExpression<R> &rhs)
    -> MatrixSum<L, R>
{
  return MatrixSum<L, R> (static_cast<const L &> (lhs),
                          static_cast<const R &> (rhs));
}
} // namespace Core

#endif // EXPRESSION_TEMPLATES_HPP
