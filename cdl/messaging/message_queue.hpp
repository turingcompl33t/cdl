// message_queue.hpp
// Class Implementation: cdl::messaging::message_queue.

#include <mutex>
#include <queue>
#include <memory>
#include <condition_variable>

namespace cdl::messaging
{
	struct message_base
	{
		virtual ~message_base() {}
	};

	template <typename ContentType>
	struct wrapped_message : message_base
	{
		ContentType content_;
		explicit wrapped_message(ContentType const& content)
			: content_{ content }
		{}
	};

	class message_queue
	{
		std::mutex mutex_;
		std::condition_variable cv_;
		std::queue<std::shared_ptr<message_base>> queue_;
	
	public:

		template <typename T>
		void push(T const& msg)
		{
			std::lock_guard<std::mutex> guard{ mutex_ };
			queue_.push(std::make_shared<wrapped_message<T>>(msg));
			cv_.notify_all();
		}

		std::shared_ptr<message_base> wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{ mutex_ };
			
			cv_.wait(lock, [&]() { return !queue_.empty(); });

			auto msg = queue_.front();
			queue_.pop();
			return msg;
		}
	};
}