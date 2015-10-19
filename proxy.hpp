#include "main.hpp"

class Proxy {
			int sockfd;
			int proXmitData (int);
			//int proRevXmitData (int, int);
	public :
			int proxySetup();
			int runProxy(int);
			Proxy();
			~Proxy();
};
