#include "lib/bootstraper.h"
#include <csignal>

void signalHandler(int signum){
    cout << "Interrupt signal (" << signum << ") received." << endl;
    system("taskkill /im socket-frontend.exe /t /f");
    
    exit(signum);
}

int main(){
		
	int res = system("./socket-frontend.exe &");
	if(res == 0)
		cout<< "UI app ready!" << endl;
    
    signal(SIGINT, signalHandler);
    init_bootstrap();

    return 0;
}