#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <thread>

using namespace std;

#define error(x) \
	do{ \
		cout << x; \
		exit(1); \
	}while(0);
