#include "randcpuid.h"
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h> // for dynamic linking

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
        perror (argv[1]);
      else
        valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;

  /* Now that we know we have work to do, arrange to use the
     appropriate library.  */
  void* dl_handler;
  unsigned long long (*rand64) (void);
  if (rdrand_supported ())
    {
      // handle hardware dynamic linking
      dl_handler = dlopen("randlibhw.so", RTLD_LAZY);
      if (dl_handler == NULL) {
	fprintf (stderr, "Error: dlopen call failed - %s\n", dlerror());
	return 1;
      }

      rand64 = dlsym(dl_handler, "rand64");
    
      if (dlerror()) {
	fprintf(stderr, "Error: dlsym with hardware failed - %s\n", dlerror());
	return 1;
      }
    }
  else
    {
      dl_handler = dlopen("randlibsw.so", RTLD_LAZY);
      if (dl_handler == NULL) {
	fprintf(stderr, "Error: dlopen call failed - %s\n", dlerror());
	return 1;
      }

      rand64 = dlsym(dl_handler, "rand64");

      if (dlerror()) {
	fprintf(stderr, "Error: dlsym with software failed - %s\n", dlerror());
	return 1;
      }
    }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      size_t outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (fwrite(&x, 1, outbytes, stdout) != outbytes)
        {
          output_errno = errno;
          break;
        }
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (dlclose(dl_handler)) {
    fprintf(stderr, "Error: dlclose failed - %s\n", dlerror());
    return 1;
  }
  
  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }
  
   return 0;
}
