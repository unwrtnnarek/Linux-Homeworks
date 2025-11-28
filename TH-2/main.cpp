#include "parallel_scheduler.h"
#include <unistd.h>
#include <iostream>



void tasker(void *data) {
	int id = *static_cast<int*>(data);
	std::cout << "task " << id << "started working" << std::endl;
	usleep(300000);
	std::cout << "task " << id << "finished" << std::endl;

}

int main() {
	
	parallel_scheduler pool(4);
	int val[20];
	for(int i = 0 ; i < 20; i++) {
		val[i] = i;
		pool.run(tasker, &val[i]);
	}
	sleep(3);
	return 0;
}
