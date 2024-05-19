// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_html_kit.h
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

namespace KITS::HTML
{
	void Scribble ( XEPL::Nucleus* _nucleus, XEPL::Gene* _gene, XEPL::Rendon* _rendon );
	void Output   ( XEPL::Nucleus* _nucleus, XEPL::Gene* _gene, XEPL::Rendon* _rendon );

	void Keyword_Html ( XEPL::Cortex* _cortex );

	void Register_Html_Kit( XEPL::Cortex* _cortex );
}

namespace KITS::HTML
{
	class RendonHtml : public XEPL::Rendon
	{
	public:
		explicit RendonHtml        ( XEPL::Neuron*,  XEPL::Gene*, XEPL::String* );
		virtual void Rendon_Markup ( XEPL::Gene*,    XEPL::Gene* ) override;
		virtual void Rendon_Render ( XEPL::Nucleus*, XEPL::Gene* ) override;
		void Scribble_Traits       ( XEPL::Nucleus*, XEPL::Gene* );
		void Scribble_Content      ( XEPL::Nucleus*, XEPL::Gene* );
		void Scribble_Gene         ( XEPL::Nucleus*, XEPL::Gene* );
	};
}
