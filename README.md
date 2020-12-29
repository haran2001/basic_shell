# basic_shell
Shell from scratch in C 
	-> Shell is a command line interface that is used to interact with the underlying kernel(like the terminal in linux)</br>
	-> I built my own shell that has 3 commands help, list and exit.</br>
	-> The program works by taking a command as input (using lsh_read_line function)</br>
	-> Splits the command into individual arguments(using lsh_split function) </br>
	-> Finally executes each argument/command of the input (using lsh_execute function) </br> 
	

Reference tutorial: https://brennan.io/2015/01/16/write-a-shell-in-c/
