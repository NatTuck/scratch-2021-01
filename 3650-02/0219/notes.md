

# Today: Starting towards Challenge 1

 
## Calculator

Any interpreter, basic flow:

 - Read one line of text from the user
   - Could be "a file"
 - Separate that line into tokens
 - Parse the tokens into an Abstract Syntax Tree (AST)
 - Evaluate the AST
   - => For calc, produces a value
   - => For shell, result in behavior

## Processes in POSIX

Two main API functions:

 - fork - creates a process
   - Copies the current process
   - Call fork once, but it returns twice in
     two different processes.
   - Initial process is parent - fork returns child pid
   - New process is child - fork returns 0
 - exec - runs a program
   - Replaces the running program in the current process
   - Old program is just gone
   - Think a chest burster from "Aliens"

When the system boots up, one "init" process is magically
spawned by the OS kernel. This is pid = 1.

After that, all subsequent processes are created by fork().

## Exec variants

The exec syscall has variants,

 - If the function name ends in l, the command line
   args are passed as args to exec.
 - If it ends in v, they're passed as an array.
 - If it has p, we use the \$PATH variable to find
   the program to run.
 - If there's no p, we need an path to the executable.
 - If there's an e, we add an argument specifying environment
   vars.

When in doubt, you want ```execvp```.

## On Windows

Instead of fork / exec, we have one syscall:

CreateProcess(pathToExecutable)
 
 - Creates a new process
 - Runs a program in it





