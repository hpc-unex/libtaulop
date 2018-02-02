#include <algorithm>
#include <string>

#include <iostream>
using namespace std;


int main() {
  std::string s = "example string";
  string news = s;
  std::replace( news.begin(), news.end(), 'x', 'y'); // replace all 'x' to 'y'

  cout << s << endl;
  cout << news << endl;
}
