//#include<iostream>
//#include<bitset>
//#include<string>
//
//// =========== test1 ===========
//// ���һ�ε��� print ���õ�����
//void print()
//{}
//
//template<typename... Types>
//void print(const Types&... args)
//{}
//// ����汾��������ػ��汾�������� print ����ʱ��������ػ��̶���ߵģ����ʺϵģ�
//template<typename T, typename... Types>
//void print(const T& firstArg, const Types&... args)
//{
//	std::cout << firstArg << std::endl;
//	print(args...);
//}
//
//// =========== test2 ===========
//
//struct Customer
//{
//	std::string _fname;
//	std::string _lname;
//	int _no;
//	Customer(std::string fname = "Wang", std::string lname = "Shepard", int no = 21)
//		:_fname(fname), _lname(lname), _no(no)
//	{}
//};
//
//#include<functional>
//template<typename T>
//inline void hash_combine(size_t& seed, const T& val)
//{
//	// std::hash<T>() ����һ����������
//	seed ^= std::hash<T>()(val) + 0x9e3779b9
//		+ (seed << 6) + (seed >> 2);
//}
//
//// �����ػ���������������
//template<typename T>
//inline void hash_val(size_t& seed, const T& val)
//{
//	hash_combine(seed, val);
//}
//
//template<typename T, typename... Types>
//inline void hash_val(size_t& seed, const T& val,
//	const Types&... Args)
//{
//	hash_combine(seed, val);
//	hash_val(seed, Args...);
//}
//
//template<typename... Types>
//inline size_t hash_val(const Types&... args)
//{
//	size_t seed = 0;
//	hash_val(seed, args...);
//	return seed;
//}
//
//class CustomerHash
//{
//public:
//	size_t operator()(const Customer& c) const
//	{
//		return hash_val(c._fname, c._lname, c._no);
//	}
//};
//
//
//int main(void)
//{
//	//print(7.5, "hello", std::bitset<16>(377), 43);
//	/*	7.5
//		hello
//		0000000101111001
//		43*/
//	
//	CustomerHash ch;
//	std::cout << (ch(Customer())) << std::endl;
//	std::cout << (ch(Customer("sementhe", "huang", 27))) << std::endl;
//	//1179791189
//	//3523170186
//	return 0;
//}