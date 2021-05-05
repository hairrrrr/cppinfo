int main()
{
	int i;    // i has undefined value
	int j{};  // j is initialized by 0
	int* q;   // q has undefined value
	int* p{}; // p is initialized by 0

	return 0;
}

int main()
{
	//int x1(5.3);      // OK
	//int x2 = 5.3;     // OK
	//int x3{ 5.0 };	  // ERROR: narrowing
	//int x4 = { 5.3 }; // ERROR: narrowing
	//char c1{ 7 };	  // OK: even thongh 7 is an int, this is not narrowing
	char c2{ 99999 }; // ERROR: narrowing 
					  // WARNING: overflow in implict constant conversion

	return 0;
}

void print(std::initializer_list<int> vals)
{
	for (auto p = vals.begin(); p != vals.end(); ++p)
		cout << *p << " ";
	cout << endl;
}

int main()
{
	print({ 1, 2, 3, 4, 5 });

	return 0;
}


class P
{
public:
	P(int a, int b)
	{
		cout << "P(int, int), a = " << a << ", b = " << b << endl;
	}

	P(initializer_list<int> initlist)
	{
		cout << "P(initializer_list<int>), value = ";
		for (auto i : initlist)
			cout << i << ' ';
		cout << endl;
	}
};

int main()
{
	P p(12, 13);		// P(int, int)
	P q{ 12, 13 };		// P(initializer_list<int>)
	P r{ 12, 13, 14 };	// P(initializer_list<int>)
	P s = { 12, 13 };	// P(initializer_list<int>)

	return 0;
}

#define _CRT_SECURE_NO_WARNINGS

#include<vector>
#include<iostream>

using namespace std;

int main()
{
	vector<int> v1{ 1, 2, 3 };
	vector<int> v2({ 1, 2, 3 });
	// call 
	// vector<initializer_list<value_type>, const allocator _type& __a = allocator_type())
	//  : Base(__a)
	// {_M_range_initialize(__l.begin(), __l.end(), random_access_iterator_tag());}

	vector<int> v3;
	v3 = { 1, 2, 3 };
	// call
	// vector& operator=(initializer_list<value_type> __l)
	// {this->assign(__l.begin(), __l.end(); return *this;)}

	v3.insert(v3.begin() + 2, { 1, 2, 3 });
	// call 
	// void insert(iterator __position, initializer_list<value_type> __l)
	// {this->insert(__position, __l.begin(), __l.end());}

	cout << max({ 1, 2, 3 }) << endl;
	//template<typename _Tp>
	//	inline _Tp min(initializer_list<_Tp> __l)
	//	{return *std::max_element(__l.begin(), __l.end());}

	cout << min({ 1, 2, 3 }) << endl;
	//template<typename _Tp>
	//	inline _Tp min(initializer_list<_Tp> __l)
	//	{return *std::min_element(__l.begin(), __l.end());}

	return 0;
}