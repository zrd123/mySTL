#include "Alloc.h"
#include <iostream>
#include "Vector.h"

using namespace lfp;

int main() {
	int ia[5] = { 0, 1, 2, 3, 4 };

	vector<int, TinySTL::allocator<int>> iv(ia, ia + 5);
	for (int i = 0; i < 5; ++i) {
		std::cout << iv[i] << " ";
	}
	std::cout << std::endl;

	return 0;
}
