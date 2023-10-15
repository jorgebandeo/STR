#include <iostream>
#include <omp.h>
#include <conio.h>

bool hPressed = false;
bool fPressed = true;
bool qPressed = false;

void receiveInput() {
    char input;
    while (!qPressed) {
        if (_kbhit()) {
            input = _getch();
            if (input == 'h' && fPressed) {
                hPressed = true;
                fPressed = false;
            } else if (input == 'f') {
                fPressed = true;
            } else if (input == 'q') {
                qPressed = true;
            }
        }
    }
}

void printOutput() {
    while (!qPressed) {
        if (hPressed) {
            std::cout << "L" << std::endl;
            hPressed = false;
        }
        if (fPressed) {
            // Limpar o terminal (Windows)
            system("cls");
            
        }
    }
}

int main() {
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            receiveInput();
        }

        #pragma omp section
        {
            printOutput();
        }
    }

    return 0;
}
