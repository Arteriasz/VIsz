#include <iostream>
#include <termios.h>
#include <csignal>
#include <unistd.h>
using std::cout, std::endl, std::signal;

termios terminal_cooked;
// https://man7.org/linux/man-pages/man3/termios.3.html

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
    cout << "pegou sginak" << sig << endl;
	setTerminalCooked(0);
	exit(0);
}

int main(){
    int i;
    char c;

    if(signal(SIGINT,sigcatch) < 0)
	{
		perror("signal");
		exit(1);
	}
    
    if(setTerminalRaw(0) < 0) {
		cout << ("Can't go to raw mode.\n") << endl; 
		exit(1);
	}

    while( (i = read(0, &c, 1)) == 1)
	{
        if(c == 3) {
            std::raise(SIGINT);
        }

		printf( "%o\n\r", c);
	}
    
    return 0;
}
