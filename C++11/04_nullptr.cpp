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
	f(0x12345678);// ambiguous,���Կ��� int��Ҳ���Կ���һ����ַ
	// ������ VS2019 �У�0x12345678 ������������ int ����
	return 0;
}