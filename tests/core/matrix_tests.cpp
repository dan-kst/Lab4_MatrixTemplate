#include "core/Matrix.hpp"
#include <gtest/gtest.h>

using namespace testing;
using namespace Core;
using namespace Core::StoragePolicy;

// --- Suite 1: Dense Storage Logic ---

TEST (MatrixDenseTest, InsertionAndAccess)
{
  Matrix<int> mat (Row{ 3 }, Column{ 3 }, 0);
  mat.insert (Row{ 1 }, Column{ 1 }, 42);
  EXPECT_EQ (mat (Row{ 1 }, Column{ 1 }), 42);
  EXPECT_EQ (mat (Row{ 0 }, Column{ 0 }), 0);
}

TEST (MatrixDenseTest, BoundsChecking)
{
  Matrix<double> mat (Row{ 2 }, Column{ 2 });
  EXPECT_THROW (mat.insert (Row{ 5 }, Column{ 5 }, 1.0), std::out_of_range);
}

TEST (MatrixDenseTest, OrderChecking)
{
  Matrix<int> mat (Row{ 1 }, Column{ 5 }, 0);
  for (int i = 0; i < 5; ++i)
    mat.insert (Row{ 0 }, Column{ static_cast<std::size_t> (i) },
                i); // 0, 1, 2, 3, 4

  EXPECT_TRUE (mat.isOrdered (0, true, true));   // Ascending
  EXPECT_FALSE (mat.isOrdered (0, true, false)); // Descending

  mat.insert (Row{ 0 }, Column{ 2 }, 0); // 0, 1, 0, 3, 4 - breaks order
  EXPECT_FALSE (mat.isOrdered (0, true, true));
}

// --- Suite 2: Sparse Storage Logic ---

TEST (MatrixSparseTest, SparseStorageEfficiency)
{
  // We use a large virtual size but only a few actual elements
  Matrix<int, Sparse> sparseMat (Row{ 1000 }, Column{ 1000 }, 0);
  sparseMat.insert (Row{ 500 }, Column{ 500 }, 99);

  EXPECT_EQ (sparseMat (Row{ 500 }, Column{ 500 }), 99);
  EXPECT_EQ (sparseMat (Row{ 0 }, Column{ 0 }), 0);
}
