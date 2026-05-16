#ifndef MAGMA_DIMACS_CNF_ASSERT_AND_ASSUME_HPP
#define MAGMA_DIMACS_CNF_ASSERT_AND_ASSUME_HPP

#ifdef NDEBUG

# define ASSERT_AND_ASSUME(expr_) \
     do { [[assume(expr_)]]; }    \
     while(false)

#else

# include <cassert>

# define ASSERT_AND_ASSUME(expr_) \
     do { assert(expr_); }        \
     while(false)

#endif

#endif
