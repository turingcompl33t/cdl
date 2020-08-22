// lockfree/spsc.hpp
//
// A lockfree single-producer single-consumer queue.

#ifndef CDL_LOCKFREE_SPSC_HPP
#define CDL_LOCKFREE_SPSC_HPP

#include <atomic>
#include <cassert>
#include <cstddef>
#include <type_traits>

#include "detail/util.hpp"
#include "detail/cache_pad.hpp"

namespace cdl::lockfree
{
    template <typename T>
    class spsc
    {
        using atomic_index = std::atomic<std::size_t>;

        constexpr static auto const interference_size = detail::cache_line::size;

        detail::cache_pad<interference_size> pad0;

        // The fixed, maximum capacity of the queue.
        std::size_t const size;

        // The buffer of records that composes the queue.
        T* records;

        detail::cache_pad<
            interference_size 
            - sizeof(std::size_t) 
            - sizeof(T*)> pad1;
        atomic_index read_index;

        detail::cache_pad<
            interference_size 
            - sizeof(atomic_index)> pad2;
        atomic_index write_index;

    public:
        constexpr explicit spsc(std::size_t const capacity_)
            : size{detail::next_power_of_two(capacity_)}
            , read_index{0}
            , write_index{0}
        {
            assert(size > 0);
            records = new T[size];
        }

        ~spsc()
        {
            if (!std::is_trivially_destructible_v<T>)
            {
                auto current_idx   = read_index.load(std::memory_order_relaxed);
                auto const end_idx = write_index.load(std::memory_order_relaxed);

                while (current_idx != end_idx)
                {
                    records[current_idx].~T();
                    current_idx = (current_idx + 1 == size) 
                        ? 0 
                        : current_idx + 1;
                }
            }

            delete[] records;
        }

        spsc(spsc const&)            = delete;
        spsc& operator=(spsc const&) = delete;

        spsc(spsc&&)            = delete;
        spsc& operator=(spsc&&) = delete;

        // read the record at the head of the queue (destructive read)
        bool read(T& record)
        {
            auto const read_idx = read_index.load(std::memory_order_relaxed);
            if (read_idx == write_index.load(std::memory_order_acquire))
            {
                // queue is empty
                return false;
            }

            auto const next_read = (read_idx + 1 == size) 
                ? 0 
                : read_idx + 1;

            record = std::move(records[read_idx]);
            records[read_index].~T();

            read_index.store(next_read, std::memory_order_release);

            return true;
        }

        // write a new record at the tail of the queue
        template <typename... Args>
        bool write(Args... args)
        {
            auto const write_idx = write_index.load(std::memory_order_relaxed);
            auto const next_write = (write_idx + 1 == size) 
                ? 0 
                : write_idx + 1;

            if (next_write == read_index.load(std::memory_order_acquire))
            {
                // queue is full
                return false;
            }

            new (&records[write_idx]) T(std::forward<Args>(args)...);
            write_index.store(next_write, std::memory_order_release);

            return true;
        }

        // return the number of records that may be stored in the queue
        std::size_t const capacity() const noexcept
        {
            return size - 1;
        }

        bool is_empty_unsafe() const noexcept
        {
            return read_index.load(std::memory_order_relaxed) 
                == write_index.load(std::memory_order_relaxed);
        }
    };
}

#endif // CDL_LOCKFREE_SPSC_HPP