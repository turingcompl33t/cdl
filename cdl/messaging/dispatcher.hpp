// dispatcher.hpp
// Class Implementation: cdl::messaging::dispatcher

#include "message_queue.hpp"

namespace cdl::messaging
{
	// special message for closing a queue
	class close_queue {};

	class dispatcher
	{
		message_queue* queue_;
		bool chained_;

		template<
			typename Dispatcher,
			typename Message,
			typename FunctionType>
		friend class template_dispatcher;

	public:

		explicit dispatcher(message_queue* queue)
			: queue_{ queue }, chained_{ false }
		{}

		dispatcher(dispatcher&& other)
			: queue_{ other.queue_ },
			chained_{ other.chained_ }
		{
			other.chained_ = true;
		}

		template <typename Message, typename FunctionType>
		template_dispatcher<dispatcher, Message, FunctionType>
		handle(FunctionType&& fn)
		{
			return template_dispatcher<dispatcher, Message, FunctionType>(
				queue_, 
				this, 
				std::forward<FunctionType>(fn)
				);
		}

		~dispatcher() noexcept(false)
		{
			if (!chained_)
			{
				wait_and_dispatch();
			}
		}

		// disable copies
		dispatcher(dispatcher const&) = delete;
		dispatcher& operator=(dispatcher const&) = delete;

	private:

		void wait_and_dispatch()
		{
			for (;;)
			{
				auto msg = queue_->wait_and_pop();
				dispatch(msg);
			}
		}

		bool dispatch(std::shared_ptr<message_base> const& msg)
		{
			if (dynamic_cast<wrapped_message<close_queue>*>(msg.get()))
			{
				throw close_queue{};
			}

			return false;
		}
	};
}