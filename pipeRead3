#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>

bool pipeRead(int pipe, int& value)
{
    int readed = 0;
    readed = read(pipe, &value, sizeof(value));
    if (readed == -1)
    {
        perror("Error reading pipe.");
        exit(EXIT_FAILURE);
    }
    else if (readed == 0)
        return false;
    return true;
}

void pipeWrite(int pipe, int value)
{
    if (write(pipe, &value, sizeof(value)) == -1)
    {
        perror("Error writing pipe.");
        exit(EXIT_FAILURE);
    }
}

std::vector<std::array<int, 2>> createPipes(int count)
{
    std::vector<std::array<int, 2>> pipes;
    for (int i = 0; i < count; ++i)
    {
        std::array<int, 2> newPipe;
        if (pipe(newPipe.data()) == -1)
        {
            perror("Error creating pipe.");
            exit(EXIT_FAILURE);
        }
        pipes.push_back(newPipe);
    }
    return pipes;
}


int main(int argc, char* argv[])
{
    auto pipes = createPipes(2);  // pipes[0] child1->child2, pipes[1] child2->parent

    pid_t child1 = fork();

    if (child1 == 0)
    {
        // -------------------- PIERWSZY POTOMEK --------------------
        // czyta liczby od użytkownika i wysyła do potoku 0

        close(pipes[0][0]); // nie czyta
        close(pipes[1][0]);
        close(pipes[1][1]);

        int value;
        do {
            std::cin >> value;
            if (value > 0)
                pipeWrite(pipes[0][1], value);
        } while (value > 0);

        close(pipes[0][1]);
        return 0;
    }

    // Tworzymy DRUGIE dziecko (pośrednik)
    pid_t child2 = fork();

    if (child2 == 0)
    {
        // -------------------- DRUGI POTOMEK – POŚREDNIK --------------------
        close(pipes[0][1]); // czyta z pipes[0][0]
        close(pipes[1][0]); // pisze do pipes[1][1]

        int value;
        long sum = 0;
        int count = 0;

        // Odczytuje wszystkie liczby
        while (pipeRead(pipes[0][0], value))
        {
            sum += value;
            count++;

            pipeWrite(pipes[1][1], value);  // przekazuje dalej do rodzica
        }

        // wyswietlenie średniej przez proces pośredniczący
        if (count > 0)
        {
            double avg = (double)sum / count;
            std::cout << "(Child2:" << getpid() << ") Średnia = " << avg << "\n";
        }

        close(pipes[0][0]);
        close(pipes[1][1]);
        return 0;
    }

    // -------------------- RODZIC --------------------
    close(pipes[0][0]);
    close(pipes[0][1]);
    close(pipes[1][1]); // tylko czyta

    int value;
    while (pipeRead(pipes[1][0], value))
        std::cout << "(Parent:" << getpid() << "): " << value << "\n";

    close(pipes[1][0]);

    wait(nullptr);
    wait(nullptr);

    return 0;
}
