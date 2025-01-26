#include <iostream>
#include <termios.h>
#include <csignal>
using std::cout, std::endl, std::signal;

termios terminal_cooked;

int setTerminalRaw(int fildes) {
    termios terminal_raw;

    if(tcgetattr(fildes, &terminal_cooked) < 0) {
        return -1;
    }

    terminal_raw = terminal_cooked;

    cfmakeraw(&terminal_raw);
    // https://www.gnu.org/software/libc/manual/html_node/Noncanonical-Input.html
    terminal_raw.c_cc[VMIN] = 1;
    terminal_raw.c_cc[VTIME] = 0; 

    if(tcsetattr(fildes, TCSAFLUSH, &terminal_raw) < 0) {
        return -1;
    }

    return 0;
}
int setTerminalCooked(int fildes)
{
	if(tcsetattr(fildes, TCSAFLUSH, &terminal_cooked) < 0)
		return(-1);
	return 0;
	
}

void sigcatch(int sig)
{
	setTerminalCooked(0);
	//raise(SIGINT);
}

int main(){
    cout << "im running" << endl;
    signal(SIGINT, sigcatch);
    

    if(setTerminalRaw(0) < 0) {
		cout << ("Can't go to raw mode.\n") << endl; 
		exit(1);
	}
    
    raise(SIGINT);
    
    return 0;
}
