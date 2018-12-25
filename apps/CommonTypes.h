#pragma once
#ifndef _CommonTypes_
#define _CommonTypes_

/*
** Includes
*/
//#include <Log.h> // For nice assertions
#include <iomanip>
#include <xs/xs_Float.h>

/*
** Macro Definitions
*/
#ifdef DEBUG
#define Assert(x) ((void)(!(x) && assert_handler(#x, __FILE__, __LINE__) && (HALT(), 1)))
#define StaticAssert(Condition, Message) static_assert(Condition, #Message);
#else
#define Assert(x) ((void)sizeof(x))
#define StaticAssert(Condition, Message) ;
#endif
/*
** Condition = TRUE is ok, Condition = FALSE is error 
*/

/*
** Define compiler specific macros
** The __extension__ compiler pragma is required
** for the uint64 type using GCC with the ANSI C90 standard.
** Other macros can go in here as needed, for example alignment 
** pragmas.
**
** NOTE: The white-box (coverage) unit testing may need to disable
** these extra attributes.  These test builds define the OSAPI_NO_SPECIAL_ATTRIBS
** macro to disable this.
*/
#if defined (__GNUC__) && !defined(OSAPI_NO_SPECIAL_ATTRIBS)
#define _EXTENSION_     __extension__
#define OS_PACK         __attribute__ ((packed))
#define OS_ALIGN(n)     __attribute__((aligned(n)))
#define OS_USED         __attribute__((used))
#define OS_PRINTF(n,m)  __attribute__ ((format (printf, n, m)))
#else
#define _EXTENSION_
#define OS_PACK
#define OS_ALIGN(n)
#define OS_USED
#define OS_PRINTF(n,m)
#endif

#define PLATFORM_WIN32 1
#define PLATFORM_LINUX 2
#define PLATFORM_KQUEUE 3

#if defined(_WIN32)
#	define PLATFORM PLATFORM_WIN32
#elif defined(__APPLE_CC__) || defined(BSD)
#	define PLATFORM PLATFORM_KQUEUE
#elif defined(__linux__)
#	define PLATFORM PLATFORM_LINUX
#endif

#if (PLATFORM == PLATFORM_WIN32)
#include <stdlib.h>

typedef unsigned __int16 U2;
typedef unsigned __int32 U4;
typedef unsigned __int64 U8;

#define bswap2 _byteswap_ushort
#define bswap4 _byteswap_ulong
#define bswap8 _byteswap_uint64

#elif (PLATFORM == PLATFORM_LINUX)
typedef uint16_t U2;
typedef uint32_t U4;
typedef uint64_t U8;
#define bswap2 __builtin_bswap16
#define bswap4 __builtin_bswap32
#define bswap8 __builtin_bswap64
#endif

#define local_persist static
#define global_variable static

constexpr uint64 Kilobytes(uint64 const value) {return value*1024LL;}
constexpr uint64 Megabytes(uint64 const value) {return Kilobytes(value)*1024LL;}
constexpr uint64 Gigabytes(uint64 const value) {return Megabytes(value)*1024LL;}
constexpr uint64 Terabytes(uint64 const value) {return Gigabytes(value)*1024LL;}

/*
** Check Sizes
*/

StaticAssert(sizeof(uint8)==sizeof(unsigned char),  TypeUint8WrongSize);
StaticAssert(sizeof(uint16)==sizeof(unsigned short), TypeUint16WrongSize);
StaticAssert(sizeof(uint32)==sizeof(unsigned long), TypeUint32WrongSize);
StaticAssert(sizeof(uint64)==sizeof(unsigned long long), TypeUint64WrongSize);
StaticAssert(sizeof(int8)==sizeof(char),   Typeint8WrongSize);
StaticAssert(sizeof(int16)==sizeof(unsigned short),  Typeint16WrongSize);
StaticAssert(sizeof(int32)==sizeof(long),  Typeint32WrongSize);
StaticAssert(sizeof(int64)==sizeof(long long),  Typeint64WrongSize);
//StaticAssert(sizeof(cpuaddr) >= sizeof(void *),  TypePtrWrongSize);

inline uint32
SafeTruncateUInt64(uint64 Value) {
  Assert(Value <= 0xFFFFFFFF);
  uint32 Result = (uint32)Value;
  return(Result);
}

#ifndef NULL              /* pointer to nothing */
#define NULL ((void *) 0)
#endif

#ifndef TRUE              /* Boolean true */
#define TRUE (1)
#endif

#ifndef FALSE              /* Boolean false */
#define FALSE (0)
#endif

#endif  /* _CommonTypes_ */

