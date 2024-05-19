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

#include "../one/xepl.h"

namespace KITS::CLI
{
	bool Execute_As_Xml    ( XEPL::Text* _chars );
	bool Execute_As_Rna    ( XEPL::Text* _chars );
	bool Execute_From_File ( XEPL::Text* _chars );
	bool Execute_Command   ( XEPL::Text* _chars );

	bool CliLoop ( std::istream& _input_stream, std::ostream& _output_stream, std::ostream& _error_stream );
}
