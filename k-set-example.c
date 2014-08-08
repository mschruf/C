/*
 * Enumerate combinations of size 'k' of given set of characters
 *
 * Parameters:
 *   1: string of characters forming set from which k-combinations are drawn
 *   2: value of 'k', where 'k' is non-negative integer
 */

#include <stdio.h>     /* stderr, fprintf(), printf(), putchar() */
#include <stdlib.h>    /* EXIT_FAILURE, exit(), atoi() */
#include <string.h>    /* strlen() */
#include "lib/k-set.h" /* set_kset_t, set_kset_init(), set_kset_next(),
                          set_kset_destroy() */


int main(int argc, char **argv)
{
  set_kset_t ks; /* structure for set of size 'k' drawn from character set */


  if (argc < 3) {
    fprintf(stderr, "usage: %s: character_set k\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  size_t const n = strlen(argv[1]);
  int const k = atoi(argv[2]);

 /* NOTE: test for negative value must come first, otherwise cast to size_t will
    yield unintended value */
  if (k < 0 || (size_t)k > n) {
    fprintf(stderr, "%s: 0 <= k <= n, where n is size of character set\n",
            argv[0]);
    exit(EXIT_FAILURE);
  }

  if (set_kset_init(n, k, &ks) != 0) {
    fprintf(stderr, "%s: unable to initialize k-set structure\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  do {
    /* print current k-set and its (zero-based) rank */
    printf("%zu: ", ks.rank);
    for (int i = 0; i < k; i++) {
      putchar(argv[1][ks.e[i]]);
    }
    putchar('\n');
  } while (set_kset_next(&ks) == 1);

  set_kset_destroy(&ks);

  return 0;
}
