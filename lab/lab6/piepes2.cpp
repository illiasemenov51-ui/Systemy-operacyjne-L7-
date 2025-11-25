#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <string>
#include <array>
#include <algorithm>

std::string pipeRead(int pipe)
{
    std::string message;

    // TODO odczytać komunikat (tekst) z potoku i umieścić go w zmiennej message
    // do czytania z potoku użyć funkcji read
    char buffer[256];
    ssize_t n = read(pipe, buffer, sizeof(buffer) - 1);
    if (n > 0)
    {
        buffer[n] = '\0';
        message = buffer;
    }

    return message;
}

void pipeWrite(int pipe, std::string message)
{
    // TODO zapisać tekst z parametru message (tekst) do potoku
    // 	do zapisu do potoku użyć funkcji write
    // skorzystać z metody message.c_str()

    write(pipe, message.c_str(), message.size());
}

std::vector<std::array<int, 2>> createPipes(int count)
{
    std::vector<std::array<int, 2>> pipes;

    for (int i = 0; i < count; i++)
    {
        std::array<int, 2> pipe_fd;

        if (pipe(pipe_fd.data()) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pipes.push_back(pipe_fd);
    }

    return pipes;
}

int main()
{
    auto pipes = createPipes(1);

    if (fork() == 0)
    {
        // child
        close(pipes[0][0]); // zamknąć koniec do czytania

        std::string message = "komunikat przesłany przez potok";
        pipeWrite(pipes[0][1], message);

        close(pipes[0][1]);
    }
    else
    {
        // parent
        close(pipes[0][1]); // zamknąć koniec do pisania

        std::string received = pipeRead(pipes[0][0]);
        std::cout << "Rodzic otrzymał: " << received << std::endl;

        close(pipes[0][0]);
        wait(NULL);
    }

    return 0;
}
