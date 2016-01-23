/*
 * 15.07.02 Ported to VMS
 */
#ifdef __vms
#include "net-snmp-config.h"
#else
#include <net-snmp/net-snmp-config.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#if HAVE_STRING_H
#include <string.h>
#else
#include <strings.h>
#endif

#ifdef __vms
#include "snmp_enum.h"
#else
#include <net-snmp/library/snmp_enum.h>
#endif

struct snmp_enum_list_str {
    char           *name;
    struct snmp_enum_list *list;
    struct snmp_enum_list_str *next;
};

static struct snmp_enum_list ***snmp_enum_lists;
unsigned int    current_maj_num;
unsigned int    current_min_num;
struct snmp_enum_list_str *sliststorage;

int
init_snmp_enum(void)
{
    int             i;

    if (!snmp_enum_lists)
        snmp_enum_lists = (struct snmp_enum_list ***)
            malloc(sizeof(struct snmp_enum_list **) * SE_MAX_IDS);
    if (!snmp_enum_lists)
        return SE_NOMEM;
    current_maj_num = SE_MAX_IDS;

    for (i = 0; i < SE_MAX_SUBIDS; i++) {
        if (!snmp_enum_lists[i])
            snmp_enum_lists[i] = (struct snmp_enum_list **)
                malloc(sizeof(struct snmp_enum_list *) * SE_MAX_SUBIDS);
        if (!snmp_enum_lists[i])
            return SE_NOMEM;
    }
    current_min_num = SE_MAX_IDS;

    if (!sliststorage)
        sliststorage = NULL;
    return SE_OK;
}

int
se_store_list(struct snmp_enum_list *new_list,
              unsigned int major, unsigned int minor)
{
    int             ret = SE_OK;

    if (major > current_maj_num || minor > current_min_num) {
        /*
         * XXX: realloc 
         */
        return SE_NOMEM;
    }


    if (snmp_enum_lists[major][minor] != NULL)
        ret = SE_ALREADY_THERE;

    snmp_enum_lists[major][minor] = new_list;

    return ret;
}

struct snmp_enum_list *
se_find_list(unsigned int major, unsigned int minor)
{
    if (major > current_maj_num || minor > current_min_num)
        return NULL;

    return snmp_enum_lists[major][minor];
}

int
se_find_value_in_list(struct snmp_enum_list *list, char *label)
{
    if (!list)
        return SE_DNE;          /* XXX: um, no good solution here */
    while (list) {
        if (strcmp(list->label, label) == 0)
            return (list->value);
        list = list->next;
    }

    return SE_DNE;              /* XXX: um, no good solution here */
}

int
se_find_value(unsigned int major, unsigned int minor, char *label)
{
    return se_find_value_in_list(se_find_list(major, minor), label);
}

char           *
se_find_label_in_list(struct snmp_enum_list *list, int value)
{
    if (!list)
        return NULL;
    while (list) {
        if (list->value == value)
            return (list->label);
        list = list->next;
    }
    return NULL;
}

char           *
se_find_label(unsigned int major, unsigned int minor, int value)
{
    return se_find_label_in_list(se_find_list(major, minor), value);
}

int
se_add_pair_to_list(struct snmp_enum_list **list, char *label, int value)
{
    struct snmp_enum_list *lastnode = NULL;

    if (!list)
        return SE_DNE;

    while (*list) {
        if ((*list)->value == value)
            return (SE_ALREADY_THERE);
        lastnode = (*list);
        (*list) = (*list)->next;
    }

    if (lastnode) {
        lastnode->next = (struct snmp_enum_list *)
            malloc(sizeof(struct snmp_enum_list));
        lastnode = lastnode->next;
    } else {
        (*list) = (struct snmp_enum_list *)
            malloc(sizeof(struct snmp_enum_list));
        lastnode = (*list);
    }
    if (!lastnode)
        return (SE_NOMEM);
    lastnode->label = label;
    lastnode->value = value;
    lastnode->next = NULL;
    return (SE_OK);
}

int
se_add_pair(unsigned int major, unsigned int minor, char *label, int value)
{
    struct snmp_enum_list *list = se_find_list(major, minor);
    int             created = (list) ? 1 : 0;
    int             ret = se_add_pair_to_list(&list, label, value);
    if (!created)
        se_store_list(list, major, minor);
    return ret;
}

/*
 * remember a list of enums based on a lookup name.
 */
struct snmp_enum_list *
se_find_slist(const char *listname)
{
    struct snmp_enum_list_str *sptr, *lastp = NULL;
    if (!listname)
        return NULL;

    for (sptr = sliststorage;
         sptr != NULL; lastp = sptr, sptr = sptr->next)
        if (sptr->name && strcmp(sptr->name, listname) == 0)
            return sptr->list;

    if (lastp) {
        lastp->next = (struct snmp_enum_list_str *)
            malloc(sizeof(struct snmp_enum_list_str));
        sptr = lastp->next;
    } else {
        sptr = (struct snmp_enum_list_str *)
            malloc(sizeof(struct snmp_enum_list_str));
        sliststorage = sptr;
    }
    sptr->list = NULL;
    sptr->name = strdup(listname);
    sptr->next = NULL;
    return sptr->list;
}


char           *
se_find_label_in_slist(const char *listname, int value)
{
    return (se_find_label_in_list(se_find_slist(listname), value));
}


int
se_find_value_in_slist(const char *listname, char *label)
{
    return (se_find_value_in_list(se_find_slist(listname), label));
}

int
se_add_pair_to_slist(const char *listname, char *label, int value)
{
    struct snmp_enum_list *list = se_find_slist(listname);
    int             created = (list) ? 1 : 0;
    int             ret = se_add_pair_to_list(&list, label, value);

    if (!created) {
        struct snmp_enum_list_str *sptr = (struct snmp_enum_list_str *)
            malloc(sizeof(struct snmp_enum_list_str));
        if (!sptr)
            return SE_NOMEM;
        sptr->next = sliststorage;
        sptr->name = strdup(listname);
        sptr->list = list;
        sliststorage = sptr;
    }
    return ret;
}

#ifdef TESTING
main()
{
    init_snmp_enum();
    se_add_pair(1, 1, "hi", 1);
    se_add_pair(1, 1, "there", 2);
    printf("hi: %d\n", se_find_value(1, 1, "hi"));
    printf("2: %s\n", se_find_label(1, 1, 2));

    se_add_pair_to_slist("testing", "life, and everything", 42);
    se_add_pair_to_slist("testing", "resturant at the end of the universe",
                         2);

    printf("life, and everything: %d\n",
           se_find_value_in_slist("testing", "life, and everything"));
    printf("2: %s\n", se_find_label_in_slist("testing", 2));
}
#endif                          /* TESTING */
