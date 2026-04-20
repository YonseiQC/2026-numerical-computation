#include <cstddef>
#include <cstdlib>
#include <stdexcept>
#include <bit>
#include <cassert>

template <class T, size_t alignment_> class AlignedAllocator {
  private:
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-const-or-ref-data-members)
    constexpr static size_t alignment = alignment_;
    static_assert(std::popcount(alignment_) == 1);

  public:
    using value_type = T;

    template <class U>
    struct rebind {
        using other = AlignedAllocator<U, alignment_>;
    };

    /**
     * @brief Constructor of AlignedAllocator class
     *
     * @param alignment Memory alignment we want.
     */
    constexpr explicit AlignedAllocator() {
    }

    template <typename U>
    explicit constexpr AlignedAllocator(
        [[maybe_unused]] const AlignedAllocator<U, alignment_> &rhs) noexcept {}

    /**
     * @brief Allocate memory with for the given number of datatype T
     *
     * @param size The number of T objects for the allocation
     * @return Allocated aligned memory
     */
    [[nodiscard]] T *allocate(std::size_t size) const {
        if (size == 0) {
            return nullptr;
        }
        void *p = nullptr;
        if constexpr (alignment_ > alignof(std::max_align_t)) {
            p = std::aligned_alloc(alignment_, sizeof(T) * size);
        } else {
            // NOLINTNEXTLINE(hicpp-no-malloc)
            p = std::malloc(sizeof(T) * size);
        }
        if (p == nullptr) {
            throw std::bad_alloc();
        }
        return static_cast<T *>(p);
    }

    /**
     * @brief Deallocate allocated memory
     *
     * @param p Pointer to the allocated data
     * @param size Size of the data we allocated (unused).
     */
    void deallocate(T *p, [[maybe_unused]] std::size_t size) const noexcept {
		std::free(p);
    }

    template <class U> void construct(U *ptr) { ::new ((void *)ptr) U(); }

    template <class U> void destroy(U *ptr) {
        (void)ptr;
        ptr->~U();
    }
};
