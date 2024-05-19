// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_http_kit.h
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

namespace KITS::HTTP
{
	void Keyword_HttpServer ( XEPL::Cortex* _cortex );
	void Register_Http_Kit  ( XEPL::Cortex* _cortex );
}

namespace KITS::HTTP
{
	class Http;

	typedef void ( Http::*HttpScanner ) ( void  );

	class Http : public XEPL::Senson
	{
		virtual void Senson_Closed  ( void ) override;
		virtual void Senson_Scan    ( void ) override;
		virtual void Http_Deliver   ( void ) = 0;
		void Header          ( XEPL::Cord*, XEPL::String* );
		void Method_Deliver  ( XEPL::Gene*, XEPL::Gene*   );
		void Scan            ( HttpScanner );
		void Scan_Payload    ( void );
	protected:
		HttpScanner    http_scanner;
		XEPL::Gene*    message_gene;
		XEPL::Axon*    request_axon;
		XEPL::Axon*    response_axon;
		XEPL::Axon*    closed_axon;
		size_t         payload_length;
		virtual ~Http ( void ) override;
	public:
		explicit Http ( XEPL::Neuron*, XEPL::Gene* );
		void Do_Next      ( HttpScanner );
		void Scan_Header  ( void );
	};

	class HttpServer : public Http
	{
		virtual void Http_Deliver( void ) override;
		void Scan_Request_Line   ( void );
		void Method_Respond      ( XEPL::Gene*, XEPL::Gene* );
	public:
		explicit HttpServer( XEPL::Neuron*, XEPL::Gene* );
	};
}
