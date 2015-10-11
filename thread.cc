#include <iostream>

#include <thread>
#include <mutex>

using namespace std;

mutex mtx;

void check(int i){
	for(int j=0;j<100;j++) {
			mtx.lock();
			cout<<"i "<<j<<"\t";
			mtx.unlock();
	}
}

int main() {
	thread test(check, 1);
	thread test1 (check, 1);
	test.join();
	test1.join();
}
