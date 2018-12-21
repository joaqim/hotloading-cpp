
#include "xs_Float.h"


// ====================================================================================================================
//  Fix Class
// ====================================================================================================================
typedef int32 Fix;


template <int32 N>
Fix       xs_Fix<N>::ToFix       (int32 val)    {return val<<N;}

template <int32 N>
Fix       xs_Fix<N>::ToFix       (real64 val)   {return xs_ConvertToFixed(val);}

// ====================================================================================================================
//  Basic Conversion to Numbers
// ====================================================================================================================

template <int32 N>
real64    xs_Fix<N>::ToReal      (Fix f)        {return real64(f)/real64(1<<N);}
template <int32 N>
int32     xs_Fix<N>::ToInt       (Fix f)        {return f>>N;}

template <int32 N>
int32 xs_Fix<N>::xs_ConvertToFixed (real64 val) {
#if _xs_DEFAULT_CONVERSION==0
  return xs_CRoundToInt(val, _xs_doublemagic/(1<<N));
#else
  return (long)((val)*(1<<N));
#endif
}





// ====================================================================================================================
// ====================================================================================================================
//  Inline implementation
// ====================================================================================================================
int32 xs_CRoundToInt(real64 val, real64 dmr) {
#if _xs_DEFAULT_CONVERSION==0
  val		= val + dmr;
	return ((int32*)&val)[_xs_iman_];
  //return 0;
#else
  return int32(floor(val+.5));
#endif
}


int32 xs_ToInt(real64 val, real64 dme) {
#if _xs_DEFAULT_CONVERSION==0
  return (val<0) ?   xs_CRoundToInt(val-dme) : 
      xs_CRoundToInt(val+dme);
#else
  return int32(val);
#endif
}


// ====================================================================================================================
int32 xs_FloorToInt(real64 val, real64 dme) {
#if _xs_DEFAULT_CONVERSION==0
      return xs_CRoundToInt (val - dme);
#else
      return floor(val);
#endif
    }


// ====================================================================================================================
int32 xs_CeilToInt(real64 val, real64 dme) {
#if _xs_DEFAULT_CONVERSION==0
    return xs_CRoundToInt (val + dme);
#else
    return ceil(val);
#endif
}


// ====================================================================================================================
int32 xs_RoundToInt(real64 val) {
#if _xs_DEFAULT_CONVERSION==0
  return xs_CRoundToInt (val + _xs_doublemagicdelta);
#else
  return floor(val+.5);
#endif
}

