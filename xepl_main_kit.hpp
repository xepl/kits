// SPDX: AGPL-3.0-only
/* XEPL Solo Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:     xepl_main_kit.hpp
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

#include "xepl_main_kit.h"

void KITS::MAIN::Command_Main( XEPL::String* _opt )
{
	main(  argc, argv, arge );
}
void KITS::MAIN::Register_Main_Kit( XEPL::Cortex* _cortex )
{
	_cortex->Register_Command( "Main",  [] ( XEPL::String* _opt ) {
		KITS::MAIN::Command_Main( _opt );
	} );
}
void KITS::MAIN::Initialize( int _argc, char** _argv, char** _arge )
{
	argc = _argc;
	argv = _argv;
	arge = _arge;
}

// Just dump out the parameters
int KITS::MAIN::main(int argc, char* argv[], char* arge[]) 
{
    for (int i = 0; i < argc; i++) 
		std::cout << argv[i] << ' ';
	std::cout << std::endl;
    return 0;
}

