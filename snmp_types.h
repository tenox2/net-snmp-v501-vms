/*
 * 11.07.02 Ported to VMS
 */

#ifndef NET_SNMP_TYPES_H
#define NET_SNMP_TYPES_H

    /**
     *  Definitions of data structures, used within the library API.
     */

                        /*
                         * For 'timeval' 
                         */
#if TIME_WITH_SYS_TIME
# ifdef WIN32
#  include <sys/timeb.h>
# else
#  include <sys/time.h>
# endif
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <sys/types.h>
#ifdef HAVE_WINSOCK_H
#include <winsock.h>
#endif

#ifndef MAX_SUBID               /* temporary - duplicate definition protection */
#ifndef EIGHTBIT_SUBIDS
typedef u_long  oid;
#define MAX_SUBID   0xFFFFFFFF
#else
typedef u_char  oid;
#define MAX_SUBID   0xFF
#endif
#endif


    /*
     *  For the initial release, this will just refer to the
     *  relevant UCD header files.
     *    In due course, the types and structures relevant to the
     *  Net-SNMP API will be identified, and defined here directly.
     *
     *  But for the time being, this header file is primarily a placeholder,
     *  to allow application writers to adopt the new header file names.
     */

#ifdef __vms
#include "snmp_definitions.h"
#include "snmp_api.h"
#else
#include <net-snmp/definitions.h>
#include <net-snmp/library/snmp_api.h>
#endif
/*
 * #include <net-snmp/library/libsnmp.h> 
 */

#endif                          /* NET_SNMP_TYPES_H */
