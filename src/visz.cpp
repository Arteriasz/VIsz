#include <iostream>
#include <termios.h>
#include <csignal>
#include <unistd.h>
#include <sys/ioctl.h>
using std::cout, std::endl, std::signal, std::raise, std::cin;

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
int setTerminalCooked(int fildes) {

    cout << "\033[2J\033[H"; //https://student.cs.uwaterloo.ca/~cs452/terminal.html

    if(tcsetattr(fildes, TCSAFLUSH, &terminal_cooked) < 0)
        return(-1);
        
    return 0;
    
}

void sigcatch(int sig) {
    cout << "signal received " << sig << endl;
    setTerminalCooked(0);
    exit(0);
}

int getTerminalSize(winsize sz){
    //get terminal size https://stackoverflow.com/questions/1022957/getting-terminal-width-in-c/1022961#1022961
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &sz) == -1) {
        std::cerr << "Erro ao obter o tamanho do terminal" << std::endl;
        return 1;
    }

    return 0;
}

int main() {
    winsize winsz;

    if(getTerminalSize(winsz) > 0) {
        raise(SIGINT);
    }
    bool control = true;	
    char input = '\0';
    int i = winsz.ws_row-1;
    
    signal(SIGINT, sigcatch);
    
    if(setTerminalRaw(0) < 0) {
        cout << ("Can't go to raw mode.\n") << endl; 
        exit(1);
    }

    cout << "\033[2J\033[" << i << 'd';

    while(i > 0){
        cout << "SZ~\033[A\033[G";
        i--;
    }

    while(cin.read(&input, 1)){
        if(input == 3) {
            raise(SIGINT);
        }
        if(input == 105){ //Received char 'i' 
            control = false;
        } else if(input == 27) { //Received char 'ESC'
            control = true;
        }
        if (control) {
            if(input == 119){ //Received char 'w'
                cout << "\033[A";
            } else if(input == 97) { //Received char 'a'
                cout << "\033[D";
            } else if (input == 115) { //Received char 's'
                cout << "\033[B";
            } else if(input == 100) { //Received char 'd'
                cout << ("\033[C");
            }
        }
    }
    
    return 0;
}
