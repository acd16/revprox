#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

using namespace std;

#define error(x) \
	do{ \
		cout << x; \
		exit(0); \
	}while(0);

int main(){
	int proxyfd, serverfd; 
	socklen_t serlen;
	struct sockaddr_in proxy_sockaddr = {0}, server_sockaddr = {0};
	proxyfd = socket(AF_INET, SOCK_STREAM, 0);
	if (proxyfd < 0)
		error("error opening socket");
	proxy_sockaddr.sin_family = AF_INET;
	proxy_sockaddr.sin_port = htons(80);
	proxy_sockaddr.sin_addr.s_addr = INADDR_ANY;
	if (bind (proxyfd, (struct sockaddr *) &proxy_sockaddr, 
							sizeof(proxy_sockaddr)) < 0)
		error("error on bind");
	listen(proxyfd, 5);
	serlen = sizeof(server_sockaddr);
	while(1){
		serverfd = accept(proxyfd, (struct sockaddr *) &server_sockaddr, 
						&serlen);
		/* Threading code to handle client */
		/* Don't think a hash is required as serverfd is sent to thread and all
		 * communication happens over it*/
	}
}
