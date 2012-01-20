#include <iostream>
#include <vector>

int main () {
	std::vector<int> lolo;
	lolo.push_back (100500);
	lolo.push_back (10);
	lolo.push_back (100500);
	lolo.push_back (10);
	std::cout << "hello\n" << sizeof(lolo) << '\n';
	std::cout << sizeof(lolo[0]) << '\n';
	std::cout << sizeof(lolo[1]) << '\n';
	std::cout << sizeof(lolo[2]) << '\n';
	std::cout << sizeof(lolo[3]) << '\n';

	return 0;
}
