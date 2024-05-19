// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_timer_kit.hpp
	Author:       Keith Edwin Robbins
	Release date: May 10, 2024
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

#include "xepl_timer_kit.h"

KITS::TIMER::TickerThread::~TickerThread()
{
	host_lobe->Lobe_Dying();
	Stop();
}

KITS::TIMER::TickerThread::TickerThread ( std::chrono::milliseconds _interval )
	: interval     ( _interval )
	, tick_count   ( 0 )
	, host_lobe    ( new XEPL::Lobe( "ticker" ) )
	, is_running   ( false )
	, shutdown_semaphore( nullptr )
{}

void KITS::TIMER::TickerThread::Start()
{
	if ( is_running )
		return;

	is_running = true;
	tick_count = 0;

	std_thread = std::thread ( [this]()
	{
		XEPL::tlsLobe = host_lobe;
		XEPL::Set_Thread_Name ( nullptr, "InterruptibleTimer" );

		XEPL::Backpack backpack;

		auto next_tick = std::chrono::steady_clock::now() + interval;
		while ( is_running && !shutdown_semaphore)
		{
			std::unique_lock<std::mutex> lock ( std_mutex );
			std_condition_variable.wait_until ( lock, next_tick, [this]()
			{
				return !is_running || shutdown_semaphore;
			} );

			if ( is_running && !shutdown_semaphore )
			{
				bool continue_timer = Deliver_Tick ( ++tick_count );

				if ( !continue_timer )
				{
					is_running = false;
					std_thread.detach();
				}
				next_tick += interval;
			}
		}
		if ( shutdown_semaphore )
		{
			std::unique_lock<std::mutex> block ( *shutdown_semaphore );
			if ( is_running )
			{
				std_thread.detach();
				(*shutdown_semaphore).notify_one();
			}
		}
		XEPL::tlsLobe = nullptr;;

	} );
}

void KITS::TIMER::TickerThread::Stop()
{
	is_running = false;
	std_condition_variable.notify_all();

	if ( std_thread.joinable() )
		std_thread.join();
}




KITS::TIMER::TimerList* KITS::TIMER::Timer::masterTimerList = nullptr;

KITS::TIMER::TimerList::~TimerList()
{
	if ( !list_of_timers->empty() )
		Stop();

	delete timer_mutex;
	delete list_of_timers;
}

KITS::TIMER::TimerList::TimerList ( long _duration )
	: TickerThread ( ( std::chrono::milliseconds ) ( _duration ) )
	, list_of_timers ( new ListTimers() )
	, timer_mutex    ( new XEPL::Mutex() )
	, pre_scaler     ( _duration )
{}

bool KITS::TIMER::TimerList::Deliver_Tick ( int64_t )
{
	XEPL::MutexScope lock_timerlist ( timer_mutex );

	if ( list_of_timers->empty() )
		return false;

	if ( --(list_of_timers->front())->offset > 0 )
		return true;

	XEPL::Gene* notification = new XEPL::Gene ( nullptr, "Tick", nullptr );

	auto   it  = list_of_timers->begin();
	while (it != list_of_timers->end() && (*it)->offset == 0)
	{
		(*it)->Expired(notification);
		it = list_of_timers->begin();
	}
	notification->Release();
	return true;
}

void KITS::TIMER::TimerList::Run ( Timer* _timer, long _rate )
{
	_timer->rate  = std::max(_rate / pre_scaler, 1L);

	XEPL::MutexScope lock_timerlist ( timer_mutex );

	long remaining = _timer->rate;
	bool start     = list_of_timers->empty();

	for ( auto it = list_of_timers->begin(); it != list_of_timers->end(); ++it )
	{
		Timer* this_timer = *it;

		if ( remaining < this_timer->offset )
		{
			this_timer->offset -= remaining;
			_timer->offset = remaining;
			_timer->time_is_running = true;
			_timer->Attach();
			list_of_timers->insert(it, _timer);

			if ( start )
				Start();

			return;
		}
		remaining -= this_timer->offset;
	}

	_timer->offset = remaining;
	_timer->time_is_running = true;
	_timer->Attach();
	list_of_timers->push_back(_timer);

	if (start)
		Start();
}

