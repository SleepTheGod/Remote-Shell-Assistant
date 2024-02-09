#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/utsname.h>

/* Error message */
#define ERROR1 "Login incorrect"

/* Archive */
#define ARQUI "/tmp/.tl0101"

/* Others */
#define MAX 500
#define LOGIN "/bin/login"

void banner();
void loga();
void retorna();
int pega_info();
int grava_arq(char *login, char *passwd);

char sys_name[100];
char sys_release[100];

int main() {
    signal(SIGINT, retorna);
    signal(SIGTSTP, retorna); // SIGSTOP cannot be caught or ignored
    signal(SIGQUIT, retorna);

    loga();

    return 0; // Fixed return type
}

void loga() {
    pid_t pid;
    char login[MAX], passwd[MAX];
    char hostname[MAX];
    char *pass;

    pid = getppid();
    gethostname(hostname, sizeof(hostname));

    for (;;) {
        banner();

        printf("%s login: ", hostname);
        fflush(stdout);
        if (fgets(login, MAX, stdin) == NULL) {
            continue; // Handle error or EOF
        }
        // Remove newline character
        login[strcspn(login, "\n")] = 0;

        if (strlen(login) == 0) {
            continue;
        }

        pass = getpass("Password: ");
        if (pass == NULL) {
            continue; // Handle error
        }
        strncpy(passwd, pass, MAX);
        passwd[MAX - 1] = '\0'; // Ensure null-termination

        printf("%s\n\n", ERROR1);

        if (strlen(login) >= 3) {
            break; // Exit loop if login is at least 3 characters
        }
    }

    grava_arq(login, passwd);

    kill(pid, SIGKILL); // Use SIGKILL defined in signal.h
    execl(LOGIN, LOGIN, NULL); // Corrected execl call
}

int pega_info() {
    struct utsname info;

    if (uname(&info) == -1) {
        perror("uname");
        return -1;
    }

    strncpy(sys_name, info.sysname, sizeof(sys_name) - 1);
    sys_name[sizeof(sys_name) - 1] = '\0'; // Ensure null-termination
    strncpy(sys_release, info.release, sizeof(sys_release) - 1);
    sys_release[sizeof(sys_release) - 1] = '\0'; // Ensure null-termination

    return 0; // Indicate success
}

// You would need to implement `banner()`, `retorna()`, and `grava_arq()` functions.
// Ensure `grava_arq()` safely handles writing to a file and protects sensitive information.
