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

class C
{
  public:
    explicit C(const string& s);
};

vector<string> vs;
for(const C& elem : vs) // ERROR: no conversion from string to C define
    cout << elem << endl;