#ifndef _COLORLIST_H_
#define _COLORLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    COLOR_4BIT = 1,
    COLOR_8BIT = 2,
    COLOR_TRUE = 3
} cpprich_colortype_t;

extern cpprich_colortype_t color_type;

typedef struct {
    char tag[32];
    char ANSI[32];
} cpprich_displaysetlist_t;

#define DISPLAY_SET_LIST_LENGTH 39
extern const cpprich_displaysetlist_t Display_Set_List[];

#ifdef __cplusplus
}
#endif

#endif