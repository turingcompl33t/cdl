// template_dispatcher.hpp
// Class Implementation: cdl::messaging::template_dispatcher

#include "message_queue.hpp"

namespace cdl::messaging
{
	template <
		typename PreviousDispatcher,
		typename Message,
		typename FunctionType>
	class template_dispatcher
	{
		message_queue*      queue_;
		PreviousDispatcher* prev_;
		FunctionType        fn_;
		bool                chained_;

		template <
			typename Dispatcher,
			typename OtherMessage,
			typename OtherFunctionType>
		friend class template_dispatcher;

		void wait_and_dispatch()
		{
			for (;;)
			{
				auto msg = queue_->wait_and_pop();
				if (dispatch(msg))
				{
					break;
				}
			}
		}

		bool dispatch(std::shared_ptr<message_base> const& msg)
		{
			if (wrapped_message<Message>* wrapper =
				dynamic_cast<wrapped_message<Message>*>(msg.get()))
			{
				fn_(wrapper->content_);
				return true;
			}
			else
			{
				// chain to the previous dispatcher
				return prev_->dispatch(msg);
			}
		}

	public:

		template_dispatcher(
			message_queue* queue, 
			PreviousDispatcher* prev, 
			FunctionType&& fn
			) : 
			queue_{ queue },
			prev_{ prev },
			fn_{ std::move(fn) },
			chained_{ false }
		{
			prev_->chained = true;
		}

		template_dispatcher(template_dispatcher&& other)
			: queue_{other.queue_},
			prev_{other.prev_},
			fn_{ std::move(other.fn_) },
			chained_{ other.chained_ }
		{
			other_.chained = true;
		}

		template <typename OtherMessage, typename OtherFunctionType>
		template_dispatcher<template_dispatcher, OtherMessage, OtherFunctionType>
		handle(OtherFunctionType&& ofn)
		{
			return template_dispatcher<
				template_dispatcher,
				OtherMessage,
				OtherFunctionType>(queue_, this, std::forward<OtherFunctionType>(ofn));
		}

		~template_dispatcher() noexcept(false)
		{
			if (!chained_)
			{
				wait_and_dispatch();
			}
		}

		template_dispatcher(template_dispatcher const&) = delete;
		template_dispatcher& operator=(template_dispatcher const&) = delete;
	};

}