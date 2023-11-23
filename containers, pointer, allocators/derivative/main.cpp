#include <iostream>
#include <string>
#include <map>

int get_num(std::string& s, uint& i)
{
    if(i >= s.size())
        return 0;
    int sign = 1;
    int mod = 0;
    if(s[i] == '-')
    {
        ++i;
        sign = -1;
    }
    while(s[i] != 'x' && s[i] != '+' &&
        s[i] != '-' && s[i] != '*')
    {
        mod = 10 * mod + (s[i] - '0');
        ++i;
        if(i == s.size())
            break;
    }
    if(s.size() > i && s[i] == '+')
        ++i;
    if(mod == 0)
        mod = 1;
    return mod * sign;
}

std::string derivative(std::string polynomial) {
    std::map<int, int> p;
    uint i = 0;
    while(i < polynomial.size())
    {
        int a = get_num(polynomial, i);
        int b = 0;
        if(polynomial.size() > i)
        {
            if(polynomial[i] == '*')
            {
                b = 1;
                i += 2;
                if(polynomial.size() > i)
                {
                    if(polynomial[i] == '^')
                    {
                        ++i;
                        b = get_num(polynomial, i);
                    }
                }
            }
            else if(polynomial[i] == 'x')
            {
                ++i;
                b = 1;
                if(polynomial.size() > i)
                {
                    if(polynomial[i] == '^')
                    {
                        ++i;
                        b = get_num(polynomial, i);
                    }
                    else
                        b = 1;
                }
            }
        }
        
        if(p.count(b) == 0)
        {
            p[b] = a;
        }
        else
            p[b] += a;
    }
    std::string res;
    bool is_first = true;
    for(auto it = p.rbegin(); it != p.rend(); ++it)
    {
        if(it->second == 0 || it->first == 0)
            continue;
        if(!is_first)
        {
            if(it->second > 0)
                res += "+";
        }
        else
            is_first = false;
        std::string tmp = "";
        if(it->first == 1)
            tmp = "";
        else if(it->first == 2)
            tmp = "*x";
        else
            tmp = "*x^" + std::to_string(it->first - 1);
        res += std::to_string(it->first * it->second) + tmp;
    }
    return res;
}

int main(int argc, char **argv) {
    std::string s;
    std::cin >> s;
    std::cout << derivative(s) << std::endl;
    return 0;
}