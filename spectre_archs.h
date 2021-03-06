#ifndef __SPECTRE_ARCHS__
#define __SPECTRE_ARCHS__

#if defined(__powerpc__) || defined(__ppc__)  || defined(__ppc64__)
// PowerPC
#define __ARCH_POWERPC__
#if defined(__ppc64__)
#define __ARCH_64BIT__ 1
#define __ARCH_32BIT__ 0
#else
#define __ARCH_32BIT__ 1
#define __ARCH_64BIT__ 0
#endif
#elif defined(__arm__) || defined(__thumb__) || defined(__aarch64__)
// ARM
#define __ARCH_ARM__
#if defined(__aarch64__)
#define __ARCH_64BIT__ 1
#define __ARCH_32BIT__ 0
#else
#define __ARCH_32BIT__ 1
#define __ARCH_64BIT__ 0
#endif
#elif defined(__s390x__) || defined(__zarch__) || defined(__s390__) || defined(__SYSC_ZARCH__)
// s390x
#define __ARCH_S390X__
#elif defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64) || defined(__i386__) || defined(_M_IX86) || defined(__X86__) || defined(__INTEL__) || defined(_X86_)
// x86
#define __ARCH_X86__
#if defined(__amd64__) || defined(__x86_64__) || defined(_M_X64) || defined(_M_AMD64)
#define __ARCH_64BIT__ 1
#define __ARCH_32BIT__ 0
#else
#define __ARCH_32BIT__ 1
#define __ARCH_64BIT__ 0
#endif

#else

// Unsupported CPU architecture
#error Unsupported CPU architecture

#endif
#endif
