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

*例 1*

```cpp
int main()
{
	int i;    // i has undefined value
	int j{};  // j is initialized by 0
	int* q;   // q has undefined value
	int* p{}; // p is initialized by 0

	return 0;
}
```



*例 2*

however, that narrowing initializations -- those that reduce precision or where the supplied value gets modified -- are not possible with braces.

```cpp
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
```



*例 3*

```cpp
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
```

`variadic template` 和 `initializer_list` 的 **不同** 是：前者可以接受任意类型的参数，而后者只能接受同一类型的参数。（有点像结构体和数组的关系）



*例 4*

```cpp
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
```

Without the constructor for the `initializer_list`, the **constructor taking two ints would be called to initialize q and s** (`like complex<T>`), while the initialization of r would be invalid.

`initializer_list `源代码：

```cpp
namespace std
{
  /// initializer_list
  template<class _E>
    class initializer_list
    {
    public:
      typedef _E 		value_type;
      typedef const _E& 	reference;
      typedef const _E& 	const_reference;
      typedef size_t 		size_type;
      typedef const _E* 	iterator;
      typedef const _E* 	const_iterator;

    private:
      iterator			_M_array;
      size_type			_M_len;

      // The compiler can call a private constructor.
      constexpr initializer_list(const_iterator __a, size_type __l)
      : _M_array(__a), _M_len(__l) { }

    public:
      constexpr initializer_list() noexcept
      : _M_array(0), _M_len(0) { }

      // Number of elements.
      constexpr size_type
      size() const noexcept { return _M_len; }

      // First element.
      constexpr const_iterator
      begin() const noexcept { return _M_array; }

      // One past the last element.
      constexpr const_iterator
      end() const noexcept { return begin() + size(); }
    };

```

`initializer_list` 底层靠 `array` 实现。

`initializer_list` 只有底层  array 的引用，并不包含 array 中的元素。如果要拷贝这个 `initializer_list` 的话，是 **浅拷贝** 而不是深拷贝。

```cpp
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
```



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



#### 7.explict

##### 单参构造函数

```cpp
struct Complex
{
  int real, imag;

  // non explicit one argument ctor
  Complex(int re, int im = 0)
    :real(re), imag(im)
  {}
  
  Complex operator+(const Complex& x)
  {
    return Complex(real + x.real, imag + x.imag);
  }
};

int main(void)
{
  Complex c1(12, 5);
  // 编译器通过调用 Complex 的单参构造函数将 5 隐式转换为 Complex 类型。进而调用 operator+ 函数
  Complex c2 = c1 + 5;

  return 0;
}
```

如果不希望编译器做这种隐式转换，可以在构造函数前加上 `explicit` 

```cpp
explicit
Complex(int re, int im = 0)
    :real(re), imag(im)
    {}
```

编译时会得到报错：

```
error: no match for ‘operator+’ (operand types are ‘Complex’ and ‘int’)
   Complex c2 = c1 + 5;
```

##### 多参构造函数

```cpp
struct P
{

  explicit P(int a, int b) 
  {
    cout << "P(int a, int b)" << endl;
  }

  //P(initializer_list<int> l)
  //{
  //  cout << "initializer_list<int> l" << endl;
  //}
};

int main(void)
{

  P p1{1, 2};   // error
  P p2 = {1, 2};// error
  P p4(1, 2);   // ok

  return 0;
}
```

初始化时，`{}` 表示的就是 `initializer_list` 。通过 `initializer_list` 隐式调用 `P(int a, int b)` 完成构造。但是此时构造函数前有 `explicit` ，这样就阻止了隐式类型转换。



#### 8.range-based for 

```cpp
#include<vector>
#include<iostream>

using namespace std;

int main()
{
  for(int i : {1, 2, 3, 4, 5, 6})
    cout << i << " ";
  cout << endl;

  vector<double> vec{1.1, 1.2, 1.3};

  for(double d : vec)
    cout << d << " ";
  cout << endl;

  for(auto& d : vec)
    d *= 3;
  for(auto& d : vec)
    cout << d << " ";
  cout << endl;

  return 0;
}
```

输出：

```
1 2 3 4 5 6 
1.1 1.2 1.3 
3.3 3.6 3.9 
```

底层实现：

```cpp
for(decl : coll)
    statement;

for(auto _pos = coll.begin(), _end = coll.end(); _pos != _end; ++_pos)
{
    decl = *_pos;
    statement;
}
或
for(auto _pos = begin(coll), _end = end(coll); _pos != _end; ++_pos)
{
    decl = *_pos;
    statement;
}
```

当与 `explicit` 结合的时候，会产生一种难以发现的错误：

```cpp
class C
{
  public:
    explicit C(const string& s);
};

vector<string> vs;
for(const C& elem : vs) // ERROR: no conversion from string to C define
    cout << elem << endl;
```

这个错误就是因为底层的语句时赋值：

```cpp
const C& elem = *_pos;
```

下面的分析时是猜想：

赋值语句并不等价于直接调用构造函数(`elem(*_pos)`)

首先应该是将 `*_pos` 转化为 C 类型，然后调用拷贝构造函数。只不过编译器会优化成直接调用构造函数。

老师在视频中说的是：“**赋值时，只要类型不一样，就会做转换**”



#### 9.=default，=delete

