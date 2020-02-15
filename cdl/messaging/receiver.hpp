// receiver.hpp
// Class Implementation: cdl::messaging::receiver

#include "sender.hpp"
#include "message_queue.hpp"

namespace cdl::messaging
{
	class receiver
	{
		// a receiver owns a message queue
		message_queue queue_;

	public:

		// allow implicit conversion to a sender that references owned queue
		operator sender()
		{
			return sender(&queue_);
		}

		dispatcher wait()
		{
			// waiting for a queue creates a dispatcher
			return dispatcher(&queue_);
		}
	};
}