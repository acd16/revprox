#include "main.hpp"

class Proxy {
		private :
				int sockfd, serverfd, proxyfd;
				int proXmitData ();
				int proRevXmitData ();
		public :
				int proxySetup();
				int runProxy(int);
				Proxy();
				~Proxy();
};
