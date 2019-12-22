# The minimal AFL-like fork-server implementation

This is just a very simple example of fork-server **without** any fuzzing components. I just created this to check my understandings.  
Basically, the child process blocks its execution at its constructor (before main()) and run a loop to read messages from parent process. Once read, it forks to clone itself and (grand)child processes continue their executions and eventually reach main().  

# Usage 

```
% make 
% ./forksrv
[+] Wrote fork order message #0
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #1
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #2
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #3
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #4
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #5
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #6
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #7
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #8
Hello, World!
[+] Grdchld process exited with status 0
[+] Wrote fork order message #9
Hello, World!
[+] Grdchld process exited with status 0
[-] The wrong length of read of forking order message!

```
