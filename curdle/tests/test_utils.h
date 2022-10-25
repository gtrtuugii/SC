#ifndef TEST_UTILS_H
#define TEST_UTILS_H

//////////////////////////
// DO NOT SUBMIT THIS FILE
//////////////////////////

#include <stddef.h>

/** Creates an in-memory file with the contents set to
  * the buffer pointed to by `conts`. Exactly `size` many
  * bytes are written to the file.
  *
  * When calculating `size`, note that if `conts`
  * has a terminating null byte ('\0'), your value for
  * `size` should NOT include it
  * (unless you do actually want the null byte written to the
  * file).
  *
  * When the file is finished with, close() (see `man 2 close`)
  * should be called on the file descriptor.
  *
  * \param name a filename used for diagnostic purposes.
  * \param conts pointer to the desired file contents,
  *     which should be `size` bytes long.
  * \param size  the size of the file contents.
  * \return a file descripter for the created file.
  */
int mock_file(const char * name, const void *conts, size_t size);

/** Generate random ASCII characters into a buffer
  * of size `size` of malloc'd memory.
  *
  * The caller should free() this memory after use.
  *
  * \param size number of random bytes to generate
  * \return pointer to a buffer containing random characters
  */
void* random_contents(size_t size);

#endif // TEST_UTILS_H

