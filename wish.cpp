// wish.cpp : Defines the entry point for the application.
//
// Requirements:
//Two use methods:
// No argument invokes interactive mode.
//  * This mode is a general use shell mode in which commands can be repeatedly passed and you stay in the Wish shell.
//  * Loop continues indefinitely until user types 'exit'.
// A single .batch file path argument invokes batch mode.
//  * Runs commands inside a single .batch file then exits.

//To execute commands, look into `fork()`, `exec()`, and `wait()/waitpid()`.
//you must use `execv`

//In this project, you should implement `exit`, `cd`, and `path` as built-in commands.
// * `exit`: When the user types `exit`, your shell should simply call the `exit`
// system call with 0 as a parameter.It is an error to pass any arguments to
// `exit`.
//
// * `cd`: `cd` always take one argument(0 or > 1 args should be signaled as an
// 	error).To change directories, use the `chdir()` system call with the argument
// 	supplied by the user; if `chdir` fails, that is also an error.
//
// 	* `path`: The `path` command takes 0 or more arguments, with each argument
// 	separated by whitespace from the others.A typical usage would be like this:
// `wish> path / bin / usr / bin`, which would set `/bin` and `/usr / bin` as the
// search path of the shell.If the user sets path to be empty, then the shell
// should not be able to run any programs(except built - in commands).The
// `path` command always overwrites the old path with the newly specified
// path.

//Your shell will also allow the user to launch parallel commands. This is
// accomplished with the ampersand operator as follows:
	// ```
	// wish> cmd1 & cmd2 args1 args2 & cmd3 args1
	// ```
//Do this using fork() and wait() or waitpid() so that all commannds run in parallel


// Implement '>' charater which writes output to file instead of terminal.
// * If the file already exists, overwrite it.
// Only a single file name argument to the right of '>' should be excepted

//Parallel commands: wish> cmd1 & cmd2 args1 args2 & cmd3 args1
//In this case, instead of running `cmd1` and then waiting for it to finish,
// your shell should run `cmd1`, `cmd2`,and `cmd3` (each with whatever arguments
// 	the user has passed to it) in parallel, * before* waiting for any of them to
// 	complete.
//
// 	Then, after starting all such processes, you must make sure to use `wait()`
// 	(or `waitpid`) to wait for them to complete.After all processes are done,
// 	return control to the user as usual(or , if in batch mode, move on to the next
// 	line).

//Error handling:
//Always print :
	// char error_message[30] = "An error has occurred\n";
	// write(STDERR_FILENO, error_message, strlen(error_message));
//If error is invoking shell with multiple batch files or a bad batch files: exit(1)
//For all other errors: print error message then continue waiting for input


#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <vector>
#include "batchCommand.h"

void runInteractiveMode(void);
void runBatchModeOn(std::string fileName);
void outputError(bool forceExit);

int main(int argc, char *argv[])
{	
	if (argc == 1)
		runInteractiveMode();
	else if (argc > 2)
		outputError(true);
	else 
		runBatchModeOn(argv[1]);
	return 0;
}


void runInteractiveMode(void) 
{
	//TODO: Interactive Mode
}


void runBatchModeOn(std::string fileName)
{
	std::ifstream batchFile;
	batchFile.open(fileName, std::fstream::in);
	wish::BatchCommand *command = NULL;
	if (batchFile.is_open()) 
	{
		for (std::string batchCommandLine; getline(batchFile, batchCommandLine);) 
		{
			command = new wish::BatchCommand(batchCommandLine);
			bool commandSucceeded = command->executeBatchCommand();
			delete(command);
			if (!commandSucceeded)
				outputError(false);
		}
	}
	else 
		outputError(true);
}


void outputError(bool forceErrExit) 
{
	char error_message[30] = "An error has occurred\n";
	write(STDERR_FILENO, error_message, strlen(error_message));

	if (forceErrExit) 
		exit(1);
}