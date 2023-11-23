#include <iostream>
#include <string>

class StringPointer {
private:
    std::string* pointer;
    bool iCreate = false;
public:
    std::string *operator->() {
        if(!pointer)
        {
            iCreate = true;
            pointer = new std::string("");
        }
        return pointer;
    }
    operator std::string*() {
        if(!pointer)
        {
            iCreate = true;
            pointer = new std::string("");
        }
        return pointer;
    }
    StringPointer(std::string *Pointer) : pointer(Pointer) {}
    ~StringPointer() {
        if(iCreate)
            delete pointer;
    }
};

int main(int argc, char **argv) {
    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    return 0;
}