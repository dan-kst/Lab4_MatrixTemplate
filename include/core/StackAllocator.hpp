#ifndef STACK_ALLOCATOR_HPP
#define STACK_ALLOCATOR_HPP

#include <cstddef>
#include <memory>
#include <new>
#include <stdexcept>

/**
 * @brief The actual memory storage (Arena).
 * This owns the memory so the Allocator can be copied safely.
 */
template <typename T, std::size_t SIZE> class MemoryArena
{
public:
  MemoryArena () : m_buffer_ (new T[SIZE]) {}
  ~MemoryArena () { delete[] m_buffer_; }

  // Prevent copying the arena itself
  MemoryArena (const MemoryArena &) = delete;
  auto operator= (const MemoryArena &) -> MemoryArena & = delete;

  auto
  getBuffer () -> T *
  {
    return m_buffer_;
  }
  auto
  getOffset () -> std::size_t &
  {
    return m_offset_;
  }

private:
  T *m_buffer_;
  std::size_t m_offset_{ 0 };
};
/**
 * @brief Stateful Stack Allocator that points to an Arena.
 */
template <typename T, std::size_t SIZE> class StackAllocator
{
public:
  using value_type = T;

  StackAllocator () : m_arena_ (std::make_shared<MemoryArena<T, SIZE> > ()) {}

  // Standard rebind (MUST BE LOWERCASE)
  template <typename U> struct rebind // NOLINT(readability-identifier-naming,-warnings-as-errors)
  {
    using other = StackAllocator<U, SIZE>;
  };

  auto
  allocate (std::size_t n) -> T *
  {
    T *buffer = m_arena_->getBuffer ();
    std::size_t &offset = m_arena_->getOffset ();

    if (offset + n > SIZE)
      {
        throw std::bad_alloc ();
      }

    T *result = &buffer[offset];
    offset += n;
    return result;
  }

  void
  deallocate (T * /*p*/, std::size_t /*n*/) noexcept
  {
  }

  auto
  operator== (const StackAllocator &other) const noexcept->bool
  {
    return m_arena_ == other.m_arena_;
  }
  auto
  operator!= (const StackAllocator &other) const noexcept->bool
  {
    return !(*this == other);
  }

private:
  std::shared_ptr<MemoryArena<T, SIZE> > m_arena_;
};

#endif // STACK_ALLOCATOR_HPP
