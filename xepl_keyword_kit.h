// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_keyword_kit.h
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

#include <algorithm>

namespace KITS::KEYWORDS
{
	void Keyword_Axons      ( XEPL::Cortex* _cortex );
	void Keyword_Forms      ( XEPL::Cortex* _cortex );
	void Keyword_Genes      ( XEPL::Cortex* _cortex );
	void Keyword_Lobe       ( XEPL::Cortex* _cortex );
	void Keyword_Macros     ( XEPL::Cortex* _cortex );
	void Keyword_Methods    ( XEPL::Cortex* _cortex );
	void Keyword_Neuron     ( XEPL::Cortex* _cortex );
	void Keyword_Properties ( XEPL::Cortex* _cortex );
	void Keyword_Synapses   ( XEPL::Cortex* _cortex );

	void Keyword_Index      ( XEPL::Cortex* _cortex );
	void Keyword_New        ( XEPL::Cortex* _cortex );
	void Keyword_Using      ( XEPL::Cortex* _cortex );

	void Keyword_Modify  ( XEPL::Cortex* _cortex );
	void Keyword_Print   ( XEPL::Cortex* _cortex );
	void Keyword_Trigger ( XEPL::Cortex* _cortex );

	void Keyword_ForEach ( XEPL::Cortex* _cortex );
	void Keyword_IfNo    ( XEPL::Cortex* _cortex );
	void Keyword_IfYes   ( XEPL::Cortex* _cortex );
	void Keyword_Repeat  ( XEPL::Cortex* _cortex );
	void Keyword_Run     ( XEPL::Cortex* _cortex );
	void Keyword_When    ( XEPL::Cortex* _cortex );

	void Register_Keyword_Kit( XEPL::Cortex* _cortex );
}

