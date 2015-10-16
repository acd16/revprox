#include "proxy.hpp"

int Proxy::proXmitData() {
	int serverfd;
	char buf[2048] = {0};
	int len;
	struct sockaddr_in client_sockaddr = {0};
	string clientIp = "127.0.0.1";
	serverfd = socket(AF_INET, SOCK_STREAM, 0);
	client_sockaddr.sin_family = AF_INET;
	client_sockaddr.sin_port = htons(80);
	client_sockaddr.sin_addr.s_addr = inet_addr(clientIp.c_str());
	if (connect (serverfd, (struct sockaddr *) &client_sockaddr,
						sizeof(client_sockaddr)) < 0)
		error("connect failure\n");
	thread proRevProcessThread (&Proxy::proRevXmitData, this);
	while(1){
		len = recv(proxyfd, buf, sizeof(buf), 0);
		cout << "DBG received " << len<<endl;
		if (len == 0) {
			proRevProcessThread.join();
			return 0;
		}
		else if (len < 0)
			error("recv error\n");
		len = send(serverfd, buf, len, 0);
		cout << "DBG sent " << len<<endl;
		if (len < 0)
			error("send error\n");
	}
	return 0;
}

int Proxy::proRevXmitData () {
	char buf[2048] = {0};
	int len;
	while(1) {
			len = recv(serverfd, buf, sizeof(buf), 0);
			cout << "DBG sec received " << len<<endl;
			if (len == 0)
				/* peer requested for shutdown */
				return 0;
			else if (len < 0)
					error("recv error\n");
			cout << "DBG sec received " << len<<endl;
			len = send(proxyfd, buf, len, 0);
			cout << "DBG sec sent" << len<<endl;
			if (len < 0)
					error("send error\n");
	}
	return 0;
}

int Proxy::proxySetup() {
	int optval = 1;
	struct sockaddr_in proxy_sockaddr = {0}, server_sockaddr = {0};
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error("error opening socket\n");
	proxy_sockaddr.sin_family = AF_INET;
	proxy_sockaddr.sin_port = htons(8080);
	proxy_sockaddr.sin_addr.s_addr = INADDR_ANY;
	setsockopt (sockfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind (sockfd, (struct sockaddr *) &proxy_sockaddr, 
							sizeof(proxy_sockaddr)) < 0)
		error("error on bind\n");
	listen(sockfd, 5);
	return sockfd;
}

int Proxy::runProxy(int sockfd) {
	socklen_t serlen;
	struct sockaddr_in server_sockaddr = {0};
	while(1){
		proxyfd = accept(sockfd, (struct sockaddr *) &server_sockaddr, 
						&serlen);
		cout << "DBG crossed 1"<<endl;
		thread proProcessThread (&Proxy::proXmitData, this);
		proProcessThread.join();
	}
	return 0;
}

Proxy::~Proxy(){
		close(serverfd);
		close(proxyfd);
		close(sockfd);
}

Proxy::Proxy(){
}
