#include "core/Matrix.hpp"
#include "core/StackAllocator.hpp"
#include <gtest/gtest.h>

using namespace testing;
using namespace Core;
using namespace Core::StoragePolicy;

// --- Suite 4: Custom Allocator ---

TEST (MatrixAllocatorTest, StackAllocatorIntegration)
{
  // Define the specific type we want to use
  using CustomAllocator = StackAllocator<int, 1024>;
  using FastMatrix = Matrix<int, Dense, CustomAllocator>;

  // Pre-allocate the pool
  CustomAllocator pool;

  // Create matrix using the explicit FastMatrix type
  FastMatrix fastMat (Row{ 5 }, Column{ 5 }, 7, pool);

  EXPECT_EQ (fastMat (Row{ 0 }, Column{ 0 }), 7);
  fastMat.insert (Row{ 2 }, Column{ 2 }, 100);
  EXPECT_EQ (fastMat (Row{ 0 }, Column{ 0 }), 7);
}
