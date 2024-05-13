///////////////////////////////////////////////////////////////////////////////
// INTEGRITY STATEMENT (v4)
//
// By signing your name and ID below you are stating that you have agreed
// to the online academic integrity statement posted on edX:
// (Course > Assignments >  Information & Policies > Academic Integrity Policy)
///////////////////////////////////////////////////////////////////////////////
// I received help from and/or collaborated with:

// None
//
// Name: Jessie Cao
// login ID: j268cao
///////////////////////////////////////////////////////////////////////////////

#include "collector.h"
#include "tokenizer.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// === Helper functions for the Collector ADT =================================

// print_char(v) prints the char that v points at.
// effects:  writes to output
// time:     O(1)
void print_char(const void *v) {
    const char *ch = v;
    if (*ch >= ' ') {
        printf("'%c'", *ch);
    } else if (*ch == '\n') {
        printf("'\\n'");
    } else {
        printf("'\\%d'", *ch);
    }
}

// print_str(v) prints the string that v points at.
// effects:  writes to output
// time:     O(n), where n is the string length
void print_str(const void *v) {
    char * const *str = v;
    const char *ch = *str;
    printf("\"");
    while (*ch != '\0') {
        if (*ch == '\n') {
            printf("\\n");
        } else {
            printf("%c", *ch);
        }
        ++ch;
    }
    printf("\"");
}

// === The Tokenizer ==========================================================

const char NULL_TERMINATOR = '\0';


// See documentation in tokenize.h.
char **tokenize_input(int *len) {
    // Your implementation goes here.
    assert(len);
    bool switched_markup = false;
    bool in_markup = false;
    bool stopping_markup = false;
    bool stop_loop = false;
    struct collector *cchar = collector_init(sizeof(char), print_char);
    struct collector *ctokens = collector_init(sizeof(char *), print_str);

    char input = NULL_TERMINATOR;
    int arr_count = 0;
    
    while (!stop_loop) {
        //check to see if the state is changed
        if (scanf("%c", &input) != 1) {
            stop_loop = true;
            input = ' ';
            switched_markup = true;
        }

        if (input == '<') {
            switched_markup = true;
            in_markup = true;
        } else if (input == '>') {
            //the state is changing but its changed after the >
            stopping_markup = true;
        } else if (stopping_markup) {
            switched_markup = true;
            in_markup = false;
            stopping_markup = false;
        }

        if (switched_markup) {
            //add the last null terminator of previous markup
            collector_insert(cchar, &NULL_TERMINATOR);
            if (in_markup) { //switched from content to markup
                char *string_arr = collector_collapse(cchar, &arr_count);
                bool is_space = true;
                //loop through the array to see if the whole thing is empty
                for (char *p = string_arr; p < string_arr + arr_count; p++) {
                    if (*p != ' ' && *p != NULL_TERMINATOR){
                        is_space = false;
                        break;
                    }
                }
                //insert string into tokens collector
                if (!is_space) {
                    collector_insert(ctokens, &string_arr);
                    *len+=1;
                } else {
                    free(string_arr);
                }

                //reset all properties of cchar 
                arr_count = 0;
                cchar = collector_init(sizeof(char), print_char);

            } else { //switched from markup to content
                char *string_arr = collector_collapse(cchar, &arr_count);
                collector_insert(ctokens, &string_arr);
                *len+=1;
                arr_count = 0;
                cchar = collector_init(sizeof(char), print_char);
            }   
        }
        
        if (input != '\n') {
            collector_insert(cchar, &input);
            arr_count++;
        }
        
       switched_markup = false;
    }

    char *temp_arr = collector_collapse(cchar, &arr_count);
    free(temp_arr);

    char **tokens_arr = collector_collapse(ctokens, len);
    return tokens_arr;

}