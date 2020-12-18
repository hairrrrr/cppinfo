#include<iostream>

void f(int i)
{
	std::cout << "int" << std::endl;
}
void f(void* p)
{
	std::cout << "void*" << std::endl;
}



int main(void)
{

	return 0;
}