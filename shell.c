#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/wait.h>
#define max_args 13 
#define maxln_Com_Amb 105 


char comando [maxln_Com_Amb];
char *args [max_args]; 
int cantcmd = 0 ; 

char SHELL [maxln_Com_Amb]; 
char PATH [maxln_Com_Amb]; 
char HOME [maxln_Com_Amb]; 
char PWD [maxln_Com_Amb] ;

void separaArgs (void); 


/*estas son las funciones a ejecutar */ 
void execute_cd (void ); 
void execute_exit (void); 
void execute_dir_or_ls(void);
void execute_pwd(void) ; 
void execute_echo(void);
void execute_clear(void); 
/*----------------------------------*/

void listaDir (void); 
void eco (void) ; 
void comExterno(void); 


/*estas son los flags del programa */ 
int continuar = 1;
/*---------------------------------*/


int main(int argc, char const *argv[])
{
	 

	getcwd (PWD,maxln_Com_Amb); 
	strcpy(PATH,getenv("PATH")); 
	//printf("%s\n", PATH);

	strcpy(HOME,PWD);
	strcpy(SHELL,PWD); 


	
	do {
		printf("%s>",PWD );
		__fpurge(stdin);

		memset(comando,'\0',maxln_Com_Amb);
		scanf("%[^\n]s" ,comando) ; 

		if (strlen(comando)>0) {
			separaArgs(); 

			if (strcmp (comando, "cd")==0 )execute_cd();
			else if (strcmp(comando,"exit")==0)execute_exit();
			else if (strcmp(comando,"dir") ==0 || strcmp(comando, "ls")==0 )execute_dir_or_ls(); 
			else if (strcmp(comando,"pwd") ==0 ) execute_pwd () ; 
			else if (strcmp(comando,"echo")==0 ) execute_echo() ; 
			else if (strcmp(comando,"clear")==0) execute_clear();
			else printf("%s\n", "No se reconoce como un comando interno ");
		}		
	}while(continuar);


	

	return 0;
}


void separaArgs (void){
	int i ; 
	for (i=0 ; i < max_args-1; i++) args[i]= NULL; 
	strtok(comando , " ") ; 
	i=0 ; 
	args[i] =comando ;
	while((args[++i]= strtok(NULL," "))!=NULL&& i <(max_args-2)); 
	cantcmd = i ; 
}




void execute_cd  (void ){
	if (args[1])
		if (chdir(args[1])!=0)
			printf("Error '%s'  no existe o np se puede cambiar a este directorio\n", args[1]);
		else getcwd(PWD,maxln_Com_Amb); 
}

void execute_exit (void){
	continuar = 0; 
}

void execute_dir_or_ls(void){
	char ruta [maxln_Com_Amb] ; 
	int archs ; 
	int cnt = -1; 
	struct dirent **lista ; 
	strcpy (ruta,PWD); 
	if (args[1]) strcat (ruta , "/"),strcat (ruta ,args[1]) ; 
	archs = scandir(ruta , &lista ,0,alphasort) ; 
	if (archs <0 )
		printf("Error , no existe o no se puede leer [%s]\n",ruta);
	else if (archs == 2 ){
		//solo encontro los directorios [ . , .. ] 
	}
	else {
		while(++cnt < archs ){
			if (strcmp(lista[cnt]->d_name, ".")!=0 && strcmp(lista[cnt]->d_name, "..")!=0)
				printf("%s\n", lista[cnt]->d_name);
		}
	}
}

void execute_pwd (void) {
	printf("%s\n", PWD);
}

void execute_echo (void){
	
	/*
	int  i =0 , maxsize = 64 ;
    char c, * buf ;

	//esto es para hacerle echo a todo lo que se pone en la entrada estandar
    if (argc == 1 ) {

        for(;;) {
            buf = calloc(maxsize, sizeof(char));
            while (read(STDIN_FILENO,&c,1)>0&&c!='\n'){
                buf[i++] = c ;
                if (i==maxsize-1){
                    maxsize*=2;
                    if (!(buf=realloc(buf,maxsize))){
                        printf("Error while allocating memory\n");
                        exit(1);
                    }
                }
            }
            buf[i] = '\0';
            maxsize=64 ;
            i=0;
            printf("%s\n",buf);
            free(buf);
        }
    }
    */


	if (!args[1]) 
		return; 
	int i,j,k =0;
	char aux [6]; 



	while(args[++k]){
		for (i=0 ; i<strlen(args[k]); i++){
			if (args[k][i]!= '$')printf("%c", args[k][i]);
			else {
				j=-1;
				while(++j<5&&strlen(args[k]) && args[k][i+j+1]!='\0')
					aux [j] = args[k][i+j+1] ; 
				aux[j] = '\0';
				if (strcmp(aux,"SHELL") ==0 )
					printf("%s\n", SHELL) , i+=5;
				else if (strncmp(aux,"PATH",4)==0)
					printf("%s\n", PATH), i+=4;
				else if (strncmp(aux , "PWD",3)==0)
					printf("%s\n", PWD) , i+=3;
				else if (strncmp(aux , "HOME",4) == 0 )
					printf("%s\n", HOME ), i+=4;
				else printf("%s\n", "$");
			}
		}
		printf(" ");
	} 
	printf("\n");
}

void execute_clear(void){
	int pid =0, status ; 
	pid = fork();
	if (pid<0)printf("%s\n", "Error no se pudo crear un proceso hijo");
	if (pid==0){
		status = execvp("clear" , args) ; 
		if(status){
			printf("Error!%s no se reconoce o no se puede ejcutar \n", comando );
			exit(1) ;
		}		
	}
	else {
		wait(NULL) ; 
	}	
}