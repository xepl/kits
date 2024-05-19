// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_opereator_kit.h
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

namespace KITS::OPERATORS
{
	void Operator_After     ( XEPL::Cortex* _cortex );
	void Operator_AfterAny  ( XEPL::Cortex* _cortex );
	void Operator_AfterLast ( XEPL::Cortex* _cortex );
	void Operator_Before    ( XEPL::Cortex* _cortex );
	void Operator_BeforeAny ( XEPL::Cortex* _cortex );
	void Operator_NextAny   ( XEPL::Cortex* _cortex );

	void Operator_Append ( XEPL::Cortex* _cortex );
	void Operator_Cr     ( XEPL::Cortex* _cortex );
	void Operator_CrLf   ( XEPL::Cortex* _cortex );
	void Operator_Lf     ( XEPL::Cortex* _cortex );
	void Operator_Period ( XEPL::Cortex* _cortex );
	void Operator_Slash  ( XEPL::Cortex* _cortex );
	void Operator_Space  ( XEPL::Cortex* _cortex );
	void Operator_Tab    ( XEPL::Cortex* _cortex );

	void Operator_Empty ( XEPL::Cortex* _cortex );
	void Operator_Has   ( XEPL::Cortex* _cortex );
	void Operator_Is    ( XEPL::Cortex* _cortex );

	void Operator_Add      ( XEPL::Cortex* _cortex );
	void Operator_Multiply ( XEPL::Cortex* _cortex );
	void Operator_Subtract ( XEPL::Cortex* _cortex );
 
	void Operator_Equal ( XEPL::Cortex* _cortex );
	void Operator_Gt    ( XEPL::Cortex* _cortex );
	void Operator_Lt    ( XEPL::Cortex* _cortex );

	void Operator_DeAmp      ( XEPL::Cortex* _cortex );
	void Operator_Depercent  ( XEPL::Cortex* _cortex );
	void Operator_Lowercase  ( XEPL::Cortex* _cortex );
	void Operator_Percentify ( XEPL::Cortex* _cortex );

	void Register_Operator_Kit( XEPL::Cortex* _cortex );
}
