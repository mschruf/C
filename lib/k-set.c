#include <stdlib.h>     /* malloc(), free() */
#include <stdint.h>     /* int64_t */
#include <string.h>     /* memcpy() */
#include "k-set.h"        /* set_kset_t */
#include "n-choose-k.h" /* n_choose_k() */


void set_kset_destroy(set_kset_t * const ks)
{
  if (ks == NULL) {
    return;
  }

  free(ks->e);

  /* NOTE: not strictly necessary, but done for safety */
  ks->e = NULL;
  ks->n = ks->k = 0;
}


int set_kset_next(set_kset_t * const ks)
{
  int retval = 0; /* return value */
  int i;       /* index for 'ks->e' */


 /* NOTE: test for 'ks->n < 0' implicit in other conditions */
  if (ks == NULL || ks->e == NULL || ks->k > ks->n || ks->k < 0) {
    return -1;
  }

  /*
   * find last element of 'e' which isn't at its maximum value, increase it by
   * one, and reset succeeding elements' (i.e. elements with higher index)
   * values to their minima (based on value of element which was increased)
   *
   * if all elements are at their maximum value, current k-set is final one;
   * indicate this via return value
   */
  i = ks->k - 1; /* begin with last element of ks->e */
  while (i >= 0) {
    /* is current element's value less than max. value for this position? */
    if (ks->e[i] < (ks->n - 1) - ((ks->k - 1) - i)) {
      ks->e[i]++;

      /* reset succeeding element indices to their minimum values given value
	 of element ks->e[i] */
      while (++i < ks->k) {
	ks->e[i] = ks->e[i - 1] + 1;
      }      

      ks->rank++;

      retval = 1;
      break;
    }
    i--;
  }

  /* if we found no element which can be increased, current k-set is last one */
  return retval;
}


/* Approach to ranking:
 *   Set ks->rank appropriately, based on n, k, and membership of kset:
 *
 *   Calculate number of k-sets which precede given one, using fact that there
 *   are (n-i)C(k-1) subsets of size 'k' with 'i' as smallest element
 *   (where 'i' is index of element in origin set ranging from 1 to n)
 *
 *   Do this iteratively for each k-set element and for each of the values
 *   of that element that came before the value representing the current k-set
 */

int set_kset_rank(int const n, int const k, int const * const elements,
		  set_kset_t * const ks)
{
  int i; /* index for 'elements' array */
  int v; /* to iterate over values of elements[i] */
  int v_min = 0; /* minimum value for current k-set element (elements[i]) */
  int64_t r = 0; /* accumulator for  number of k-sets preceding one represented
		    by elements array */


 /* NOTE: test for 'n < 0' implicit in other conditions */
  if (elements == NULL || ks == NULL || k > n || k < 0) {
    return -1;
  }

  /* initialize provided k-set structure */
  if ((ks->e = malloc(k * sizeof(int))) == NULL) {
    return -1;
  }
  memcpy(ks->e, elements, k * sizeof(int));
  ks->n = n;
  ks->k = k;

  /* iterate over k-set elements */
  for (i = 0; i < k; i++) {
    /* iterate over values of this element which came before current value
       with preceding element fixed at its present value */
    for (v = v_min; v < elements[i]; v++) {
      r += n_choose_k(n - (v + 1), k - (i + 1)); /* # k-sets where e[i] = v */
    }
    v_min = elements[i] + 1;
  }
  ks->rank = r;

  return 0;
}


/*
 * Approach to unranking:
 *   Conceptual and algorithmic reverse of ranking operation:
 *
 *   Given ks->n, ks->k, and ks->rank, iterate over each ks->e[i] in array
 *   order and calculate how many k-sets there are for each value in the
 *   possible range of e[i]; the value preceding the one which yields a k-set
 *   number exceeding 'rank' is the value for that e[i]
 */

int set_kset_unrank(int const n, int const k, int64_t const rank,
		    set_kset_t * const ks)
{
  int i; /* index for 'ks->e' */
  int v; /* to iterate over values of 'ks->e[i]' */
  int64_t n_ksets_preceding = 0; /* number of k-sets preceding those where
				    ks->e[i] == v */
  int64_t n_ksets_current; /* number of k-sets where ks->e[i] == v */
  int v_min; /* minimum value for current ks->e[i] */
  int v_max; /* maximum value for ks->e[i] */


 /* NOTE: test for 'n < 0' implicit in other conditions */
  if (ks == NULL || k > n || k < 0) {
    return -1;
  }

  /* initialize provided k-set structure */
  if ((ks->e = malloc(k * sizeof(int))) == NULL) {
    return -1;
  }
  ks->n = n;
  ks->k = k;
  ks->rank = rank;

  v_min = 0;
  v_max = n - k;
  for (i = 0; i < k; i++) {
     for (v = v_min; v <= v_max; v++) {
       /* number of k-sets where e[i] = v */
       n_ksets_current =  n_choose_k(n - (v + 1), k - (i + 1));
       if (n_ksets_preceding + n_ksets_current >= rank + 1) {
	 break;
       }
       n_ksets_preceding += n_ksets_current;
     }
     ks->e[i] = v;

     v_min = ks->e[i] + 1;
     v_max++;
  }

  return 0;
}
