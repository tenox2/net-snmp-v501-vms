/*
 * 11.07.02 Port to VMS
 */
/*
 * oid_array.h
 * $Id: oid_array.h,v 5.0 2002/04/20 07:30:12 hardaker Exp $
 *
 * External definitions for functions and variables in oid_array.c.
 *
 * The basic idea is to keep an array of data items (or, more
 * likely, pointers to data items) sorted by their index oid.
 *
 * Inspired by agent/mibgroup/util_funcs.c
 *
 *
 * Note: These arrays are sorted lazily. Thus, some methods
 * have a sort parameter in case you don't care if the array
 * has been sorted since it was last changed.
 *
 *
 * EXAMPLE:
 * ------------------------------------------------------------
 * typedef struct my_row_s {
 *   oid_header index;
 *   int column_a;
 *   int column_b;
 * } my_row;
 *
 * oid_array my_table;
 * netsnmp_variable_list *var = NULL;
 * int idx1;
 *
 * my_table = netsnmp_Initialise_oid_array( sizeof(my_row*) );
 *
 * 
 * my_row* new_row = (my_row*)calloc(sizeof(my_row));
 * new_row->column_a = 10;
 * new_row->column_b = 20;
 * idx1 = 22;
 * snmp_varlist_add_variable(&var, NULL, 0, ASN_INTEGER,
 *                           &idx1, sizeof(idx1));
 * snmp_varlist_add_variable(&var, NULL, 0, ASN_OCTET_STR,
 *                           "skiddo", strln("skiddo"));
 * build_oid( &my_row->index.idx, &my_row->index.idx_len,
 *            NULL, 0, var);
 * netsnmp_Add_oid_data( my_table, new_row );
 *
 * ------------------------------------------------------------
 */

#ifndef OID_ARRAY_H
#define OID_ARRAY_H

#ifdef __cplusplus
extern          "C" {
#endif

#ifdef __vms
#include "snmp_lib_asn1.h"
#else
#include <net-snmp/library/asn1.h>
#endif
    typedef void   *oid_array;

    /*
     * since I didn't want to re-write qsort and such, your data
     * structure must start with this header. (Ok, not really, but
     * the first two elements of your data structure better be a
     * pointer to the index oid and the length of the index!)
     */
    typedef struct netsnmp_oid_array_header_s {
        oid            *idx;
        int             idx_len;
    } netsnmp_oid_array_header;

    typedef struct netsnmp_oid_array_header_wrapper_s {
        oid            *idx;
        int             idx_len;
        void           *data;
    } netsnmp_oid_array_header_wrapper;

    typedef void    (Netsnmp_For_Each) (netsnmp_oid_array_header *,
                                        void *context);

    /*
     * compare two entries. Nothing fancy, just a wrapper around
     * snmp_oid_compare.
     */
    int             netsnmp_array_compare(const void *lhs,
                                          const void *rhs);

    /*
     * initialize an oid array which will contain data.
     *
     * data_size  should be the size of each item
     */
    oid_array       netsnmp_initialize_oid_array(int data_size);

    /*
     * add an entry to an array.
     *
     * returns 0 on success, -1 on failure
     */
    int             netsnmp_add_oid_data(oid_array a, void *);

    /*
     * replace an entry to an array.
     *
     * returns 0 on success, -1 on failure
     */
    int             netsnmp_replace_oid_data(oid_array a, void *key);

    /*
     * find the entry in the array with the same index
     *
     * Note: do not change the index!  If you need to
     * change an index, remove the entry, change the index,
     * and the re-add the entry.
     */
    void           *netsnmp_get_oid_data(oid_array a, void *, int exact);

    /*
     * find entries in the array with the same index prefix
     *
     * Upon return, *len will contain the number of entries found.
     *
     * Note: caller is responsible for calling free() on the
     * pointer returned by this function.
     */
    void          **netsnmp_get_oid_data_subset(oid_array a, void *key,
                                                int *len);

    /*
     * find the number of entries in the array
     *
     */
    int             netsnmp_get_oid_data_count(oid_array a);

    /*
     * remove an entry
     *
     * if save is not null, the entry will be copied to the address
     * save points at.
     */
    int             netsnmp_remove_oid_data(oid_array a, void *key,
                                            void *save);

    /*
     * release memory used by a table.
     *
     * Note: if your data structure contained allcoated
     * memory, you are responsible for releasing that
     * memory before calling this function!
     */
    void            netsnmp_release_oid_array(oid_array a);

    /*
     * call a function for each entry (useful for cleanup).
     *
     * The Netsnmp_For_Each function will be called with a pointer
     * to an entry and the context pointer.
     *
     * If sort = 1, entries will be in sorted order. Otherwise
     * the order is not defined.
     */
    void            netsnmp_for_each_oid_data(oid_array a,
                                              Netsnmp_For_Each *,
                                              void *context, int sort);

    /*
     * get internal pointer to array (DANGER WILL ROBINSON!)
     *
     * standard disclaimer: DO NOT USE THIS METHOD!
     *
     * Ok, you can use it. Just don't muck about with the
     * ordering or indexes and expect anything to still work.
     *
     * size will be set to the number of elements. If sort is set,
     * the table will be sorted. If sort is not set, the order is
     * not defined.
     */
    void           *netsnmp_retrieve_oid_array(oid_array a, int *size,
                                               int sort);

#ifdef __cplusplus
}
#endif
#endif
