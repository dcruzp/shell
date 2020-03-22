#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#define max_args 13
#define maxln_Com_Amb 105

char comando[maxln_Com_Amb];
char *args[max_args];
int cantarg = 0;

char SHELL[maxln_Com_Amb];
char PATH[maxln_Com_Amb];
char HOME[maxln_Com_Amb];
char PWD[maxln_Com_Amb];

char *historial [55] ; 
char * directorio;
int h_head , h_tail;  


typedef struct command
{
	char **args; 
	char **redir; 
}cmd;


void separaArgs(void);
//void load_history(void); 

/*estas son las funciones a ejecutar */
void execute_cd(void);
void execute_exit(void);
void execute_dir_or_ls(void);
void execute_pwd(void);
void execute_echo(void);
void execute_clear(void);
/*----------------------------------*/

void listaDir(void);
void eco(void);
void comExterno(void);
void Print(cmd ** c); 
void loadhistory(void) ;


/*estas son los flags del programa */
int continuar = 1;
/*---------------------------------*/

int main(int argc, char const *argv[])
{

	getcwd(PWD, maxln_Com_Amb);
	strcpy(PATH, getenv("PATH"));
	//printf("%s\n", PATH);

	strcpy(HOME, PWD);
	strcpy(SHELL, PWD);

	//loadhistory();

	do
	{
		// Print beggining of shell
		printf("%s>", PWD);

		// Read input
		__fpurge(stdin);
		memset(comando, '\0', maxln_Com_Amb);
		scanf("%[^\n]s", comando);

		// React to input
		if (strlen(comando) > 0)
		{
			separaArgs();

			if (strcmp(comando, "cd") == 0)
				execute_cd();
			else if (strcmp(comando, "exit") == 0)
				execute_exit();
			else if (strcmp(comando, "dir") == 0 || strcmp(comando, "ls") == 0)
				execute_dir_or_ls();
			else if (strcmp(comando, "pwd") == 0)
				execute_pwd();
			else if (strcmp(comando, "echo") == 0)
				execute_echo();
			else if (strcmp(comando, "clear") == 0)
				execute_clear();
			else
				printf("%s\n", "No se reconoce como un comando interno ");
			
		}
	} while (continuar);

	return 0;
}

void separaArgs(void)
{
	int i;
	for (i = 0; i < max_args - 1; i++)args[i] = NULL;
	
	i = 0;
	args[i] = strtok(comando, " ");
	if(!args[i])return;
	
	while ((args[++i] = strtok(NULL, " ")) != NULL && i < (max_args - 1));
	cantarg = i;
}

void execute_cd(void)
{
	if (args[1])
		if (chdir(args[1]) != 0)
			printf("Error '%s'  no existe o np se puede cambiar a este directorio\n", args[1]);
		else
			getcwd(PWD, maxln_Com_Amb);
}

void execute_exit(void)
{
	continuar = 0;
	//savehistorial();
}

void execute_dir_or_ls(void)
{
	char ruta[maxln_Com_Amb];
	int archs, cnt = -1;;
	struct dirent **lista;
	strcpy(ruta, PWD);
	if (args[1])
		strcat(ruta, "/"), strcat(ruta, args[1]);
	archs = scandir(ruta, &lista, 0, NULL);
	if (archs < 0)
		printf("Error , no existe o no se puede leer [%s]\n", ruta);
	else if (archs == 2)
	{
		//solo encontro los directorios [ . , .. ]
	}
	else
	{
		while (++cnt < archs)
		{
			if (strncmp(lista[cnt]->d_name, "." ,1)!=0)
				printf("%s\n", lista[cnt]->d_name);
			/*if (strcmp(lista[cnt]->d_name, ".") != 0 && strcmp(lista[cnt]->d_name, "..") != 0)
				printf("%s\n", lista[cnt]->d_name);*/
			if (args[1] !=NULL)printf("%s\n" , args[1]);

		}
	}
}

void execute_pwd(void)
{
	printf("%s\n", PWD);
}

void execute_echo(void)
{
	if (!args[1])
		return;
	int i, j, k = 0;
	char aux[6];

	while (args[++k])
	{
		for (i = 0; i < strlen(args[k]); i++)
		{
			if (args[k][i] != '$')
				printf("%c", args[k][i]);
			else
			{
				j = -1;
				while (++j < 5 && strlen(args[k]) && args[k][i + j + 1] != '\0')
					aux[j] = args[k][i + j + 1];
				aux[j] = '\0';
				if (strcmp(aux, "SHELL") == 0)
					printf("%s\n", SHELL), i += 5;
				else if (strncmp(aux, "PATH", 4) == 0)
					printf("%s\n", PATH), i += 4;
				else if (strncmp(aux, "PWD", 3) == 0)
					printf("%s\n", PWD), i += 3;
				else if (strncmp(aux, "HOME", 4) == 0)
					printf("%s\n", HOME), i += 4;
				else
					printf("%s\n", "$");
			}
		}
		printf(" ");
	}
	printf("\n");
}

void execute_clear(void)
{
	int pid = 0, status;
	pid = fork();
	if (pid < 0)
		printf("%s\n", "Error no se pudo crear un proceso hijo");
	if (pid == 0)
	{
		status = execvp("clear", args);
		if (status)
		{
			printf("Error!%s no se reconoce o no se puede ejcutar \n", comando);
			exit(1);
		}
	}
	else
	{
		wait(NULL);
	}
}

void loadhistory(void){
	int i =0 ; 
	char * in =NULL; 
	historial [i] = NULL ; 
	historial [i] = (char*) malloc(sizeof(char)*1024) ; 
	FILE *f , *f1 ; 
	f = fopen(".history" , "r");
	f1= fopen(".ptr_HISTORY", "r"); 
	fscanf(f1,"%d %d" , &h_head , &h_tail) ; 
	int read , len , n_ptr = h_head; 
	while ((read = getline(&historial[n_ptr],&len , f))!=-1)
	{
		if (n_ptr == h_tail) break ; 
		if (read <=1 )break ; 
		n_ptr = (n_ptr+1)%50; 
	}
	
}

void savehistorial (void ){
	FILE *f , *f1 ; 
	char *aux1 = NULL ;
	aux1 = (char*) malloc (sizeof(char)*1024) ; 
	strcpy(aux1, directorio) ; 
	strcat(aux1 , "/.ptr_HISTORY"); 

	f1 = fopen(aux1, "w"); 

	fprintf(f1,"%d %d" , h_head , h_tail) ; 

	aux1 = NULL ; 
	aux1 = (char*) malloc (sizeof(char)*1024); 

	strcpy(aux1, directorio);
	strcat(aux1, "/.history");

	f= fopen(aux1 , "w"); 

	if (f == NULL) {
		printf("%s\n", "Missing file error. '.history' does not exist"); 
	}
	int i , j, k , p = 0 , first =1 , n_ptr = h_head ; 
	for (i=h_head;; i= (i+1)%50){
		fprintf(f,"%s", historial[i]);
		if (i==h_tail) break; 
	}
}

void historial_Add (cmd **c ){

}

void Print (cmd ** c ){
	int i , j ; 
	for (i = 0; c[i] != NULL; i++)
	{
		for (j=0 ; c[i] ->args[j] !=NULL; j++)
			printf("%s" ,c[i] ->args[j]) ;

		for (j=0 ; c[i]->redir[j] !=NULL ; j++)
			printf ("%s" , c[i] ->redir[j]) ;
		printf("\n"); 	
	}
	printf("print end");
	
}