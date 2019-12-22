#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdint.h>
#include "forksrv.h"

int main(int argc, char *argv[]) {
  int ctl[2], st[2];
  pipe(ctl); 
  pipe(st);

  pid_t chld = fork();

  if (!chld) {
    /* Child process */
    dup2(ctl[0], FD_CTL_READ_END);
    dup2(st[1], FD_ST_WRITE_END);

    close(ctl[0]); 
    close(ctl[1]);
    close(st[0]);
    close(st[1]);

    char *execve_arg[] = { "./hello", NULL };
    execve(execve_arg[0], execve_arg, NULL);
  }

  /* Parent process */
  close(ctl[0]);
  close(st[1]);

  int32_t status;
  if ((read(st[0], &status, 4)) != 4) {
    fprintf(stderr, "[-] The wrong length of read of sanity check message!\n");
    _exit(1);
  }

  for (int i = 0; i < 10; i++) {
    /* Tell child to fork */
    int32_t prev_timed_out = 0;
    if ((write(ctl[1], &prev_timed_out, 4)) != 4) {
      fprintf(stderr, "[-] Failed to write fork order message!\n");
      _exit(1);
    }
    printf("[+] Wrote fork order message #%d\n", i);

    if ((read(st[0], &chld, 4)) != 4) {
      fprintf(stderr, "[-] Failed to read grdchld PID!\n");
      _exit(1);
    }
    
    if ((read(st[0], &status, 4)) != 4) {
      fprintf(stderr, "[-] Failed to read grdchld process status!\n");
      _exit(1);
    }

    if (WIFEXITED(status))
      printf("[+] Grdchld process exited with status %d\n", WEXITSTATUS(status));

  }
}

