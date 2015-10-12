#include "proxy.hpp"

int main() {
	Proxy * local = new Proxy();
	auto fd = local->proxySetup();
	local->runProxy(fd);
	delete local;
	return 0;
}
