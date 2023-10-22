#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <sys/select.h>
#include <sys/time.h>

int kbhit() {
    struct timeval tv;
    fd_set fds;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &fds);
}

void enable_raw_mode() {
    struct termios raw;
    tcgetattr(STDIN_FILENO, &raw);
    raw.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}



int main() {
    enable_raw_mode();

    char c;
    while (1) {
        if (kbhit()) {
            c = getchar();
            printf("Tecla pressionada: %c\n", c);
            if (c == 'q') {
                break;
            }
        }
        // Seu código principal pode continuar executando aqui...
    }

    return 0;
}