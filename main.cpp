#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <thread>

using namespace std;

#define error(x) \
	do{ \
		cout << x; \
		exit(1); \
	}while(0);

int proRevXmitData (int proxfd, int serverfd) {
	char buf[2048] = {0};
	int len;
	while(1) {
			len = recv(serverfd, buf, sizeof(buf), 0);
			cout << "sec received " << len<<endl;
			if (len == 0)
				/* peer requested for shutdown */
				return 0;
			else if (len < 0)
					error("recv error\n");
			cout << "sec received " << len<<endl;
			len = send(proxfd, buf, len, 0);
			cout << "sec sent" << len<<endl;
			if (len < 0)
					error("send error\n");
	}
	return 0;
}

int proXmitData(int proxyfd) {
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
	thread proRevProcessThread (proRevXmitData, proxyfd, serverfd);
	while(1){
		len = recv(proxyfd, buf, sizeof(buf), 0);
		cout << "received " << len<<endl;
		if (len == 0) {
			proRevProcessThread.join();
			return 0;
		}
		else if (len < 0)
			error("recv error\n");
		len = send(serverfd, buf, len, 0);
		cout << "sent " << len<<endl;
		if (len < 0)
			error("send error\n");
	}
	return 0;
}

int main() {
	int sockfd, proxyfd; 
	socklen_t serlen;
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
	serlen = sizeof(server_sockaddr);
	while(1){
		proxyfd = accept(sockfd, (struct sockaddr *) &server_sockaddr, 
						&serlen);
		cout << "crossed 1"<<endl;
		/* Threading code to handle client
		 * Don't think a hash is required as proxyfd is sent to thread and all
		 * communication happens over it 
		 */
		//Weird error
		thread proProcessThread (proXmitData, proxyfd);
		proProcessThread.join();
	}
	return 0;
}
