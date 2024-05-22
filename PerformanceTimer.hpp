// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_cli_kit.h
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

#include "PerformanceTimer.h"

std::string KITS::TIMER::formatMilliseconds(long long _milliseconds )
{
	int hours   = static_cast<int>( _milliseconds / (1000 * 60   * 60));
	int minutes = static_cast<int>((_milliseconds / (1000 * 60)) % 60);
	int seconds = static_cast<int>((_milliseconds / 1000) % 60);
	int ms      = static_cast<int>( _milliseconds % 1000);

	std::ostringstream oss;
	oss << std::setfill('0')
		<< std::setw(2) << hours   << ":"
		<< std::setw(2) << minutes << ":"
		<< std::setw(2) << seconds << "."
		<< std::setw(5) << ms;

	return oss.str();
}

KITS::TIMER::PerformanceTimer::PerformanceTimer( XEPL::Neuron*, XEPL::Gene* )
	: start_clock ( std::chrono::high_resolution_clock::now()),
	  start_time  ( std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()))
{}

std::string KITS::TIMER::PerformanceTimer::Time()
{
	auto end_clock = std::chrono::high_resolution_clock::now();
	auto duration  = std::chrono::duration_cast<std::chrono::milliseconds>(end_clock - start_clock);

	return formatMilliseconds(duration.count());
}

KITS::TIMER::PerformanceTimer::~PerformanceTimer()
{
	std::cout << "Elapsed time: " << Time() << std::endl;
}

void KITS::TIMER::Keyword_Time ( XEPL::Cortex* _cortex )
{
	_cortex->Register_Keyword ( "Time", [] ( XEPL::Neuron* _neuron, XEPL::Gene* _call_gene, XEPL::String* _str )
	{
		XEPL::Script ( _neuron, _call_gene );

		auto pt = new PerformanceTimer ( _neuron, _call_gene );
	
		_neuron->Process_Inner_Genes ( _call_gene );

		_neuron->observer->Trait_Set("time", pt->Time().c_str() );

		delete pt;

	} );
}

void KITS::TIMER::Register_Performance_Kit( XEPL::Cortex* _cortex )
{
	Keyword_Time ( _cortex );
}

