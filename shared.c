/*
        DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
                    Version 2, December 2004

 Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>

 Everyone is permitted to copy and distribute verbatim or modified
 copies of this license document, and changing it is allowed as long
 as the name is changed.

            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION

  0. You just DO WHAT THE FUCK YOU WANT TO.
*/

#include <sys/param.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "shared.h"

/*
 * Function:  EnsureEbookDirExists
 * --------------------
 *  Check if the directory to store ebook exists.
 *  If not, create it in read write access mode for owner
 *
 *  no parameter.
 *
 *  ro returned value.
 */
void EnsureEbookDirExists(void) {
    struct stat fs_stat = {0};

    if (stat(ENTRY_MASK_DIR, &fs_stat) == -1) {
        mkdir(ENTRY_MASK_DIR, 0700);
    }
}

/*
 * Function:  StoreContent
 * --------------------
 * Store the "n" bytes of content of string source in string destination.
 * "n" is the content size capped by LIMIT_DYNAMIC_STRING.
 *
 *  src: pointer to the source string
 *  contentsize: the number of char to transfert
 *  dest: adress to write content
 *
 *  returns: WNDL_OK if everything is ok. An error plus WNDL_ERROR otherwise.
 */
wd_result StoreContent(const char* src, size_t contentsize, char** dest) {

    wd_result result = WNDL_OK;

    // Compute the amount of memory to allocate
    const  size_t  realsize = MIN(contentsize, LIMIT_DYNAMIC_STRING) * sizeof(char);

    // If dest is null
    if (NULL == *dest) {
        // then we allocate and fill with zeros
        *dest = calloc((realsize + 1), sizeof(char));
    } else {
        // pointer already exists, the we reallocate
        *dest = realloc(*dest, (realsize + 1) * sizeof(char));
    }

    // If memory allocation fails
    if (NULL == *dest) {
        // then show an error message
        fprintf(stderr, "%s", "not enough memory (realloc returned NULL)\n");
        // and stop here with and error
        result = WNDL_ERROR;
    } else {
        // Copy realsize bytes from src to dest
        strncpy(*dest, src, realsize);
        // just to be sure dest is really null terminated
        *(*dest + realsize) = 0;
    }

    return result;
}
