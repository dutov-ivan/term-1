#ifndef ROOT_H
#define ROOT_H

#define DEFAULT_ITERATION_LIMIT 1000000

long double approximate_kth_root(long double x, int k, long double e_target,
                                 int decimal_places, int is_quiet_mode,
                                 int is_return_delta, int iteration_limit);
#endif
