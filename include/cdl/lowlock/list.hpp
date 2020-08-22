// lowlock/list.hpp
// 
// A concurrent intrusive list for use in low-contention situation.

#ifndef CDL_LOWLOCK_LIST_HPP
#define CDL_LOWLOCK_LIST_HPP

#include <cstddef>
#include <optional>
#include <functional>

#include "detail/spinlock.hpp"

namespace cdl::lowlock
{
    struct list_entry
    {
        list_entry* flink;
        list_entry* blink;

        list_entry() 
            : flink{nullptr}
            , blink{nullptr} {}
    };

    class list
    {
        list_entry head;

        detail::spinlock lock;

        std::size_t count;

    public:
        list() 
            : head{}
            , lock{}
            , count{}
        {
            head.flink = &head;
            head.blink = &head;
        }

        static auto push_front(list& l, list_entry& entry) -> void
        {
            l.lock.lock();

            if (0 == l.count)
            {
                l.head.blink = &entry;
            }

            entry.flink = l.head.flink;
            entry.blink = &l.head;

            l.head.flink->blink = &entry;
            l.head.flink = &entry;

            l.lock.unlock();
        }

        static auto push_back(list& l, list_entry& entry) -> void
        {
            l.lock.lock();

            if (0 == l.count++)
            {
                l.head.flink = &entry;
            }

            entry.flink = &l.head;
            entry.blink = l.head.blink;

            l.head.blink->flink = &entry;
            l.head.blink = &entry;

            l.lock.unlock();
        }

        static auto pop_front(list& l) -> std::optional<list_entry*>
        {
            l.lock.lock();

            if (0 == l.count)
            {
                l.lock.unlock();
                return std::nullopt;
            }

            list_entry* popped = l.head.flink;
            
            popped->flink->blink = popped->blink;
            popped->blink->flink = popped->flink;

            l.count--;

            l.lock.unlock();

            return popped;
        }

        static auto pop_back(list& l) -> std::optional<list_entry*>
        {
            l.lock.lock();

            if (0 == l.count)
            {
                l.lock.unlock();
                return std::nullopt;
            }

            list_entry* popped = l.head.blink;
            
            popped->flink->blink = popped->blink;
            popped->blink->flink = popped->flink;
            
            l.count--;

            l.lock.unlock();

            return popped;
        }

        static auto remove_entry(list& l, list_entry* entry) -> void
        {
            l.lock.lock();

            entry->flink->blink = entry->blink;
            entry->blink->flink = entry->flink;

            l.lock.unlock();
        }

        auto iterate(std::function<void(list_entry*)> fn) -> void
        {
            lock.lock();

            list_entry* current = head.flink;
            while (current != &head)
            {
                fn(current);
                current = current->flink;
            }

            lock.unlock();
        }

        auto iterate_while(
            std::function<void(list_entry*)> fn, 
            std::function<bool(list_entry*)> pred) -> bool
        {
            bool broken{false};

            lock.lock();

            list_entry* current = head.flink;
            while (current != &head)
            {
                if (pred(current))
                {
                    broken = true;
                    break;
                }

                fn(current);

                current = current->flink;
            }

            lock.unlock();

            return broken;
        }
    };
}

#endif // CDL_LOWLOCK_LIST_HPP