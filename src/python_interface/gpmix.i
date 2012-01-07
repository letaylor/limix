%module gpmix

%{
#define SWIG_FILE_WITH_INIT
#define SWIG
#include "gpmix/types.h"
#include "gpmix/LMM/lmm.h"
#include "gpmix/lasso/lasso.h"
#include "gpmix/covar/covariance.h"
#include "gpmix/covar/linear.h"
#include "gpmix/covar/se.h"
#include "gpmix/covar/fixed.h"	
#include "gpmix/covar/combinators.h"	
#include "gpmix/likelihood/likelihood.h"
#include "gpmix/mean/ADataTerm.h"
#include "gpmix/mean/CData.h"
#include "gpmix/mean/CLinearMean.h"
#include "gpmix/gp/gp_base.h"
#include "gpmix/gp/gp_kronecker.h"
#include "gpmix/gp/gp_opt.h"



	//typedef unsigned int uint64_t;
  using namespace gpmix;
%}
//typedef unsigned int uint_t;
//typedef unsigned int uint64_t;

/* Get the numpy typemaps */
%include "numpy.i"
%include "eigen.i"
%include "std_vector.i"
%include "std_map.i"
%include "std_string.i"


%init %{
  import_array();
%}



/* numpy wrappers for default wrapping
   Note: we are not using these. The real wrappers are in eigen.i
*/
%apply (short*  IN_ARRAY1, int DIM1) {(short*  series, int size)};
%apply (int*    IN_ARRAY1, int DIM1) {(int*    series, int size)};
%apply (long*   IN_ARRAY1, int DIM1) {(long*   series, int size)};
%apply (float*  IN_ARRAY1, int DIM1) {(float*  series, int size)};
%apply (double* IN_ARRAY1, int DIM1) {(double* series, int size)};

%apply (int*    IN_ARRAY2, int DIM1, int DIM2) {(int*    matrix, int rows, int cols)};
%apply (double* IN_ARRAY2, int DIM1, int DIM2) {(double* matrix, int rows, int cols)};

%apply (int*    INPLACE_ARRAY1, int DIM1) {(int*    array,   int size)};
%apply (double* INPLACE_ARRAY1, int DIM1) {(double* array,   int size)};



/* Remove C Prefix 
%rename(VBFA) cVBFA;
%rename(PEER) cSPARSEFA;
*/


/* Include the header file to be wrapped */
%include "gpmix/types.h"
%include "gpmix/LMM/lmm.h"
%include "gpmix/lasso/lasso.h"
%include "gpmix/covar/covariance.h"
%include "gpmix/covar/linear.h"
%include "gpmix/covar/se.h"
%include "gpmix/covar/fixed.h"
%include "gpmix/covar/combinators.h"	
%include "gpmix/likelihood/likelihood.h"
%include "gpmix/mean/ADataTerm.h"
%include "gpmix/mean/CData.h"
%include "gpmix/mean/CLinearMean.h"
%include "gpmix/gp/gp_base.h"
%include "gpmix/gp/gp_kronecker.h"
%include "gpmix/gp/gp_opt.h"



 
