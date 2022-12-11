#include <string>
#include <iostream>

std::string s(std::string a)
{
    return a;
}

int main()
{
    std::string ss{"ciao ciao"}; 
    auto x = s(ss);
    x[0] = 'p';
    std::cout << ss;
}