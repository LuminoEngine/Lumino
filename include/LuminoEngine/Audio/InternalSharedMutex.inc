#pragma once

#include <mutex>
#include "Common.hpp"

namespace ln {
namespace detail {

#ifdef LN_AUDIO_THREAD_ENABLED

// When std::shared_mutex can not be used
// https://www.boost.org/doc/libs/1_61_0/boost/thread/pthread/shared_mutex.hpp
class internal_shared_mutex
{
private:
	class state_data
	{
	public:
		state_data() :
			shared_count(0),
			exclusive(false),
			upgrade(false),
			exclusive_waiting_blocked(false)
		{}

		void assert_free() const
		{
			LN_CHECK(!exclusive);
			LN_CHECK(!upgrade);
			LN_CHECK(shared_count == 0);
		}

		void assert_locked() const
		{
			LN_CHECK(exclusive);
			LN_CHECK(shared_count == 0);
			LN_CHECK(!upgrade);
		}

		void assert_lock_shared() const
		{
			LN_CHECK(!exclusive);
			LN_CHECK(shared_count > 0);
		}

		void assert_lock_upgraded() const
		{
			LN_CHECK(!exclusive);
			LN_CHECK(upgrade);
			LN_CHECK(shared_count > 0);
		}

		void assert_lock_not_upgraded() const
		{
			LN_CHECK(!upgrade);
		}

		bool can_lock() const
		{
			return !(shared_count || exclusive);
		}

		void exclusive_blocked(bool blocked)
		{
			exclusive_waiting_blocked = blocked;
		}

		void lock()
		{
			exclusive = true;
		}

		void unlock()
		{
			exclusive = false;
			exclusive_waiting_blocked = false;
		}

		bool can_lock_shared() const
		{
			return !(exclusive || exclusive_waiting_blocked);
		}

		bool more_shared() const
		{
			return shared_count > 0;
		}

		unsigned get_shared_count() const
		{
			return shared_count;
		}

		unsigned  lock_shared()
		{
			return ++shared_count;
		}


		void unlock_shared()
		{
			--shared_count;
		}

		bool unlock_shared_downgrades()
		{
			if (upgrade) {
				upgrade = false;
				exclusive = true;
				return true;
			}
			else {
				exclusive_waiting_blocked = false;
				return false;
			}
		}

		void lock_upgrade()
		{
			++shared_count;
			upgrade = true;
		}

		bool can_lock_upgrade() const
		{
			return !(exclusive || exclusive_waiting_blocked || upgrade);
		}

		void unlock_upgrade()
		{
			upgrade = false;
			--shared_count;
		}

		//private:
		unsigned shared_count;
		bool exclusive;
		bool upgrade;
		bool exclusive_waiting_blocked;
	};



	state_data state;
	std::mutex state_change;
	std::condition_variable shared_cond;
	std::condition_variable exclusive_cond;
	std::condition_variable upgrade_cond;

	void release_waiters()
	{
		exclusive_cond.notify_one();
		shared_cond.notify_all();
	}

public:
	LN_DISALLOW_COPY_AND_ASSIGN(internal_shared_mutex);

	internal_shared_mutex()
	{
	}

	~internal_shared_mutex()
	{
	}

	void lock_shared()
	{
#if defined BOOST_THREAD_PROVIDES_INTERRUPTIONS
		boost::this_thread::disable_interruption do_not_disturb;
#endif
		std::unique_lock<std::mutex> lk(state_change);
		while (!state.can_lock_shared())
		{
			shared_cond.wait(lk);
		}
		state.lock_shared();
	}

	void unlock_shared()
	{
		std::unique_lock<std::mutex> lk(state_change);
		state.assert_lock_shared();
		state.unlock_shared();
		if (!state.more_shared())
		{
			if (state.upgrade)
			{
				// As there is a thread doing a unlock_upgrade_and_lock that is waiting for ! state.more_shared()
				// avoid other threads to lock, lock_upgrade or lock_shared, so only this thread is notified.
				state.upgrade = false;
				state.exclusive = true;
				lk.unlock();
				upgrade_cond.notify_one();
			}
			else
			{
				state.exclusive_waiting_blocked = false;
				lk.unlock();
			}
			release_waiters();
		}
	}

	void lock()
	{
#if defined BOOST_THREAD_PROVIDES_INTERRUPTIONS
		boost::this_thread::disable_interruption do_not_disturb;
#endif
		std::unique_lock<std::mutex> lk(state_change);

		while (state.shared_count || state.exclusive)
		{
			state.exclusive_waiting_blocked = true;
			exclusive_cond.wait(lk);
		}
		state.exclusive = true;
	}

	void unlock()
	{
		std::unique_lock<std::mutex> lk(state_change);
		state.assert_locked();
		state.exclusive = false;
		state.exclusive_waiting_blocked = false;
		state.assert_free();
		release_waiters();
	}
};
#else

class internal_shared_mutex
{
public:
	LN_DISALLOW_COPY_AND_ASSIGN(internal_shared_mutex);

	internal_shared_mutex()
	{
	}

	~internal_shared_mutex()
	{
	}

	void lock_shared()
	{
	}

	void unlock_shared()
	{
	}

	void lock()
	{
	}

	void unlock()
	{
	}
};

#endif


} // namespace detail
} // namespace ln

