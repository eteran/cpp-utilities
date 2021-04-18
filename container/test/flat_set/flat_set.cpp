#include <cpp-utilities/flat_set.h>
#include <string>
#include <iostream>

template class flat_set<int>;

int main() {

	flat_set<int> set = {
		10, 2, 50
    };

        
    set.insert(1000);
    set.insert(1000);
    
    
    for(auto &&x : set) {
    	std::cout << x << "\n";
    }
    
}
