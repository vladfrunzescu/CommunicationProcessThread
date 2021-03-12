#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char** argv) {
  int n;
  printf("Introduceti numarul: ");
  scanf("%d", &n);

  int a2c[2], c2b[2], b2a[2];
  pipe(a2c), pipe(c2b), pipe(b2a);

  if (fork() == 0) { // proces c
    close(b2a[0]);
    close(b2a[1]);
    close(a2c[1]);
    close(c2b[0]);

    while (n > 0) {
      if (read(a2c[0], &n, sizeof(int)) != sizeof(int)) {
    break;
      }

      if (n > 0) {
    printf("C: %d -> %d\n", n, n + 1);
    n += 1;

    write(c2b[1], &n, sizeof(int));
      }
    }

    close(a2c[0]);
    close(c2b[1]);
    exit(0);
  }


  if (fork() == 0) { // proces b
    close(a2c[0]);
    close(a2c[1]);
    close(c2b[1]);
    close(b2a[0]);

    while (n > 0) {
      if (read(c2b[0], &n, sizeof(int)) != sizeof(int)) {
    break;
      }

      if (n > 0) {
    printf("B: %d -> %d\n", n, n - 2);
    n -= 2;

    write(b2a[1], &n, sizeof(int));
      }
    }

    close(c2b[0]);
    close(b2a[1]);
    exit(0);
  }


  // proces a
  close(c2b[0]);
  close(c2b[1]);
  close(a2c[0]);
  close(b2a[1]);
  
  if (n > 0) {
    printf("A: %d\n", n);
    write(a2c[1], &n, sizeof(int));
  }

  while (n > 0) {
    if (read(b2a[0], &n, sizeof(int)) != sizeof(int)) {
      break;
    }

    if (n > 0) {
      if (n % 2 == 0) {
    printf("A: %d -> %d\n", n, n - 2);
    n -= 2;
      } else {
    printf("A: %d -> %d\n", n, n - 1);
    n -= 1;
      }
      write(a2c[1], &n, sizeof(int));
    }
  }

  close(b2a[0]);
  close(a2c[1]);

  wait(0);
  wait(0);
}
