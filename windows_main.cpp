#include <iostream>

using namespace std;

int main(){
	
	system("START /B socket-frontend.exe > .elct");
	cout << "UI ready..." << endl;
	system("START /B main.exe > log.txt");
	
	return 0;
} // windows mingw