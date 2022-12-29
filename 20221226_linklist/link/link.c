#include "stdio.h"
#include "stdlib.h"
#include "link.h"

STU *insert_link(STU *head, STU tmp) {
    STU *pi = (STU *) calloc(1, sizeof(STU));
    if (pi == NULL) {
        perror("calloc");
        exit(-1);
    }
}
