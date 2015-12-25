/* T2.CPP - Test Harness for State.CPP, and also 
** the Second Series of Terminators (T-1000, etc.)
** p.s. just kidding, I do not work for Skynet/Cyberdyne/
** Asuran Asshole Agency, anymore.
*/

#include <iostream>
#include <string>

#include "state.hpp"



int main() {
	cout << "Testing state thingy, creation:" << endl;
	
	SaveState T;
	
	T.open("test1000.txt");
	
	T.close()
	
	cout << "What a boring test." << endl;
	
	return 0
}


