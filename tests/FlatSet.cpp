
#include "../FlatSet.h"
#include <string>
#include <iostream>

template class FlatSet<int>;

int main() {

	FlatSet<int> set = {
		10, 2, 50
    };

        
    set.insert(1000);
    set.insert(1000);
    
    
    for(auto &&x : set) {
    	std::cout << x << "\n";
    }
    
}
