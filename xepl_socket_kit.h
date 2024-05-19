// SPDX: AGPL-3.0-only
/* XEPL Operating Environment - Copyright (c) 2024 Keith Edwin Robbins
	Project Name: XEPL Operating Environment
	File Name:    xepl_sockets_kit.h
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


namespace KITS::SOCKETS
{
	void Keyword_SocketMan   ( XEPL::Cortex* _cortex );
	void Keyword_Tcpserver   ( XEPL::Cortex* _cortex );
	void Register_Socket_Kit ( XEPL::Cortex* _cortex );
}


namespace KITS::OS
{
	class osFd;
	class osFdPair;
	class osFdSelect;
	class osFdSet;
	class osSocket;
	class osSocketAddress;
	class osSocketControl;
	class osTcpServer;
	class osTcpSocket;	
	
	typedef bool ( osFd::*osFdHandler ) ( void );
}


namespace KITS::SOCKETS
{
	class Socket;
	class SocketMan;
	class SocketControl;
	class SocketTcp;
	class TcpServer;

	class Socket : public XEPL::Neuron
	{
	protected:
		virtual ~Socket ( void ) override;
	public:
		OS::osSocket*  os_socket;
		XEPL::Gene*    config_gene;
		explicit Socket ( XEPL::Neuron*, XEPL::Gene* );
		virtual void Nucleus_Dropped( void ) override;
	};

	class SocketTcp : public Socket
	{
		friend class KITS::OS::osTcpSocket;
		OS::osTcpSocket*    os_tcp_socket;
		bool                aborted;
		virtual void Nucleus_Dropped( void ) override;
		void Closed_Socket_Write      ( void );
		void Closed_Socketman_Read    ( void );
		void Closed_Socketman_Write   ( void );
		void Method_Closed            ( XEPL::Gene*, XEPL::Gene* );
		void Method_SendContent       ( XEPL::Gene*, XEPL::Gene* );
		void Method_Start             ( XEPL::Gene*, XEPL::Gene* );
	protected:
		XEPL::Axon*       closed_axon;
		void Set_Tcp_Socket ( OS::osTcpSocket* );
		virtual ~SocketTcp  ( void ) override;
	public:
		explicit SocketTcp ( XEPL::Neuron*,    XEPL::Gene* );
		explicit SocketTcp ( OS::osTcpSocket*, XEPL::Neuron*, XEPL::Gene* );
	};

	class SocketControl : public Socket
	{
	public:
		OS::osSocketControl*  control_socket;
		explicit SocketControl (SocketMan* );
		void Send ( void );
	};

	class SocketMan : public XEPL::Lobe
	{
		friend class SocketControl;
		virtual void Lobe_Born      ( void ) override;
		virtual void Lobe_Dying     ( void ) override;
		virtual void Lobe_Rest_Now  ( void ) override;
		virtual void Lobe_Wake_Up   ( void ) override;
		void Do_Read      ( XEPL::Atom*, XEPL::Atom* );
		void Do_Write     ( XEPL::Atom*, XEPL::Atom* );
		void Do_Cancel    ( XEPL::Atom*, XEPL::Atom* );
		SocketControl*   socket_control;
		OS::osFdSet*     fdSet;
	public:
		XEPL::Axon*      read_axon;
		XEPL::Axon*      write_axon;
		XEPL::Axon*      cancel_axon;
		explicit SocketMan ( XEPL::Neuron*, XEPL::Gene* );
		void Unhook ( OS::osFd* );
	protected:
		virtual ~SocketMan ( void ) override;
	};


	class TcpServer : public SocketTcp
	{
		friend class OS::osTcpServer;
		XEPL::Axon*        connecting_axon;
		XEPL::String*      node_string;
		OS::osTcpServer*   server_socket;
	protected:
		virtual ~TcpServer ( void ) override;
	public:
		explicit TcpServer ( XEPL::Neuron*, XEPL::Gene* );
	private:
		void Set_Server_Socket    ( OS::osTcpServer* );
		void Server_Create        ( void );
		void Listen               ( void );
		bool SocketMan_Connecting ( int,  OS::osSocketAddress* );
		void Client_Connected     ( XEPL::Atom*, XEPL::Atom* );
	};
	
}


struct sockaddr;
struct sockaddr_in;

#ifdef _WIN32
#include <winsock2.h>
#else
typedef int SOCKET;
#endif

namespace KITS::OS
{
	class osFdHandlerMap : public XEPL::UMapT< SOCKET, std::pair<OS::osFd*, OS::osFdHandler> > {};

	class osFd : public XEPL::Atom
	{
		friend class OS::osFdPair;
		friend class SOCKETS::SocketMan;
		bool Fd_Does_Nothing( void );
		virtual SOCKET osFd_Descriptor      ( void ) = 0;
		virtual osFdHandler osFd_OnFdRead   ( void ) { return ( osFdHandler )&osFd::Fd_Does_Nothing; }
		virtual osFdHandler osFd_OnFdWrite  ( void ) { return ( osFdHandler )&osFd::Fd_Does_Nothing; }
	protected:
		XEPL::String*         socket_name;
		XEPL::Gene*           receiving_gene;
		XEPL::Wire*           backpressure_wire;
		XEPL::Mutex*          fd_mutex;
		virtual ~osFd ( void ) override;
		explicit osFd ( void );
		void Receive_From_Fd  ( XEPL::Text*, long );
	public:
		SOCKETS::SocketMan*    socket_manager;
		XEPL::Axon*           eReceived;
		bool Set_Manager       ( XEPL::Neuron* );
		void Set_Data_Axon     ( XEPL::Axon*   );
		void Add_Backpressure  ( XEPL::Wire*   );
		bool Has_Backpressure  ( void );
	};

	class osFdSelect : public XEPL::NoCopy
	{
		friend class osFdPair;
		::fd_set*   os_fd_set;
	public:
		~osFdSelect();
		osFdSelect ( void );
	private:

		void Set_Fd        ( int );
		bool Is_Fd_Ready   ( int );
		void Clear_Fd      ( int );
		int  Deliver_Fds   ( osFdPair*, int );
	};

	class osFdPair : public XEPL::NoCopy
	{
		friend class osFdSelect;
		friend class osFdSet;
		friend class SOCKETS::SocketMan;
		using SetInts = std::set<int, std::less<int>, XEPL::IntAllocator>;
	protected:
		SetInts*              all_fds;
		OS::osFdHandlerMap*   handler_map;
		OS::osFdSelect*       watch_fds;
		OS::osFdSelect*       active_fds;
		int                   num_fds;
		int                   max_fd;
	public:
		osFdPair ( void );
		~osFdPair ( void );
	protected:
		void Remove_Fd       ( int );
		void Ignore_Fd       ( osFd* file_descriptor );
		void Set_Fd_Handler  ( osFd* file_descriptor, osFdHandler to_do );
	private:
		int  Max_Fd          ( int current_max_fd );
		int  Deliver_Fd      ( int number_of_active_descriptors );
		void Clear_Bad_Fds   ( void );
		::fd_set* Active_Fds ( void );
	};

	class osFdSet : public XEPL::NoCopy
	{
		friend class SOCKETS::SocketMan;
		osFdPair*     read_pair;
		osFdPair*     write_pair;
	public:
		~osFdSet ( void );
		osFdSet  ( void );
	private:
		void Ignore            ( osFd*, osFdPair* );
		void Wait_On_Selected  ( void );
	};
}


namespace KITS::OS
{
	class osSocket : public osFd
	{
	protected:
		SOCKET                 socket_fd;
		SOCKETS::Socket*       socket_neuron;
	public:
		osSocketAddress*      socket_address;
	protected:
		friend class SOCKETS::Socket;	
		virtual ~osSocket ( void ) override;
		explicit osSocket ( SOCKETS::Socket* );
		explicit osSocket ( SOCKETS::Socket*, int, osSocketAddress* );
		virtual SOCKET  osFd_Descriptor( void ) override;
		void Bind_Socket  ( bool );
		void Build_Socket ( int  );
		void Close_Socket ( void );
	};

	class osSocketAddress : public XEPL::NoCopy
	{
		::sockaddr_in*           sockAddr;
		XEPL::String*            ip_string;
	public:
		~osSocketAddress         ( void );
		osSocketAddress          ( void );
		explicit osSocketAddress ( XEPL::Gene* );
		long Length              ( void );
		long Port                ( void );
		XEPL::Cord* IpString     ( void );
		::sockaddr* Get          ( void );
		static void Hostname     ( XEPL::String* );
	};

	class osSocketControl : public osSocket
	{
		SOCKETS::SocketControl*   socket_control;
	public:
		explicit osSocketControl ( SOCKETS::SocketControl* );
	private:
		friend class SOCKETS::SocketControl;
		virtual osFdHandler osFd_OnFdRead ( void ) override;
		bool    Fd_Receive       ( void );
		void    Send             ( void );
	};

	class osTcpSocket : public osSocket
	{
		virtual osFdHandler osFd_OnFdRead  ( void ) override {
			return ( osFdHandler )&osTcpSocket::Read_Available;
		}
		virtual osFdHandler osFd_OnFdWrite ( void ) override {
			return ( osFdHandler )&osTcpSocket::Send_Backpressure;
		}
	public:
		SOCKETS::SocketTcp*             socket_tcp;
		explicit osTcpSocket         ( SOCKETS::SocketTcp* );
		explicit osTcpSocket         ( SOCKETS::SocketTcp*, int, osSocketAddress* );
		void Create_Tcp_Socket       ( void );
		void Send_Data               ( XEPL::Wire* );
		bool Read_Available          ( void );
		bool Send_Backpressure       ( void );
	};

	class osTcpServer : public osTcpSocket
	{
		SOCKETS::TcpServer*       tcp_server;
		virtual osFdHandler osFd_OnFdRead ( void ) override {
			return ( osFdHandler )&osTcpServer::Fd_Is_Connecting;
		}
	protected:
		virtual ~osTcpServer() override;
	public:
		explicit osTcpServer         ( SOCKETS::TcpServer* );
		void Listen_For_Connections  ( long );
		bool Fd_Is_Connecting        ( void );
	};
}


