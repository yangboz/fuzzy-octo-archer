/* Boost Regex */
#cmakedefine  HAVE_BOOST_REGEX

/* Boost FileSystem */
#cmakedefine  HAVE_BOOST_FILESYSTEM

/* OpenCV library */
#cmakedefine  HAVE_OPENCV

/* VOSM compiled as static or dynamic libs */
#cmakedefine  VOSM_BUILD_SHARED_LIB

/* Name of package */
#define  PACKAGE "${PACKAGE}"

/* Define to the address where bug reports for this package should be sent. */
#define  PACKAGE_BUGREPORT "${PACKAGE_BUGREPORT}"

/* Define to the full name of this package. */
#define  PACKAGE_NAME "${PACKAGE_NAME}"

/* Define to the full name and version of this package. */
#define  PACKAGE_STRING "${PACKAGE_STRING}"

/* Define to the one symbol short name of this package. */
#define  PACKAGE_TARNAME "${PACKAGE_TARNAME}"

/* Define to the version of this package. */
#define  PACKAGE_VERSION "${PACKAGE_VERSION}"

/* If using the C implementation of alloca, define if you know the
   direction of stack growth for your system; otherwise it will be
   automatically deduced at runtime.
	STACK_DIRECTION > 0 => grows toward higher addresses
	STACK_DIRECTION < 0 => grows toward lower addresses
	STACK_DIRECTION = 0 => direction of growth unknown */
#cmakedefine  STACK_DIRECTION

/* Version number of package */
#define  VERSION "${PACKAGE_VERSION}"

/* Define to 1 if your processor stores words with the most significant byte
   first (like Motorola and SPARC, unlike Intel and VAX). */
#cmakedefine  WORDS_BIGENDIAN

