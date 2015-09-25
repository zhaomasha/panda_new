#include<iostream>
#include<boost/lexical_cast.hpp>
using namespace boost;
int main()
{
   int a = lexical_cast<int>("123456");
   std::cout << a <<std::endl;
   return 0;
}
