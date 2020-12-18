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
	f(0);	   // call f(int)
	f(NULL);   // call f(int)
	f(nullptr);// call f(void*)
	f(0x12345678);// ambiguous,可以看成 int，也可以看成一个地址
	// 但是在 VS2019 中，0x12345678 被编译器看成 int 类型
	return 0;
}