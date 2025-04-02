#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

// Fonction pour exécuter une commande externe
void execute_command(char **args) 
{
	pid_t pid = fork(); // Création d'un processus enfant
	if (pid == -1)
	{
		perror("Erreur lors du fork");
		return;
	}

	if (pid == 0) { // Processus enfant
		// Exemple avec execv (remplacer par le chemin approprié)
		char *path = "/bin/echo"; // Chemin complet requis
		execv(path, args);
		perror("Erreur lors de l'exécution de la commande");
		exit(EXIT_FAILURE);
	} else { // Processus parent
		int status;
		waitpid(pid, &status, 0); // Attente de la fin du processus enfant
		printf("Commande terminée avec le statut : %d\n", WEXITSTATUS(status));
	}
}

// Fonction principale du mini-shell
int main() {
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
		printf("$> "); // Affichage du prompt

		// Lecture de la commande utilisateur
		if (getline(&buffer, &buf_size, stdin) == -1) {
			perror("Erreur lors de la lecture");
			free(buffer);
			return EXIT_FAILURE;
		}

		// Suppression du retour à la ligne à la fin de la commande
		buffer[strcspn(buffer, "\n")] = '\0';

		// Vérification si la commande est vide
		if (strlen(buffer) == 0) {
			continue;
		}

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
			continue;
		}

		// Exécution des commandes externes avec execv
		execute_command(args);
	}

	free(buffer); // Libération de la mémoire avant de quitter
	return EXIT_SUCCESS;
}
