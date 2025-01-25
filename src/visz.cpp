#include <iostream>
#include <termios.h>
#include <csignal>

using std::cout, std::endl;

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

int main(){
    cout << "im running" << endl;

    if(setTerminalRaw(0) < 0)
	{
		fprintf(stderr,"Can't go to raw mode.\n");
		exit(1);
	}
    return 0;
}