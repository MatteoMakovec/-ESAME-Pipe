#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define BUF_SIZE 1024

int main(int argc, char * argv[]) {
	int pfd[2];
	char buf[BUF_SIZE];

	if (pipe(pfd) == -1) {
		perror("problema con pipe");
		exit(EXIT_FAILURE);
	}

  /*
  *   file handler pipe, estremità di lettura: pfd[0]
  *   file handler pipe, estremità di scrittura: pfd[1]
  */

	switch (fork()) {
		case -1:
			perror("problema con fork");
			exit(EXIT_FAILURE);

		case 0: 
			close(pfd[1]); 

			while (1) {
				int numRead = read(pfd[0], buf, BUF_SIZE);

				if (numRead == -1) {
					perror("errore in read");
					exit(EXIT_FAILURE);
				}

				if (numRead == 0)
					break;

				write(STDOUT_FILENO, buf, numRead);
			}

			close(pfd[0]);

			exit(EXIT_SUCCESS); 

		default:
			close(pfd[0]);

			char * msg = "messaggio per il processo figlio\n";
			int len = strlen(msg);

			write(pfd[1], msg, len); 

			sleep(2);

			write(pfd[1], msg, len);

			sleep(2);

			close(pfd[1]);
			wait(NULL); 

      printf("Fine\n");

			exit(EXIT_SUCCESS);
	}

  return EXIT_SUCCESS;
}


