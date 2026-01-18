#ifndef MATRIX_HPP
#define MATRIX_HPP

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
template <MatrixElement T, typename Alloc = std::allocator<T> > class Matrix
{
public:
  using value_type = T;
  using allocator_type = Alloc;
  using size_type = std::size_t;

  // --- Constructors ---

  Matrix (size_type rows, size_type cols, const T &initial = T (),
          const Alloc &alloc = Alloc ())
      : m_rows (rows), m_cols (cols), m_data (rows * cols, initial, alloc)
  {
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

private:
  size_type m_rows;
  size_type m_cols;
  std::vector<T, Alloc> m_data;
};

#endif // MATRIX_HPP
