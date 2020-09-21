
#include "ntConnection.h"
#include "ntEventHandler.h"

namespace nettools {

	static NT_ERROR wsa_to_nt_error(int wsa_error) {
		switch (wsa_error) {

		case WSAEWOULDBLOCK:
			return NTERR_WOULDBLOCK;
		case WSAEADDRINUSE:
			return NTERR_ADDRINUSE;
		case WSAECONNREFUSED:
			return NTERR_CONNREFUSED;
		}

		return NTERR_OTHER;
	}

	static NT_ERROR wsa_error_or_success(int result) {
		if (result == SOCKET_ERROR)
			return wsa_to_nt_error(::WSAGetLastError());
		return NTERR_SUCCESS;
	}



	NT_ERROR ntConnection::_socket(int *socket) {
		int s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		*socket = s;
		return NTERR_SUCCESS;
	}

	NT_ERROR ntConnection::_bind(int socket, sockaddr *address, u32 len) {
		int result = ::bind(socket, address, len);
		return wsa_error_or_success(result);
	}

	NT_ERROR ntConnection::_listen(int socket) {
		int result = ::listen(socket, 10);
		return wsa_error_or_success(result);
	}

	NT_ERROR ntConnection::_accept(int socket, ntConnection **newClient) {
		sockaddr_in clientAddr;
		int addrLen = sizeof(sockaddr_in);
		int newSocket = ::accept(m_socket, (sockaddr*)&clientAddr, &addrLen);
		
		NT_ERROR err = wsa_error_or_success(newSocket);
		if (err == NTERR_SUCCESS) {
			*newClient = new ntConnection(newSocket, (sockaddr*)&clientAddr, sizeof(clientAddr));
		}
		else {
			*newClient = NULL;
		}
		return err;
	}

	NT_ERROR ntConnection::_connect(int socket, sockaddr *address, u32 len) {
		int res = ::connect(socket, address, len);
		return wsa_error_or_success(res);
	}

	NT_ERROR ntConnection::_send(int socket, const u8 *data, u32 len) {
		int result = ::send(socket, (const char*)data, len, 0);
		return wsa_error_or_success(result);
	}

	NT_ERROR ntConnection::_recv(int socket, u8 *data, u32 maxlen, u32 *recvLen) {
		int result = ::recv(socket, (char*)data, maxlen, 0);
		NT_ERROR err = wsa_error_or_success(result);
		if (err == NTERR_SUCCESS) {
			*recvLen = result;
		}
		return err;
	}

	NT_ERROR ntConnection::_close(int socket) {
		::closesocket(socket);
		return NTERR_SUCCESS;
	}

	ntConnection::ntConnection() {
		WSADATA wsaData;
		int result = ::WSAStartup(MAKEWORD(2, 2), &wsaData);
		_socket(&m_socket);
		setBlockingMode(false);
		m_boundData = NULL;
	}

	ntConnection::ntConnection(int sock, sockaddr *address, u32 len) {
		m_socket = sock;
		m_address = *(sockaddr_in*)address;
	}

	ntConnection::~ntConnection() {
		close();
		int result = ::WSACleanup();
	}

	NT_ERROR ntConnection::setBlockingMode(bool blocking) {
		u_long mode = blocking ? 0 : 1;
		int result = ::ioctlsocket(m_socket, FIONBIO, &mode);
		return wsa_error_or_success(result);
	}

	NT_ERROR ntConnection::getOption() {
		return NTERR_SUCCESS;
	}

	NT_ERROR ntConnection::setOption() {
		return NTERR_SUCCESS;
	}

	NT_ERROR ntConnection::send(int socket, const u8 *data, u32 len) {
		return _send(socket, data, len);
	}

	NT_ERROR ntConnection::recv(int socket, u8 *data, u32 maxlen, u32 *recvLen) {
		return _recv(socket, data, maxlen, recvLen);
	}

	NT_ERROR ntConnection::poll() {
		return NTERR_SUCCESS;
	}

	NT_ERROR ntConnection::close() {
		::shutdown(m_socket, SD_BOTH);
		return _close(m_socket);
	}

}