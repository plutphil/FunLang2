#include"FunLang.hpp"
#include <iostream>
void fl_log(char* str) {
    std::cout << str;
}
void fl_log(const char* str) {
    std::cout << str;
}

int main()
{
    StaticHashMap statmap;
    std::cout << "statmap size " << sizeof(statmap) << "\n";
    std::cout << "int size " << sizeof(int) << "\n";
    std::cout << "ptr size " << sizeof(int*) << "\n";
    statmap_set(&statmap, (char*)"key1", (char*)"hi");
    std::cout << "statmap_get " << statmap_get(&statmap, (char*)"key1") << " \n";
    std::cout << "statmap_get " << (unsigned int)statmap_get(&statmap, (char*)"key2") << " \n";
    std::cout << "statmap_contains " << (unsigned int)statmap_contains(&statmap, (char*)"key1") << " \n";
    std::cout << "statmap_contains " << statmap_contains(&statmap, (char*)"key1")->key << " \n";
    std::cout << "statmap_contains " << statmap_contains(&statmap, (char*)"key1")->val << " \n";
    std::cout << "statmap_contains " << (unsigned int)statmap_contains(&statmap, (char*)"key2") << " \n";

    stringsptr = strings;
    std::cout << "storestring1 " << (unsigned int)storestring("__asdfasdf") << "\n";
    std::cout << "storestring2 " << (unsigned int)storestring("__asdfasdf") << "\n";
    
    test("set('asdf',23);print('test:',get('asdf')); ");
    std::cout << "Hello World!\n";
}
