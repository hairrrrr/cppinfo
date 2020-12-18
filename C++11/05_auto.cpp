#include<iostream>
#include<vector>

void testauto()
{
	std::vector<int> v;
	// it has type std::vector<int>::iterator
	auto it = v.begin();
	// l has type bool (int)
	auto l = [](int x) -> bool {return true; };
	// l is an object representing a lambda
}