void KITS::TIMER::TimerList::Remove(Timer* _timer)
{
	XEPL::MutexScope lock_timerlist(timer_mutex);

	auto it  = std::find(list_of_timers->begin(), list_of_timers->end(), _timer);
	if ( it != list_of_timers->end())
	{
		auto next_it = std::next(it);

		if ( next_it != list_of_timers->end() )
			(*next_it)->offset += _timer->offset;

		_timer->time_is_running = false;
		_timer->Release();

		list_of_timers->remove(_timer);
	}
}

void KITS::TIMER::TimerList::Requeue ( Timer* _timer )
{
	XEPL::MutexScope lock_timerlist ( timer_mutex );

	long remaining = _timer->rate;

	list_of_timers->remove(_timer);

	for (auto it = list_of_timers->begin(); it != list_of_timers->end(); ++it)
	{
		Timer* this_timer = *it;

		if ( remaining < this_timer->offset )
		{
			this_timer->offset -= remaining;
			_timer->offset      = remaining;

			list_of_timers->insert(it, _timer);

			return;
		}
		remaining -= this_timer->offset;
	}
	_timer->offset = remaining;
	_timer->time_is_running = true;

	list_of_timers->push_back(_timer);
}




KITS::TIMER::Timer::~Timer()
{
	if ( config_gene )
		config_gene->Release();
}

KITS::TIMER::Timer::Timer ( XEPL::Neuron*  _parent, XEPL::Gene* _gene )
	: Neuron              ( _parent, _gene )
	, config_gene         ( _gene )
	, timer_fired         ( nullptr )
	, offset              ( -1 )
	, rate                ( -1 )
	, duration            ( 1000 )
	, time_is_running     ( false )
{
	config_gene->Attach();

	XEPL::String content;
	if ( config_gene->Copy_Content(&content) )
		duration = strtol( content.c_str(), nullptr, 10 );
}

void KITS::TIMER::Timer::Nucleus_Dropped()
{
	if ( masterTimerList )
	{
		XEPL::MutexScope lock_timerlist( masterTimerList->timer_mutex );

		if ( time_is_running )
			Stop_Timer();
	}
	Neuron::Nucleus_Dropped();
}

void KITS::TIMER::Timer::Stop_Timer()
{
	if ( !time_is_running )
		return;

	masterTimerList->Remove ( this );
}

void KITS::TIMER::Timer::Start_Timer ( long _rate )
{
	if ( time_is_running )
		Stop_Timer();

	time_is_running = true;
	long pace = _rate;

	if ( !_rate )
		pace = duration;

	if ( masterTimerList )
		masterTimerList->Run ( this, pace );
	else
		XEPL::ErrorReport( "TIMER not initialized" );
}

void KITS::TIMER::Initialize ( long _rate )
{
	Timer::masterTimerList = new TimerList ( _rate );
}

void KITS::TIMER::Shutdown()
{
	XEPL::Semaphore* semaphore = new XEPL::Semaphore();
	{
		std::unique_lock<std::mutex> block ( *semaphore );
		{
			Timer::masterTimerList->shutdown_semaphore = semaphore;
			if ( Timer::masterTimerList->is_running )
				semaphore->std::condition_variable::wait ( block );
		}
	}
	delete semaphore;
	delete Timer::masterTimerList;
	Timer::masterTimerList = nullptr;
}





KITS::TIMER::SleepTimer::SleepTimer( XEPL::Neuron* _parent, XEPL::Gene* _gene )
: Timer( _parent, _gene )
{
	timer_fired = new XEPL::Axon ( this, "timer_fired" );
	Synapse_Axon ( timer_fired,  ( XEPL::Receiver )&SleepTimer::Handle_Sleep_Fire, nullptr );
	Start_Timer(0);
}

void KITS::TIMER::SleepTimer::Handle_Sleep_Fire ( XEPL::Atom*, XEPL::Atom* )
{
	Process_Inner_Genes ( config_gene );
	Neuron::Nucleus_Dropped();
}

void KITS::TIMER::SleepTimer::Expired ( XEPL::Gene* _signal_gene )
{
	masterTimerList->Remove ( this );
	timer_fired->Trigger( _signal_gene );
}





