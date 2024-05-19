// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_file_kit.h
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

namespace KITS::FILES
{
	bool String_Into_File ( XEPL::Cord* _name_cord, XEPL::Cord* _contents_cord );
	bool File_Load_String ( XEPL::Cord* _cord, std::string& std_output_string );
	bool File_Load_String ( XEPL::Cord* _cord, XEPL::String* _string );
	bool File_Load_Gene   ( XEPL::Cord* _filename_cord, XEPL::Gene** _gene );

	void Keyword_FileWrite ( XEPL::Cortex* _cortex );
	void Keyword_FileRead  ( XEPL::Cortex* _cortex );
	void Keyword_Include   ( XEPL::Cortex* _cortex );
	void Keyword_Command   ( XEPL::Cortex* _cortex );

	void Register_File_Kit( XEPL::Cortex* _cortex );
}
