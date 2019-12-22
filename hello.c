#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "forksrv.h"

__attribute__((constructor)) void start_forksrv(void) {
  int32_t tmp = 0;  // the value does not matter

  if (((fcntl(FD_CTL_READ_END, F_GETFD)) < 0) 
     || ((fcntl(FD_ST_WRITE_END, F_GETFD)) < 0)) {
    fprintf(stderr, "[-] FDs have not been opened yet!\n");
    _exit(1);
  }


  if ((write(FD_ST_WRITE_END, &tmp, 4)) != 4) {
    fprintf(stderr, "[-] The wrong length of write of sanity check message!\n");
    _exit(1);
  }

  int32_t was_killed;

  for (;;) {
    if ((read(FD_CTL_READ_END, &was_killed, 4)) != 4) {
      fprintf(stderr, "[-] The wrong length of read of forking order message!\n");
      _exit(1);
    }

    pid_t grdchld;
    if ((grdchld = fork()) < 0) {
      fprintf(stderr, "[-] Failed to fork grdchld!\n");
      _exit(1);
    }

    if (!grdchld) {
      /* (Grand) child process */
      close(FD_CTL_READ_END);
      close(FD_ST_WRITE_END);

      /* Continue execution and will reach main() */
      return;
    }

    /* Parent process */
    if ((write(FD_ST_WRITE_END, &grdchld, 4)) != 4) {
      fprintf(stderr, "[-] Failed to write grdchild pid!\n");
      _exit(1);
    }

    int wstatus;
    if ((waitpid(grdchld, &wstatus, 0)) < 0) {
      fprintf(stderr, "[-] Failed to waitpid grdchld process!\n");
      _exit(1);
    }

    if ((write(FD_ST_WRITE_END, &wstatus, 4)) != 4) {
      fprintf(stderr, "[-] Failed to write a status of grdchld process!\n");
      _exit(1);
    }
  }
}

int main(int argc, char *argv[]) {
  puts("Hello, World!");
  sleep(1); // Prevent a terminal from flooding

  return 0;
}

