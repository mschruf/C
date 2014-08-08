#include <stdint.h> /* int64_t */
#include <assert.h> /* assert() */


int64_t n_choose_k(int n, int k)
{
  int i;
  int64_t v = 1;


  if (k > n || k < 0) /* NOTE: these two conditions also cover 'n < 0' */

  /* since nCk == nC(n-k), we'll go with whichever of the two (k and n-k) is
     smaller, since it'll mean fewer iterations below */
  if (k > n - k) {
    k = n - k;
  }

  for (i = 1; i <= k; i++) {
    /* NOTE: this works (i.e. we won't end up with fractional intermediate
       result) due to pigeonhole principle, because we apply divisors in
       increasing order starting with 1 and multipliers are successive integers;
       thus, by the time we divide by 'm', 'v' already contains 'm' successive
       integers factors, one of which must be divisible by 'm' */
    v *= n--;
    v /= i;
  }

  return v;
}
