#include <iostream>
#include <iterator>
#include <numeric>

int main()
{
	std::istream_iterator<int> it(std::cin);
	std::istream_iterator<int> end;

	std::cout << std::accumulate(it, end, 0) << '\n';

	return 0;
}
