### 一 Variadic Templates

*test1.cpp*

```cpp
#include<iostream>
#include<bitset>

// 最后一次调用 print 调用的是它
void print()
{}

// const Types&... 表示参数个数不确定（参数包），可以是 0 
template<typename... Types>
void print(const Types&... args)
{}
// 这个版本是上面的特化版本，当调用 print 函数时，会调用特化程度最高的（最适合的）
template<typename T, typename... Types>
void print(const T& firstArg, const Types&... args)
{
	std::cout << firstArg << std::endl;
	print(args...);
}

int main(void)
{
	print(7.5, "hello", std::bitset<16>(377), 43);
	/*	7.5
		hello
		0000000101111001
		43*/
	return 0;
}
```

注意几个细节：

- `template<typename... Types>`
- `void print(const Types&... args)`
- `print(args...);`



*test2.cpp*

```cpp
struct Customer
{
	std::string _fname;
	std::string _lname;
	int _no;
	Customer(std::string fname = "Wang", std::string lname = "Shepard", int no = 21)
		:_fname(fname), _lname(lname), _no(no)
	{}
};

#include<functional>
template<typename T>
inline void hash_combine(size_t& seed, const T& val)
{
	// std::hash<T>() 这是一个匿名对象
	seed ^= std::hash<T>()(val) + 0x9e3779b9
		+ (seed << 6) + (seed >> 2);
}

// “最特化”（结束条件）
template<typename T>
inline void hash_val(size_t& seed, const T& val)
{
	hash_combine(seed, val);
}

template<typename T, typename... Types>
inline void hash_val(size_t& seed, const T& val,
	const Types&... Args)
{
	hash_combine(seed, val);
	hash_val(seed, Args...);
}

template<typename... Types>
inline size_t hash_val(const Types&... args)
{
	size_t seed = 0;
	hash_val(seed, args...);
	return seed;
}

class CustomerHash
{
public:
	size_t operator()(const Customer& c) const
	{
		return hash_val(c._fname, c._lname, c._no);
	}
};


int main(void)
{	
	CustomerHash ch;
	std::cout << (ch(Customer())) << std::endl;
	std::cout << (ch(Customer("sementhe", "huang", 27))) << std::endl;
	//1179791189
	//3523170186
	return 0;
}
```



### 二 auto

```cpp
auto i = 42; // i has type int
double f();
auto d = f(); // d has type double
```

`auto` 常用于当一个类型很长或很复杂时：

```cpp
#include<iostream>
#include<vector>

void f(int i)
{
	std::cout << "int" << std::endl;
}
void f(void* p)
{
	std::cout << "void*" << std::endl;
}

void testauto()
{
	std::vector<int> v;
	// it has type std::vector<int>::iterator
	auto it = v.begin();
	// l has type bool (int)
	auto l = [](int x) -> bool {return true; };
	// l is an object representing a lambda
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
```



### 三 initializer_list





### 其他特性 

#### 1. `__cpluscplus` 

可以用来看编译器支持的 C++ 版本：

```cpp
cout << __cplusplus << endl;
```

我的 VS2019 是支持 C++ 11 的，但是输出的是：

```
199711
```



#### 2. 模板表达式中的空格

```cpp
vector<list<int> >; // OK in each C++ version
vector<list<int>> ; // OK since C++11
```

在 `C++11` 前的版本中，如果在不用空格分开两个 `>` 的话，编译器会把 `>>` 当作运算符，从而产生错误。



#### 3. `nullptr` and `nullptr_t`

```cpp
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
```

`nullptr_t` 则是一个类型

`\include\stddef.h`

```cpp
typedef decltype(nullptr) nullptr_t
```



#### 4. Uniform Initialization

```cpp
double d = { 1e9 };

int value[]{ 1, 2, 3 };

vector<int> v{2, 3, 5, 7, 11, 13, 17};

vector<string> cities{
    "Shanghai", "Xi'an", "New York"
};

complex<double> c{ 4.0, 3.0 };
```

我们发现任何数据类型（从内置类型，到 STL 中的自定义类型）都可以用 `{}` 来初始化。这就是初始化一致性。

虽然用法很简单，但是底层实现并没有这么“丝滑”。

```cpp
int value[]{ 1, 2, 3 };
```

编译器看到 `{t1, t2, ... tn}` 便做出一个 `initailizer_list<T>` ，它底层用 `array<T,n>`实现。调用函数如`ctor` 时该 `array` 内的元素可被编译器逐一分解传给函数。

```cpp
// 这个形成 initializer_list<int> 背后有 array<int, 7>
// vector<int> 找到一个接受 initializer_list<int> ctor 。所有容器都有此 ctor
vector<int> v{2, 3, 5, 7, 11, 13, 17};
// 这个行程 initializer_list<string> 背后有 array<string, 3>
vector<string> cities{
    "Shanghai", "Xi'an", "New York"
};
```

我们再来看下一个：

```cpp
complex<double> c{ 4.0, 3.0 }; // equivalent to c(4.0, 3.0)
```

同样的,`initializer_list `后有 `array<double, 2>`  调用 `complex<double> ctor` 时，`array` 内的元素被分解给 `ctor` (`complex<double>` 没有 `ctor` 接受 `initializer_list<double>` )  

```cpp
int i;   // i has undefined value
int j{}; //	j is initialized by 0
int* p;  // p has undefined value
int* q{};// q is initialized by 0
```



#### 5. Alias Template

```cpp
#include<vector>

template<class T>
using Vec = std::vector<T>;

int main(void)
{
	Vec<int> vec;
	// is equivalent to
	std::vector<int> vec;

	return 0;
}
```

It is not possible to partially or explicitly specialize an alias template.

不能偏特化或显式特化别名模板

使用 `#define` 或 `typedef` 都无法达到此效果，因为它们的语法都不接受模板参数。



#### 6. template template parameter

模板的模板参数

```cpp
#include<vector>

template<class T,
	template<class> // 等于 template<class T>
	class Container >
class XCIs
{
private:
	Container<T> c;
public:
};

template<class T>
using Vec = std::vector<T, std::allocator<T>>;

int main(void)	
{
	// 不能直接在定义处写模板，需要使用 alias template
	// XCIs<int, template<class T> std::vector<T>> xcis; 
	XCIs<int, Vec> xcis;

	return 0;
}
```



