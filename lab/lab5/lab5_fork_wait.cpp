#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2;

    // ---- Tworzenie pierwszego potomka ----
    pid1 = fork();
    if (pid1 < 0) {
        std::cerr << "Fork 1 failed!\n";
        return 1;
    }

    if (pid1 == 0) {
        // --- Kod pierwszego procesu potomnego ---
        int number;
        int album = 50779; 

        std::cout << "Child 1: Podaj liczbę: ";
        std::cin >> number;

        std::cout << "Child 1: Wynik = " << number + album << std::endl;
        return 0;
    }

    // ---- Tworzenie drugiego potomka ----
    pid2 = fork();
    if (pid2 < 0) {
        std::cerr << "Fork 2 failed!\n";
        return 1;
    }

    if (pid2 == 0) {
        // --- Kod drugiego procesu potomnego ---
        sleep(5);
        std::cout << "Child 2: Hello, Illia semennov!\n";  // <<< wpisz imię
        return 0;
    }

  
    for (int i = 0; i < 2; i++) {
        pid_t ended = wait(NULL);
        std::cout << "Parent: Process " << ended << " finished.\n";
    }

    std::cout << "Parent: Both children finished.\n";
    return 0;
}
