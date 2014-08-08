/*
 * Functions for enumerating, ranking, and unranking k-sets
 *
 * Note:
 *   K-set is represented as string of strictly increasing integers;
 *   integers are zero-based indices of elements of set from which k-set was
 *   chosen
 */

#ifndef __LIB_KSET_H
#define __LIB_KSET_H 1

#include <stdint.h> /* int64_t */

typedef struct {
  int n;        /* size of origin set */
  int k;        /* size of subsets to be chosen */
  int *e;       /* pointer to array of size 'k'; element values represent
                   (arbitrary) indices of origin set elements which are members
		               of this k-set */
  int64_t rank; /* zero-based rank of k-set, i.e. in our canonical order this
                   is k-set 'rank' of n-choose-k */
} set_kset_t;

/*
 * Initialize k-set structure
 *
 * Arguments:
 *   n: size of set of origin set
 *   k: size of subsets to be chosen
 *   ksp: pointer to type 'set_kset_t' to be initialized
 *
 * Return values:
 *   -1: error (invalid arguments or malloc() failure)
 *    0: success
 *
 * Note: caller must call set_kset_destroy() to free allocated memory
 */
#define set_kset_init(n,k,ksp) set_kset_unrank(n,k,0,ksp)

/*
 * Release memory allocated for k-set structure
 *
 * Arguments:
 *   *ks: pointer to k-set structure
 *
 * Return values:
 *   None
 */
void set_kset_destroy(set_kset_t * const ks);

/*
 * Update 'ks' structure to reflect k-set succeeding current one in canonical
 * order
 *
 * Arguments:
 *   *ks: structure representing k-set; must have been previously initialized,
 *        either via call to set_kset_init(), set_kset_rank(), or
 *        set_kset_unrank()
 *
 * Return values:
 *   -1: error (invalid arguments)
 *    0: k-set provided as argument is last one in canonical order
 *    1: success; 'ks' updated to reflect next k-set
 */
int set_kset_next(set_kset_t * const ks);

/*
 * Calculate rank of k-set
 *
 * Arguments:
 *    n:        size of set from which subset was chosen
 *    k:        size of subset
 *   *elements: pointer to array whose values represent indices of elements of
 *              origin set which are members of k-set; elements of array must
 *              be in increasing order of value
 *   *ks:       pointer to k-set structure; initialized with arguments and
 *              result placed in ks->rank, suitable for calls to
 *              set_kset_next()
 *
 * Return values:
 *   -1: error (invalid arguments or malloc() failure)
 *    0: success
 */
int set_kset_rank(int const n, int const k, int const * const elements,
                  set_kset_t * const ks);

/*
 * Calculate k-set membership from rank, i.e. unrank k-set
 *
 * Arguments:
 *    n:    size of set from which subset was chosen
 *    k:    size of subset
 *    rank: rank of subset
 *   *ks:   pointer to k-set data structure to be initialized according to
 *          arguments 'n', 'k', and 'rank'; will be suitable for calls to
 *          set_kset_next()
 *
 * Return values:
 *   -1: error (invalid arguments or malloc() failure)
 *    0: success
 */
int set_kset_unrank(int const n, int const k, int64_t const rank,
                    set_kset_t * const ks);

#endif /* !__LIB_KSET_H */
