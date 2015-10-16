#include <iostream>

#include <thread>
#include <mutex>
#include <chrono>

using namespace std;

mutex mtx;

class Thread {
		public : 
				void check(char);
};

void Thread::check(char i){
	for(int j=0;j<100;j++) {
			mtx.lock();
			cout<<i << " "<<j<<"\t";
			mtx.unlock();
	}
}

int main() {
	Thread * chek = new Thread();
	thread test(&Thread::check, chek, 'a');
	thread test1 (&Thread::check, chek, 'b');
	while(1){
		cout<<"in loop\n"<<endl;
		this_thread::sleep_for(chrono::seconds(2));
	}
	test.join();
	test1.join();
}
