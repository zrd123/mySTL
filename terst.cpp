#include <cstring>
#include <iostream>

using namespace std;

int main() {
	string str1("1234567890");
	cout << "str1: " << str1 << endl;
	cout << str1.capacity();
	return 0;
}