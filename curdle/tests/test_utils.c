
//////////////////////////
// DO NOT SUBMIT THIS FILE
//////////////////////////


// for memfd_create
#define _GNU_SOURCE

#include <sys/types.h>
#include "test_utils.h"
#include "adjust_score.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// for fcntl
#include <unistd.h>
#include <fcntl.h>

// for memfd_create
#include <sys/mman.h>

// be verbose
//#define DEBUG

void* random_contents(size_t size) {
  const char allowable_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789";

  const size_t arr_len = sizeof(allowable_chars)/sizeof(char);

  // only initialize srand once
  static int has_been_initialized = 0;
  if (!has_been_initialized) {
    srand(time(NULL));
    has_been_initialized = 1;
  }

  char *conts = malloc(size);

  for(size_t i = 0; i < size; i++) {
    int c_pos = rand() % arr_len;
    conts[i]  = allowable_chars[c_pos];
  }

  return conts;
}

int mock_file(const char * name, const void *conts, size_t size) {
#ifdef DEBUG
  fprintf(stderr, __FILE__
          ":mock_file: creating mock file '%s' with conts size '%zu'\n",
          name,
          size
  );
#endif

  int fd = memfd_create(name, MFD_ALLOW_SEALING);
  if (fd == -1) {
    die_perror(__FILE__, __LINE__, "memfd_create() failed");
  }
  int res = fcntl(fd, F_ADD_SEALS, F_SEAL_SHRINK );
  if (res == -1) {
    die_perror(__FILE__, __LINE__, "fcntl() initial seals failed");
  }
  ssize_t write_res = write(fd, conts, size);
  if (write_res == -1) {
    die_perror(__FILE__, __LINE__, "write() failed");
  }
  if ((size_t) write_res < size) {
    die_perror(__FILE__, __LINE__, "short write()");
  }
  res = fcntl(fd, F_ADD_SEALS, F_SEAL_SEAL );
  if (res == -1) {
    die_perror(__FILE__, __LINE__, "fcntl() final seals failed");
  }
  return fd;
}



