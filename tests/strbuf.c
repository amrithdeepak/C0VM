/* 
 * String Buffer Library
 *
 * 15-122 Principles of Imperative Computation
 * This version exposes the externals, requires
 * discipline from client! */

/*** Interface ***/

#include "strbuf.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"



/*** Implementation ***/
bool is_strbuf(struct strbuf* sb) {
    if(sb == NULL)
        return false;
    if(sb->limit <= 0)
        return false;
    if(sb->len >= sb->limit)
        return false;
    if(sb->buf == NULL)
      return false;
    if(strlen(sb->buf) != sb->len)
        return false;
    if(sb->buf[sb->len]!= '\0')
        return false;
    return true;
}

struct strbuf* strbuf_new(size_t initial_limit)
//@requires initial_limit > 0;
//@ensures is_strbuf(\result);
{
  REQUIRES (initial_limit > 0);
  struct strbuf* S = xmalloc(sizeof(struct strbuf));
    S->limit = initial_limit;
    S->buf = xcalloc(initial_limit, sizeof(char));
    S->len = 0;
    ENSURES (is_strbuf(S));
    return S;

}
char* strbuf_str(struct strbuf* str)
//@requires is_strbuf(str);
{
  REQUIRES (is_strbuf(str));
  char* strfull = xcalloc((str->len +1),sizeof(char));
    for(size_t i = 0; i <= str->len; i++) {
        strfull[i] = str->buf[i];
    }
    ENSURES (is_strbuf(str));
    return strfull;
}

void strbuf_add(struct strbuf* sb, char* str, size_t str_len)
//@requires is_strbuf(sb);
//@ensures is_strbuf(sb);
{
  REQUIRES (is_strbuf(sb));
  REQUIRES (str != NULL);
  REQUIRES (strlen(str) == str_len);
    if(sb->limit <= sb->len + str_len) {
      char* B = xcalloc(((sb->limit + str_len) * 2), sizeof(char));
        for(size_t i = 0; i < sb->len; i++) {
            B[i] = sb->buf[i];
        }
        free(sb->buf);
        sb->buf = B;
        sb->limit = (sb->limit + str_len) * 2;
    }
    for(size_t i = 0; i < str_len; i++) {
        sb->buf[sb->len+i] = str[i];
    }
    sb->buf[sb->len+str_len] = '\0';
    sb->len = sb->len + str_len;
    ENSURES (is_strbuf(sb));
}
void strbuf_addstr(struct strbuf* sb, char* str)
//@requires is_strbuf(sb);
//@ensures is_strbuf(sb);
{
  REQUIRES (is_strbuf(sb));
  REQUIRES (str != NULL);
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    strbuf_add(sb, str, len);
    ENSURES (is_strbuf(sb));
}
char *strbuf_dealloc(struct strbuf *sb)
{
  REQUIRES(is_strbuf(sb));
  char* temp = sb->buf;
  free(sb);
  return temp;
}
