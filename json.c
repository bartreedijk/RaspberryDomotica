/* 
 * File:   json.c
 * Author: bart en gerjan
 *
 * Created on 7 april 2015, 13:58
 */

#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <string.h>


json_t* loadJson() {
    
    json_t *root;
    json_error_t error;
    
    root = json_load_file("/var/www/lights.json", 0, &error);
    if(!root)
    {
        fprintf(stderr, "error: on line %d: %s\n", error.line, error.text);
        exit(0);
    }  
    else {
        return root;
    }
}
