//Experimentsl shell in c(functions like exit, cd etc..)


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define LSH_RL_BUFSIZE 1024
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"

void lsh_loop();
char* lsh_read_line(void);
char** lsh_split_line(char*);
int lsh_execute();

int main(int argc, char **argv){
	lsh_loop();
	return 0;
}

void lsh_loop(){
	char *line;
	char **args;
	int status;
		printf("Hello here!\n");
		printf("Welcome to my shell\n");
		printf("Have fun!!\n");
	do{
		printf(">>> ");
		line = lsh_read_line();
		args = lsh_split_line(line);
		status = lsh_execute(args);

		free(line);
		free(args);

	}while(status);
}

char *lsh_read_line(void){
	int bufsize = LSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(bufsize*sizeof(char));
	int c;

	if(!buffer){
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		c = getchar();
		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		position++;
	
		if(position >= bufsize){
			bufsize += LSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if(!bufsize){
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


// char *lsh_read_line(void){
// 	char *line = NULL;
// 	ssize_t bufsize = 0;

// 	if(getline(&line, &bufsize, stdin) == -1){
// 		if(feof(stdin)){
// 			exit(EXIT_SUCCESS);
// 		}
// 		else{
// 			perror("readline");
// 			exit(EXIT_FAILURE);
// 		}
// 	}
// 	return line;
// }


char **lsh_split_line(char *line){
	int bufsize = LSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!token){
		fprintf(stderr, "las: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, LSH_TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;

		if(position >= bufsize){
			bufsize += LSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize*sizeof(char*));
			if(!token){
				fprintf(stderr, "las: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, LSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}

int lsh_launch(char **args){
	pid_t pid, wpid;
	int status;

	pid = fork();

	if(pid == 0){
		if(execvp(args[0], args) == -1){
			perror("lsh");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid<0){
		perror("lsh");
	}
	else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return -1;
}


int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};


int (*builtin_func[])(char **) = {
	&lsh_cd,
	&lsh_help,
	&lsh_exit
};


int lsh_num_builtins(){
	return sizeof(builtin_str)/sizeof(char*);
}


int lsh_cd(char **args){
	if(args[1] == NULL){
		fprintf(stderr, "lsh: expected argument to \"cd\"\n");
	}
	else{
		if(chdir(args[1]) != 0){
			perror("lsh");
		}
	}
	return 1;
}


int lsh_help(char **args){
	int i;
	printf("Stephen Brenan LSH\n");
	printf("Type program namnes and arguements, and hit enter. \n");
	printf("The following are built in : \n");

	for(i = 0; i< lsh_num_builtins(); i++){
		printf(" %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs. \n");
	return 1;
}


int lsh_exit(char **args){
	return 0;
}


int lsh_execute(char **args){
	int i;
	if(args[0] == NULL){
		return 1;
	}

	for(i=0; i< lsh_num_builtins() ; i++){
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
		}
	}

	return lsh_launch(args);
}



