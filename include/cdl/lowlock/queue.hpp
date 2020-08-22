// queue.hpp
//
// A multi-producer multi-consumer concurrent queue
// designed to maximize concurrent throughput.
//
// Adapted from the concurrent queue described by Herb Sutter 
// in his article "Writing a Generalized Concurrent Queue".

#ifndef CDL_LOWLOCK_QUEUE_HPP
#define CDL_LOWLOCK_QUEUE_HPP

#include <atomic>
#include <memory>
#include <optional>
#include <functional>
#include <type_traits>

#include "detail/cache_pad.hpp"

namespace cdl::lowlock
{
    namespace detail
    {
        template <typename T>
        struct node
        {
            static auto const LINE_SIZE{detail::cache_line::size};

            T*                 value;
            std::atomic<node*> next;
            
            detail::cache_pad<LINE_SIZE 
                - sizeof(T*) 
                - sizeof(std::atomic<node*>)> pad;

            node() 
                : value{nullptr}
                , next{nullptr} {}

            node(T* value) 
                : value{value}
                , next{nullptr} {}
        };
    }

    template <typename T>
    class queue
    {
        static auto const LINE_SIZE{detail::cache_line::size};

        detail::cache_pad<LINE_SIZE>    pad0;

        detail::node<T>*                head;
        detail::cache_pad<LINE_SIZE 
            - sizeof(detail::node<T>)>  pad1;

        std::atomic_bool                consumer_lock;
        detail::cache_pad<LINE_SIZE 
            - sizeof(std::atomic_bool)> pad2;

        detail::node<T>*                tail;
        detail::cache_pad<LINE_SIZE 
            - sizeof(detail::node<T>)>  pad3;

        std::atomic_bool                producer_lock;
        detail::cache_pad<LINE_SIZE 
            - sizeof(std::atomic_bool)> pad4;

    public:
        queue() 
            : consumer_lock{false}
            , producer_lock{false}
        {
            auto* sentinel = new detail::node<T>{};
            head = sentinel;
            tail = sentinel;
        }

        ~queue()
        {
            while (head != nullptr)
            {
                detail::node<T>* tmp = head;
                head = tmp->next;
                delete tmp;
            }
        }

        auto push(T* value) -> void
        {
            detail::node<T>* new_node 
                = new detail::node<T>{value};

            // acquire exclusivity
            while (std::atomic_exchange(&producer_lock, true)) {}

            tail->next = new_node;  // publish to consumers
            tail = new_node;        // swing `last` forward

            // release exclusivity
            std::atomic_store(&producer_lock, false);
        }

        auto try_pop() -> std::optional<T*>
        {   
            // acquire exclusivity
            while (std::atomic_exchange(&consumer_lock, true)) {}

            detail::node<T>* the_first = head;
            detail::node<T>* the_next = head->next;

            if (the_next != nullptr)
            {
                // the queue is nonempty
                T* value = the_next->value;
                the_next->value = nullptr;
                head = the_next;
                
                std::atomic_store(&consumer_lock, false);
                
                delete the_first;
                return value;
            }
            else
            {
                // queue is empty
                std::atomic_store(&consumer_lock, false);
                return std::nullopt;
            }
        }

        auto try_pop_if(std::function<bool(T*)>&& predicate) -> std::optional<T*>
        {
            // acquire exclusivity
            while (std::atomic_exchange(&consumer_lock, true)) {}

            detail::node<T>* the_first = head;
            detail::node<T>* the_next = head->next;

            if (the_next != nullptr 
                && predicate(the_next->value))
            {
                // the queue is nonempty
                T* value = the_next->value;
                the_next->value = nullptr;
                head = the_next;
                
                std::atomic_store(&consumer_lock, false);
                
                delete the_first;
                return value;
            }
            else
            {
                // queue is empty or item did not satisfy predicate
                std::atomic_store(&consumer_lock, false);
                return std::nullopt;
            }
        }
    };
}

#endif // CDL_LOWLOCK_QUEUE_HPP