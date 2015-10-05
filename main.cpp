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

int proRevXmitData (int servfd, int clifd) {
	char buf[2048] = {0};
	int len;
	len = recv(clifd, buf, sizeof(buf), 0);
	if (len < 0)
		error("recv error\n");
	if (send(servfd, buf, len, 0) < 0)
		error("send error\n");
}

int proXmitData(int fd) {
	int clientfd;
	char buf[2048] = {0};
	int len;
	struct sockaddr_in client_sockaddr = {0};
	string clientIp = "127.0.0.1";
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	client_sockaddr.sin_family = AF_INET;
	client_sockaddr.sin_port = htons(8080);
	client_sockaddr.sin_addr.s_addr = inet_addr(clientIp.c_str());
	if (connect (clientfd, (struct sockaddr *) &client_sockaddr,
						sizeof(client_sockaddr)) < 0)
		error("connect failure\n");
	//thread proRevProcessThread (proRevXmitData, fd, clientfd);
	len = recv(fd, buf, sizeof(buf), 0);
	cout << "received " << len<<endl;
	if (len < 0)
		error("recv error\n");
	if (send(clientfd, buf, len, 0) < 0)
		error("send error\n");
}

int main() {
	int proxyfd, serverfd; 
	socklen_t serlen;
	int optval = 1;
	struct sockaddr_in proxy_sockaddr = {0}, server_sockaddr = {0};
	proxyfd = socket(AF_INET, SOCK_STREAM, 0);
	if (proxyfd < 0)
		error("error opening socket\n");
	proxy_sockaddr.sin_family = AF_INET;
	proxy_sockaddr.sin_port = htons(8080);
	proxy_sockaddr.sin_addr.s_addr = INADDR_ANY;
	setsockopt (proxyfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	if (bind (proxyfd, (struct sockaddr *) &proxy_sockaddr, 
							sizeof(proxy_sockaddr)) < 0)
		error("error on bind\n");
	listen(proxyfd, 5);
	serlen = sizeof(server_sockaddr);
	while(1){
		serverfd = accept(proxyfd, (struct sockaddr *) &server_sockaddr, 
						&serlen);
		cout << "crossed 1"<<endl;
		/* Threading code to handle client
		 * Don't think a hash is required as serverfd is sent to thread and all
		 * communication happens over it 
		 */
		//Weird error
		//thread proProcessThread (proXmitData, serverfd);
	}
}
