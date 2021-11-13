//#include "8stl_queue.h"
#include "Queue.h"
#include <iostream>

using namespace lfp;
using namespace std;


int main() {
	queue<int> que;
	que.push(1);
	que.push(3);
	que.push(5);
	que.push(7);

	cout << "queue.size() = " << que.size() << endl << endl;
	
	cout << "queue.front() = " << que.front() << endl; que.pop();
	cout << "queue.front() = " << que.front() << endl; que.pop();
	cout << "queue.front() = " << que.front() << endl; que.pop();
	cout << "queue.front() = " << que.front() << endl; que.pop();

	cout << "\nqueue.size() = " << que.size() << endl << endl;

	return 0;
}