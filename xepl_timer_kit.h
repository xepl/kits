// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_timer_kit.h
	Author:       Keith Edwin Robbins
	Release date: May 20, 2024
	Website:      https://xepl.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published by
	the Free Software Foundation version 3 of the License.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.

	For more information about the AGPL, please visit:
 	https://www.gnu.org/licenses/agpl-3.0.html
*/


namespace KITS::TIMER
{
	void Keyword_Sleep     ( XEPL::Cortex* _cortex );
	void Keyword_Recur     ( XEPL::Cortex* _cortex );
	void Keyword_Pulson    ( XEPL::Cortex* _cortex );
	void Register_Timer_Kit( XEPL::Cortex* _cortex );
}

namespace KITS::TIMER
{
	void Initialize       ( long  );
	void Shutdown         ( void  );

	class TimerList;

	class TickerThread : public XEPL::NoCopy
	{
		std::chrono::milliseconds      interval;
		int64_t                        tick_count;
		std::thread                    std_thread;
		std::mutex                     std_mutex;
		std::condition_variable        std_condition_variable;
	protected:
		virtual ~TickerThread();
		explicit TickerThread ( std::chrono::milliseconds );
		virtual bool Deliver_Tick ( int64_t ) = 0;
	public:
		XEPL::Lobe*                    host_lobe;
		std::atomic<bool>              is_running;
		std::atomic<XEPL::Semaphore*>  shutdown_semaphore;
		void Start ( void );
		void Stop  ( void );
	};


	class Timer : public XEPL::Neuron
	{
		friend class KITS::TIMER::TimerList;

	protected:
		XEPL::Gene*            config_gene;
		XEPL::Axon*            timer_fired;
		long                   offset;
		long                   rate;
		long                   duration;
		bool                   time_is_running;

		virtual ~Timer ( void ) override;
		virtual void Nucleus_Dropped ( void  ) override;
		virtual void Expired         ( XEPL::Gene* ) = 0;

	public:
		static KITS::TIMER::TimerList* masterTimerList;

		explicit Timer ( XEPL::Neuron*, XEPL::Gene* );
		void Start_Timer    ( long  );
		void Stop_Timer     ( void  );
	};


	class SleepTimer : public KITS::TIMER::Timer
	{
	public:
		explicit SleepTimer ( XEPL::Neuron*, XEPL::Gene* );
		void Expired            ( XEPL::Gene* ) override;
		void Handle_Sleep_Fire  ( XEPL::Atom*, XEPL::Atom* );
	};


	class RepeatTimer : public KITS::TIMER::Timer
	{
	public:
		explicit RepeatTimer ( XEPL::Neuron*, XEPL::Gene* );
		void Expired             ( XEPL::Gene* ) override;
		void Handle_Repeat_Fire  ( XEPL::Atom*, XEPL::Atom* );
	};


	class Pulson : public KITS::TIMER::Timer
	{
		long tick;
		class PulsarAxon : public XEPL::Axon
		{
		public:
			PulsarAxon* next_pulsar;
			long  skew;
			long  skip;
		public:
			PulsarAxon( Pulson*, XEPL::Cord*, long _skip, long _skew, PulsarAxon* );
			void Pulse( long, XEPL::Gene* );
		};

		PulsarAxon* head;

	public:
		explicit Pulson     ( XEPL::Neuron*, XEPL::Gene* );
		void Expired        ( XEPL::Gene* ) override;
		void Handle_Pulson  ( XEPL::Atom*, XEPL::Atom* );
	};


	class TimerList : public TickerThread
	{
		friend class Timer;
		using TimerAllocator = XEPL::TAllocatorT<Timer*>;
		using ListTimers     = std::list<Timer*, TimerAllocator>;
	protected:
		ListTimers*   list_of_timers;
		XEPL::Mutex*  timer_mutex;
		long          pre_scaler;
	public:
		virtual ~TimerList() override;
		explicit TimerList ( long );
		virtual bool Deliver_Tick( int64_t tick_count ) override;
		void Run     ( Timer*, long );
		void Remove  ( Timer* );
		void Requeue ( Timer* );
	};
}
