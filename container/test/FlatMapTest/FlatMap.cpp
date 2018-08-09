#include <eteran/cpp-utilities/FlatMap.h>
#include <string>
#include <iostream>

template class FlatMap<int, std::string>;

int main() {
	FlatMap<int, std::string> map = {
		std::make_pair(1, "one"),
        std::make_pair(2, "two"),
    };
    
	map[10]  = "ten";
    map[5]   = "five";
    map[50]  = "fifty";
    map[50]  = "fifty!";
    
    std::cout << map.at(10) << std::endl;
    
    map.emplace(100, "one hundred");
    map.insert(std::make_pair(1000, "one thousand"));
    
    
    for(auto &&x : map) {
    	//x.first = 1000;
    	std::cout << x.first << " : " << x.second << "\n";
    }
    
}
