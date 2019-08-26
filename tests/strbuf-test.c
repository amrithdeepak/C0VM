#include "strbuf.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lib/contracts.h"
#include "lib/xalloc.h"

/*Free all arrays allocated, and free sb->buf*/

int main()
{
    struct strbuf* sb= strbuf_new(5);
    assert((is_strbuf(sb))==true);
    sb->len = -1;
    assert((is_strbuf(sb))==false);
    sb->len = 3;
    sb->buf[0] = 'C';
    sb->buf[1] = 'A';
    sb->buf[2] = 'T';
    sb->buf[3] = '\0';
    assert((is_strbuf(sb))==true);
    sb->len = 5;
    assert((is_strbuf(sb))==false);
    sb->len = 3;
    assert((is_strbuf(sb))==true);
    sb->len = 6;
    assert((is_strbuf(sb))==false);
    sb->len = 3;
    assert((is_strbuf(sb))==true);
    sb->len = 2;
    assert((is_strbuf(sb))==false);
    sb->len = 3;
    assert((is_strbuf(sb))==true);
    sb->buf[1] = '\0';
    assert((is_strbuf(sb))==false);
    sb->buf[1] = 'A';
    assert((is_strbuf(sb))==true);
    free(sb->buf);
    free(sb);
    sb = NULL;
    assert((is_strbuf(sb))==false);
   
    

    
    struct strbuf* sb2= strbuf_new(6);
    sb2->len = 4;
    sb2->buf[0] = 'C';
    sb2->buf[1] = 'A';
    sb2->buf[2] = 'T';
    sb2->buf[3] = 'S';
    sb2->buf[4] = '\0';
    char* supposed = strbuf_str(sb2);
    for(int i = 0; i < 5; i++) {
        assert(sb2->buf[i] == supposed[i]);            
    }
    assert(supposed != sb2->buf);

    struct strbuf* sb3= strbuf_new(8);
    sb3->buf[0] = 'E';
    sb3->buf[1] = 'L';
    sb3->buf[2] = 'E';
    sb3->buf[3] = 'P';
    sb3->buf[4] = '\0';
    sb3->len = 4;
    char* inputone = xmalloc(5*sizeof(char));
    inputone[0] = 'H';
    inputone[1] = 'A';
    inputone[2] = 'N';
    inputone[3] = 'T';
    inputone[4] = '\0';
    char* finalone = xmalloc(8*sizeof(char));
    finalone[0] = 'E';
    finalone[1] = 'L';
    finalone[2] = 'E';
    finalone[3] = 'P';
    finalone[4] = 'H';
    finalone[5] = 'A';
    finalone[6] = 'N';
    finalone[7] = 'T';
    strbuf_add(sb3, inputone, 4);

    for(int i = 0; i < 8; i++) {
      assert(finalone[i] == sb3->buf[i]);\
    }
    
    struct strbuf* sb4= strbuf_new(8);
    sb4->buf[0] = 'E';
    sb4->buf[1] = 'L';
    sb4->buf[2] = 'E';
    sb4->buf[3] = 'P';
    sb4->buf[4] = '\0';
    sb4->len = 4;
    char* inputtone = xmalloc(5*sizeof(char));
    inputtone[0] = 'H';
    inputtone[1] = 'A';
    inputtone[2] = 'N';
    inputtone[3] = 'T';
    inputtone[4] = '\0';
    char* finallone = xmalloc(8*sizeof(char));
    finallone[0] = 'E';
    finallone[1] = 'L';
    finallone[2] = 'E';
    finallone[3] = 'P';
    finallone[4] = 'H';
    finallone[5] = 'A';
    finallone[6] = 'N';
    finallone[7] = 'T';
    strbuf_addstr(sb4, inputtone);
    for(int i = 0; i < 8; i++) {
        assert(finallone[i] == sb4->buf[i]);
    } 
   
    struct strbuf* sb5= strbuf_new(8);
    sb5->buf[0] = 'E';
    sb5->buf[1] = 'L';
    sb5->buf[2] = 'E';
    sb5->buf[3] = 'P';
    sb5->buf[4] = '\0';
    sb5->len = 4;
    char* inputtwo = xmalloc(6*sizeof(char));
    inputtwo[0] = 'H';
    inputtwo[1] = 'A';
    inputtwo[2] = 'N';
    inputtwo[3] = 'T';
    inputtwo[4] = 'S';
    inputtwo[5] = '\0';
    char* finaltwo = xmalloc(9*sizeof(char));
    finaltwo[0] = 'E';
    finaltwo[1] = 'L';
    finaltwo[2] = 'E';
    finaltwo[3] = 'P';
    finaltwo[4] = 'H';
    finaltwo[5] = 'A';
    finaltwo[6] = 'N';
    finaltwo[7] = 'T';
    finaltwo[8] = 'S';
    strbuf_add(sb5, inputtwo, 5);
    for(int i = 0; i < 9; i++) {
        assert(finaltwo[i] == sb5->buf[i]);
    }
    free(supposed); 
    free(sb2->buf);   
    free(sb2);
    free(sb3->buf);
    free(sb3);
    free(sb4->buf);
    free(sb4);
    free(sb5->buf);
    free(sb5);
    free(inputone);
    free(finalone);
    free(inputtone);
    free(finallone);
    free(inputtwo);
    free(finaltwo);
    printf("ALL TESTS PASSED\n");
    return 0;
}
