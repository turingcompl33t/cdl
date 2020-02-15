// sender.hpp
// Class Implementation: cdl::messaging::sender

#include "message_queue.hpp"

namespace cdl::messaging
{
	class sender
	{
		message_queue* queue_;

	public:

		sender() : queue_{ nullptr }
		{}

		explicit sender(message_queue* queue)
			: queue_{ queue }
		{}

		template <typename ContentType>
		void send(ContentType const& msg)
		{
			if (queue_)
			{
				queue_->push(msg);
			}
		}
	};
}