#include <assert.h>
#include "shelf.h"
#include "box.h"
#include "cs136-trace.h"
#include <stdio.h>
#include <stdlib.h>

// maximum number of boxes that can be stored on the shelf
static const int SHELF_CAPACITY = 10;

// See box.h for documentation.
struct shelf shelf_init(void) {
    struct shelf x;
    x.count = 0;
    return x;
}

// See box.h for documentation.
// This function fails if the shelf capacity has been exceeded.
bool shelf_push(struct shelf *sh, char id, int volume, int weight) {
    assert(sh);
    assert(('A' <= id && id <= 'Z') || ('a' <= id && id <= 'z'));
    assert(volume > 0);
    assert(weight > 0);
    if (sh->count == SHELF_CAPACITY) {
        return false;
    } else {
        int i = sh->count;
        struct box box = box_init(id, volume, weight);
        sh->boxes[i] = box;
        sh->pboxes[i] = &sh->boxes[i];
        sh->count += 1;
        return true;
    }
}

// See box.h for documentation.
// This function fails if the shelf is empty.
bool shelf_pop(struct shelf *sh, struct box *popped) {
    assert(sh);
    assert(popped);
    if (sh->count == 0) {
        return false;
    } else {
        int i = sh->count - 1;
        *popped = sh->boxes[i];
        int n = 0;
        for (; n <= i; n++){
            if ((sh->boxes[i].id == sh->pboxes[n]->id) &&
                (sh->boxes[i].volume == sh->pboxes[n]->volume) &&
                (sh->boxes[i].weight == sh->pboxes[n]->weight)) {
                    break;
                } 
        }
        while (n < i) {
            sh->pboxes[n] = sh->pboxes[n+1];
            n++;
        }
        sh->count -= 1;
        return true;
    }
}

// See box.h for documentation.
// This function fails if the shelf is empty.
bool shelf_top(const struct shelf *sh, const struct box **top) {
    assert(sh);
    assert(top);
    if (sh->count == 0) {
        return false;
    } else {
        *top = &sh->boxes[sh->count-1];
        return true;
    }
}

// See box.h for documentation.
void shelf_sort(struct shelf *sh, int(comp)(const struct box *, const struct box *)) {
    assert(sh);
    int len = sh->count;
    for (int i = 0; i < len - 1; i++) {
        int selected = i;
        for (int j = i + 1; j < len; j++) {
            int result = comp(sh->pboxes[selected], sh->pboxes[j]);
            if (result > 0) {
                selected = j;
            } 
        }
        struct box *temp = sh->pboxes[i];
        sh->pboxes[i] = sh->pboxes[selected];
        sh->pboxes[selected] = temp;
    }
}

// See box.h for documentation.
void shelf_print(const struct shelf *sh) {
    assert(sh);
    printf("=== SHELF (actual) =========\n");
    for (int i = 0; i < sh->count; ++i) {
        box_print(&(sh->boxes[i]));
    }
    printf("============================\n");
}

// See box.h for documentation.
void shelf_print_refs(const struct shelf *sh) {
    assert(sh);
    printf("=== SHELF (references) =====\n");
    for (int i = 0; i < sh->count; ++i) {
        box_print(sh->pboxes[i]);
    }
    printf("============================\n");
}
