#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>

# include "bastien.h"

void execute_command(char **args) 
{
	pid_t pid = fork();
	if (pid == -1)
	{
		perror("Erreur lors du fork");
		return;
	}

	if (pid == 0) 
    {
		char *path = "/bin/echo"; // Chemin complet requis
		execv(path, args);
		perror("Erreur lors de l'exécution de la commande");
		exit(EXIT_FAILURE);
	} 
    else 
    { // Processus parent
		int status;
		waitpid(pid, &status, 0); // Attente de la fin du processus enfant
		printf("Commande terminée avec le statut : %d\n", WEXITSTATUS(status));
	}
}

// Fonction principale du mini-shell
/* int main() {
	char *buffer = NULL;
	size_t buf_size = 1024;
	char *args[128];
	char *token;

	buffer = (char *)malloc(buf_size); // Allocation du buffer
	if (!buffer) {
		perror("Erreur d'allocation mémoire");
		return EXIT_FAILURE;
	}

	while (1) {
        // Utilisation de readline pour lire la commande de l'utilisateur
        buffer = readline("$> ");  // Affichage du prompt

        if (buffer == NULL) {
            perror("Erreur lors de la lecture");
            return EXIT_FAILURE;
        }

		

        // Suppression du retour à la ligne à la fin de la commande
        if (strlen(buffer) == 0) {
            free(buffer);
            continue;
        }

        // Ajout de l'entrée dans l'historique
        //add_history(buffer);

        // Découpage de la commande en arguments
        int i = 0;
        token = strtok(buffer, " ");
        while (token != NULL) {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Terminer le tableau des arguments par NULL

        // Commande interne "exit"
        if (strcmp(args[0], "exit") == 0) {
            printf("Bye!\n");
            free(buffer);
            break;
        }

        // Commande interne "cd"
        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "cd : argument manquant\n");
            } else {
                if (chdir(args[1]) != 0) {
                    perror("Erreur lors du changement de répertoire");
                }
            }
            free(buffer);
            continue;
        }

        // Exécution des commandes externes avec execv
        execute_command(args);

        free(buffer);  // Libérer la mémoire après chaque boucle
    }

    return EXIT_SUCCESS;
}
 */