#include "WebSocket.hpp"

#define max_clients 30

namespace ws
{	
	struct sockaddr_in address; 
	int m_server, client_socket[max_clients];
	
	WebSocket::WebSocket(int sockPort)
	{
		int opt = true; 
		int i;
	
		//initialise all client_socket[] to 0 so not checked 
		for(i = 0 ; i < max_clients ; i++) 
		{ 
			client_socket[i] = 0; 
		} 
		
		//create a master socket 
		if((m_server = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
		{ 
			perror("socket failed");
		} 
	
		//set master socket to allow multiple connections , 
		//this is just a good habit, it will work without this 
		if(setsockopt(m_server, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt)) < 0) 
		{ 
			perror("setsockopt");
		} 
	
		//type of socket created 
		address.sin_family = AF_INET; 
		address.sin_addr.s_addr = INADDR_ANY; 
		address.sin_port = htons(sockPort); 
		
		//bind the socket to localhost port 8888 
		if(bind(m_server, (struct sockaddr *)&address, sizeof(address)) < 0) 
		{ 
			perror("bind failed");
		} 
		printf("Listener on port %d \n", std::to_string(sockPort)); 
		
		//try to specify maximum of 3 pending connections for the master socket 
		if(listen(m_server, 10) < 0) 
		{ 
			perror("listen");
		}
		
		m_running = true;
		
		//m_running = false;
		//m_server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
		//
		//if (0 > m_server)
		//{
		//	Debug::Error("Could not create socket.");
		//	exit(ERR_NETWORK);
		//}
		//
		//int opt = YES;
		//
		//if (0 > setsockopt(m_server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)))
		//{
		//	Debug::Error("Socket reuse setup failed.", errno);
		//	exit(ERR_NETWORK);
		//}
		//
		//if (0 > setsockopt(m_server, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int)))
		//{
		//	Debug::Error("Socket keep alive setup failed.", errno);
		//	exit(ERR_NETWORK);
		//}
		//
		//struct sockaddr_in server_addr;
		//
		//memset(&server_addr, 0, sizeof(server_addr));
		//
		//server_addr.sin_family = AF_INET;
		//server_addr.sin_addr.s_addr = INADDR_ANY;
		//server_addr.sin_port = htons(sockPort);
		//
		//int binding = bind(m_server, (struct sockaddr *) &server_addr, sizeof(server_addr));
		//
		//if (0 > binding)
		//{
		//	Debug::Error("Could not bind socket.");
		//	return;
		//}
		//
		//m_server_addr = &server_addr;
		//
		//m_running = true;
	}

	WebSocket::~WebSocket()
	{
		m_running = false;
	}

	std::string WebSocket::AcceptKey()
	{

		return AcceptKey(m_key);
	}

	std::string WebSocket::AcceptKey(std::string key)
	{
		if (key.empty())
			key = m_key;

		std::string result = "";

		if (key.empty())
			return result;

		key += RFC6455_KEY_GUID;                                     // RFC 6544 Magic GUID

		unsigned char digest[20];                                    // 160-bit SHA1 hash container
		SHA1 sha;

		sha.Input(key.data(), key.size());
		sha.Result((unsigned *) digest);

		// Little to Big Endian
		for(unsigned int i = 0 ; i < sizeof(digest) ; i += 4)
		{
			digest[i]        ^= digest[i + 3];
			digest[i + 3]    ^= digest[i];
			digest[i]        ^= digest[i + 3];

			digest[i + 1]    ^= digest[i + 2];
			digest[i + 2]    ^= digest[i + 1];
			digest[i + 1]    ^= digest[i + 2];
		}

		result = base64_encode((const unsigned char *) digest, sizeof(digest));

		return result;
	}

	void WebSocket::Handshake()
	{
		std::string content = "";

		content += "HTTP/1.1 101 Switching Protocols\r\n";
		content += "Upgrade: WebSocket\r\n";
		content += "Connection: Upgrade\r\n";

		if (0 < m_key.length())
			content += "Sec-WebSocket-Accept: " + m_accept + "\r\n";

		if (0 < m_protocol.length())
			content += "Sec-WebSocket-Protocol: " + m_protocol + "\r\n";

		content += "\r\n";

		Response *res = new Response(content.c_str());

		write(m_client, res->ToString().c_str(), res->Length());

		Debug::Info("Handshake accepted.");
	}

	void WebSocket::ParseHandshake()
	{
		//char request[65535];
		//int request_length = 0;
		//
		//memset(request, 0, sizeof(request));
		//
		//request_length = read(m_client, request, sizeof(request));
		//
		//if (0 >= request_length)
		//{
		//	Debug::Warn("Request with no data ignored.");
		//	return;
		//}
		//
		//Request *req = new Request(request);
		//
		//m_host = req->GetHeader("Host");
		//m_origin = req->GetHeader("Origin");
		//m_protocol = req->GetHeader("Sec-WebSocket-Protocol");
		//m_key = req->GetHeader("Sec-WebSocket-Key");
		//m_accept = AcceptKey();
		
		m_host = "";
		m_origin = "";
		m_protocol = "";
		m_key = "";
		m_accept = "";
	}

	void WebSocket::Listen()
	{
		char buffer[1025];
		//int m_client;
		int client_len = sizeof(address); 
		int activity;
		int valread;
		int sd;
		int max_sd;
		int i;
		fd_set readfds; 
		char *message = "ECHO Daemon v1.0 \r\n"; 
	
		puts("Waiting for connections ...");
	
		while (m_running) 
		{ 
			//clear the socket set 
			FD_ZERO(&readfds); 
	
			//add master socket to set 
			FD_SET(m_server, &readfds); 
			max_sd = m_server; 
			
			//add child sockets to set 
			for(i = 0 ; i < max_clients ; i++) 
			{ 
				//socket descriptor 
				sd = client_socket[i]; 
				
				//if valid socket descriptor then add to read list 
				if(sd > 0) 
					FD_SET(sd, &readfds); 
				
				//highest file descriptor number, need it for the select function 
				if(sd > max_sd) 
					max_sd = sd; 
			} 
	
			//wait for an activity on one of the sockets , timeout is NULL , 
			//so wait indefinitely
			struct timeval tv;
			tv.tv_sec = 0;
			tv.tv_usec = 1000;
			activity = select(max_sd + 1, &readfds, NULL, NULL, &tv); 
	
			if ((activity < 0) && (errno != EINTR)) 
			{ 
				printf("select error"); 
			}
			
			//If something happened on the master socket , 
			//then its an incoming connection 
			if(FD_ISSET(m_server, &readfds)) 
			{ 
				if ((m_client = accept(m_server, (struct sockaddr *)&address, (socklen_t*)&client_len)) < 0) 
				{ 
					perror("accept");
				} 
			
				//inform user of socket number - used in send and receive commands 
				//printf("New connection"); // , socket fd is %d , ip is : %s , port : %d \n", m_client, inet_ntoa(address.sin_addr), ntohs(address.sin_port)); 
		
				//send new connection greeting message 
				//if(send(m_client, message, strlen(message), 0) != strlen(message)) 
				//{ 
				//	perror("send"); 
				//} 
				//
				//puts("Welcome message sent successfully"); 
				
				ClientInfo info;
				info.descriptor = m_client;
				info.address = address;
				
				CallbackManager::Trigger(CT_CONNECTED, info, m_key.c_str());
				
				ParseHandshake();
				Handshake();
				
				//add new socket to array of sockets 
				for(i = 0 ; i < max_clients ; i++) 
				{ 
					//if position is empty 
					if(client_socket[i] == 0) 
					{ 
						client_socket[i] = m_client; 
						printf("Adding to list of sockets as %d\n", i); 
						
						break; 
					} 
				} 
			} 
			
			//else its some IO operation on some other socket 
			for(i = 0 ; i < max_clients ; i++) 
			{ 
				sd = client_socket[i]; 
				
				if (FD_ISSET(sd, &readfds)) 
				{ 
					//Check if it was for closing , and also read the 
					//incoming message 
					if((valread = read(sd, buffer, 1024)) == 0) 
					{ 
						//Somebody disconnected , get his details and print 
						getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&client_len); 
						printf("Host disconnected");   // , ip %s , port %d \n", inet_ntoa(address.sin_addr), ntohs(address.sin_port)); 
						
						ClientInfo info;
						info.descriptor = sd;
						info.address = address;
						CallbackManager::Trigger(CT_DISCONNECTED, info);
						
						//Close the socket and mark as 0 in list for reuse 
						close(sd); 
						client_socket[i] = 0; 
					} 
					
					//Echo back the message that came in 
					else
					{ 
						//set the string terminating NULL byte on the end 
						//of the data read 
						//buffer[valread] = '\0'; 
						//send(sd, buffer, strlen(buffer), 0); 
						
						char *end;
						std::string reason;
						struct Frame frame;
						Frame::ParseResult result = Frame::Parse(buffer, sizeof(buffer), frame, end, reason); 
						
						memset(&buffer, 0, sizeof(buffer));
						
						ClientInfo info;
						info.descriptor = sd;
						info.address = address;
						
						CallbackManager::Trigger(CT_FRAME, info, &frame);
						
						if (NULL == frame.payload)
							continue;
						
						CallbackManager::Trigger(CT_MESSAGE, info, frame.payload);
					}
				}
			}
		}
		
		//int client_len = sizeof(struct sockaddr_in);
		//
		//listen(m_server, SOCK_BACKLOG);
		//
		//struct sockaddr_in client_addr;
		//
		//memset(&client_addr, 0, sizeof(client_addr));
		//
		//while (m_running && 0 <= (m_client = accept(m_server, (struct sockaddr *) &client_addr, (socklen_t *) &client_len)))
		//{
		//	Debug::Warn("Client Connected");
		//
		//	ClientInfo info;
		//	info.descriptor = m_client;
		//	info.address = client_addr;
		//
		//	CallbackManager::Trigger(CT_CONNECTED, info, m_key.c_str());
		//
		//	ParseHandshake();
		//	Handshake();
		//
		//	int in = 0;
		//	int buffer_length = 512;
		//	char buffer[buffer_length];
		//
		//	while (0 != (in = recv(m_client, &buffer, buffer_length, 0)))
		//	{
		//		char *end;
		//		std::string reason;
		//		struct Frame frame;
		//		Frame::ParseResult result = Frame::Parse(buffer, sizeof(buffer), frame, end, reason); 
		//
		//		memset(&buffer, 0, sizeof(buffer));
		//
		//		CallbackManager::Trigger(CT_FRAME, info, &frame);
		//
		//		if (NULL == frame.payload)
		//			continue;
		//
		//		CallbackManager::Trigger(CT_MESSAGE, info, frame.payload);
		//	}
		//
		//	Debug::Warn("Client Disconnected");
		//	CallbackManager::Trigger(CT_DISCONNECTED, info);
		//}
		//
		//Debug::Warn("Server Closed");
	}
	
	void SignalCallbackHandler(int signum)
	{
		printf("Caught signal SIGPIPE %d\n", signum);
	}

	void WebSocket::Send(std::string message)
	{
		struct Frame response(static_cast<Frame::Type>(Frame::TEXT), true, false, message.c_str(), (size_t) message.size());
		std::vector<byte> data;

		response.Data(data, message.size());
		
		signal(SIGPIPE, SignalCallbackHandler);
		//write(m_client, data.data(), data.size());
		
		for(unsigned int i = 0 ; i < max_clients ; i++)
			if(client_socket[i] != 0)
			write(client_socket[i], data.data(), data.size());
	}

	void WebSocket::Stop()
	{
		m_running = false;
	}
}
