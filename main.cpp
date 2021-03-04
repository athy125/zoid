#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>

// Terminal color codes.
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

// Replaces all occurence of a substring by another substring.
std::string replace_all(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;

    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }

    return str;
}

// Launches an application by forking the current process and changing the command.
void launch(char** args, std::string input, std::string output) {
    pid_t pid, wpid;
    int status;
    pid = fork();

    // The forked process ends up here.
    if (pid == 0) {
        if (input != "") {
            freopen(input.c_str(), "r", stdin);
        }

        if (output != "") {
            freopen(output.c_str(), "w", stdout);
        }

        if (execvp(args[0], args) == -1) {
            perror("mussels");
            _exit(EXIT_FAILURE);
        }
    // An error occured with the fork.
    } else if (pid < 0) {
        perror("mussels");
    // The parent process ends up here.
    } else {
        // Waits for the forked process to finish, if not in background.
        if (output != "/dev/null") {
            do {
                wpid = waitpid(pid, &status, WUNTRACED);
            } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        }
    }
}
