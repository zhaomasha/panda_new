#include <iostream>
#include <cassert>
#include <string>
#include "boost/regex.hpp"
int main(){
    boost::regex reg("//d{3}([a-zA-Z]+).(//d{2}|N/A)//s//1");

    std::string correct="123Hello N/A Hello";
    std::string incorrect="123Hello 12 hello";
    //assert(boost::regex_match(correct,reg)==true);
    //assert(boost::regex_match(incorrect,reg)==false);
    return 0;
}
