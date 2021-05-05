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