KITS::TIMER::RepeatTimer::RepeatTimer( XEPL::Neuron* _parent, XEPL::Gene* _gene )
: Timer( _parent, _gene )
{
	timer_fired = new XEPL::Axon ( this, "timer_fired" );
	Synapse_Axon ( timer_fired,  ( XEPL::Receiver )&RepeatTimer::Handle_Repeat_Fire, nullptr );
	Start_Timer(0);
}

void KITS::TIMER::RepeatTimer::Handle_Repeat_Fire ( XEPL::Atom*, XEPL::Atom* )
{
	Process_Inner_Genes ( config_gene );
}

void KITS::TIMER::RepeatTimer::Expired ( XEPL::Gene* _signal_gene )
{
	masterTimerList->Requeue ( this );
	timer_fired->Trigger( _signal_gene );
}




KITS::TIMER::Pulson::PulsarAxon::PulsarAxon( Pulson* _parent, XEPL::Cord* _name, long _skip, long _skew, PulsarAxon* _next )
: XEPL::Axon( _parent, _name )
, next_pulsar( _next )
, skew       ( _skew )
, skip       ( _skip )
{
}

void KITS::TIMER::Pulson::PulsarAxon::Pulse( long _tick, XEPL::Gene* _signal_gene )
{
	if ( ( _tick + skew ) % skip == 0 )
		Trigger( _signal_gene );
}

KITS::TIMER::Pulson::Pulson( XEPL::Neuron* _parent, XEPL::Gene* _gene )
: Timer( _parent, _gene )
, tick( 0 )
{
	if ( _gene->traits )
	{
		XEPL::String the_skew;
		XEPL::String the_skip;

		XEPL::Trait* trait = _gene->traits->first_trait;
		while ( trait )
		{
			if ( trait->trait_name->compare("name") != 0 )
			{
				long skew = 0;
				long skip = 1;

				if( XEPL::Split_ch_lhs_rhs( trait->trait_term, ':', &the_skip, &the_skew ) )
					skew = strtol( the_skew.c_str(), nullptr, 10 );

				skip = strtol( the_skip.c_str(), nullptr, 10 );

				if ( skip )
					head = new PulsarAxon( this, trait->trait_name, skip, skew, head );
			}
			trait = trait->next_trait;
		}
	}
	Process_Inner_Genes( _gene );
	Start_Timer(0);
}

void KITS::TIMER::Pulson::Expired ( XEPL::Gene* _signal_gene )
{
	masterTimerList->Requeue ( this );

	++tick;

	PulsarAxon* pulsar = head;
	while ( pulsar )
	{
		pulsar->Pulse(tick, _signal_gene );
		pulsar = pulsar->next_pulsar;
	}
}



void KITS::TIMER::Keyword_Sleep ( XEPL::Cortex* _cortex )
{
	_cortex->Register_Keyword ( "Sleep", [] ( XEPL::Neuron* _neuron, XEPL::Gene* _call_gene, XEPL::String* )
	{
		XEPL::Script ( _neuron, _call_gene );

		( new SleepTimer ( _neuron, _call_gene ) );
	} );
}

void KITS::TIMER::Keyword_Recur ( XEPL::Cortex* _cortex )
{
	_cortex->Register_Keyword ( "Recur", [] ( XEPL::Neuron* _neuron, XEPL::Gene* _call_gene, XEPL::String* )
	{
		XEPL::Script ( _neuron, _call_gene );

		( new RepeatTimer ( _neuron, _call_gene ) );
	} );
}

void KITS::TIMER::Keyword_Pulson ( XEPL::Cortex* _cortex )
{
	_cortex->Register_Keyword ( "Pulson", [] ( XEPL::Neuron* _neuron, XEPL::Gene* _call_gene, XEPL::String* )
	{
		XEPL::Script ( _neuron, _call_gene );

		( new Pulson ( _neuron, _call_gene ) );
	} );
}

void KITS::TIMER::Register_Timer_Kit( XEPL::Cortex* _cortex )
{
	Keyword_Sleep ( _cortex );
	Keyword_Recur ( _cortex );
	Keyword_Pulson( _cortex );
}

