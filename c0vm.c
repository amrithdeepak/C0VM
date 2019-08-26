#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#include "lib/xalloc.h"
#include "lib/contracts.h"
#include "lib/stacks.h"
#include "lib/c0vm.h"
#include "lib/c0vm_c0ffi.h"
#include "lib/c0vm_abort.h"

/* call stack frames */
typedef struct frame * frame;
struct frame {
  c0_value *V; /* local variables */
  stack S;     /* operand stack */
  ubyte *P;    /* function body */
  size_t pc;   /* return address */
};

/* TODO: implement execute function */
int execute(struct bc0_file *bc0) {
  
  /* Variables used for bytecode interpreter. You will need to initialize
     these appropriately. */
  /* callStack to hold frames when functions are called */
  stack callStack = stack_new();
  (void) callStack;
  /* initial program is the "main" function, function 0 (which must exist) */
  struct function_info *main_fn = bc0->function_pool;
  (void) (*main_fn);
  /* array to hold local variables for function */
  c0_value *V = xcalloc(main_fn->num_vars,sizeof(c0_value));
  (void) V;
  /* stack for operands for computations */
  stack S;
  /* array of (unsigned) bytes that make up the program */
  ubyte *P;
  /* program counter that holds "address" of next bytecode to interpret from
     program P */
  size_t pc;
  
  S = stack_new();
  P = ((bc0->function_pool)[0]).code;
  pc = 0;
  while (true) {
    
#ifdef DEBUG
    fprintf(stderr,
            "Opcode %x -- Operand stack size: %zu -- Program counter: %zu\n",
            P[pc], stack_size(S), pc);
#endif
    
    switch (P[pc]) {
      
      /* GENERAL INSTRUCTIONS: Implement the following cases for each of the
         possible bytecodes.  Read the instructions in the assignment to see
         how far you should go before you test your code at each stage.  Do
         not try to write all of the code below at once!  Remember to update
         the program counter (pc) for each case, depending on the number of
         bytes needed for each bytecode operation.  See PROG_HINTS.txt for
         a few more hints.
         
         IMPORTANT NOTE: For each case, the case should end with a break
         statement to prevent the execution from continuing on into the
         next case.  See the POP case for an example.  To introduce new
         local variables in a case, use curly braces to create a new block.
         See the DUP case for an example.
         
         See C_IDIOMS.txt for further information on idioms you may find
         useful.
      */
      
    /* Additional stack operation: */

    case POP: {
      pc++;
      pop(S);
      break;
    }

    case DUP: {
      pc++;
      c0_value v = pop(S);
      push(S,v);
      push(S,v);
      break;
    }
      
    case SWAP: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      push(S,v1);
      push(S,v2);
      break;
    }

    /* Returning from a function */

    case RETURN: {
      if(stack_empty(callStack)) {
      c0_value v1 = pop(S);
      stack_free(S, NULL);
      free(V);
      stack_free(callStack, NULL);
      return INT(v1);
      }
      else {
c0_value v1 = pop(S);
	struct frame* new = pop(callStack);
	free(V);
        stack_free(S,NULL);
     V= new->V;
     S= new->S;
     P= new->P;
     push(S,v1);
      pc = new->pc;
      pc+=3;
      free(new);
      }
      break;
    }

    /* Arithmetic and Logical operations */

    case IADD: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2+va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case ISUB: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2-va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IMUL: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2*va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IDIV: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      if(va1 == 0)
	c0_arith_error("Division by 0 error!");
      if((va2 == (INT_MIN)) && (va1==-1))
	c0_arith_error("Division by 0 error!");
      int va3 = va2/va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IREM: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      if(va1 == 0)
	c0_arith_error("Modulo by 0 error!");
      if((va2 == (INT_MIN)) && (va1==-1))
	c0_arith_error("Division by 0 error!");
      int va3 = va2%va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IAND: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2&va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IOR: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2|va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case IXOR: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      int va3 = va2^va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case ISHL: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      if(va1 < 0 || va1 > 31)
	c0_arith_error("Shifting by an invalid value error!");
      int va3 = va2<<va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
    case ISHR: {
      pc++;
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      int va1 = INT(v1);
      int va2 = INT(v2);
      if(va1 < 0 || va1 > 31)
	c0_arith_error("Shifting by an invalid value error!");
      int va3 = va2>>va1;
      c0_value v3 = VAL(va3);
      push(S,v3);
      break;
    }
      
      
    /* Pushing small constants */

    case BIPUSH: {
      int32_t x = (int32_t)((int8_t)(P[pc+1]));
      pc+=2;
      push(S, VAL(x));
      break;
    }

    /* Operations on local variables */

    case VLOAD: {
      c0_value v1 = V[P[pc+1]];
      push(S, v1);
      pc+=2;
      break;
    }
      
    case VSTORE:  {

      c0_value v1 = pop(S);
      V[P[pc+1]] = v1;
      pc+=2;
      break;

    }
      
    case ACONST_NULL: {
      c0_value v1 = NULL;
      push(S, v1);
      pc++;
      break;
    }
      
    case ILDC: {
      uint8_t va1 = P[pc+1];
      uint8_t va2 = P[pc+2]; 
      int va3 = bc0->int_pool[(uint16_t)((va1<<8)|va2)];
      c0_value v3 = VAL(va3);
      push(S,v3);
      pc+=3;
      break;
    }
      
    case ALDC: {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  char *va3 = &bc0->string_pool[(uint16_t)((va1<<8)|va2)];
      push(S,va3);
      pc+=3;
      break;
    }
      
      
    /* Control flow operations */

    case NOP: {
      pc++;
      break;
    }
      
    case IF_CMPEQ: {
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v1)) == (INT(v2))) {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;

    }
      
    case IF_CMPNE: {
     
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v1)) != (INT(v2))) {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;

    }
      
    case IF_ICMPLT: {

      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v2)) < (INT(v1))) {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;
    }
      
    case IF_ICMPGE: {
       c0_value  v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v2)) >= (INT(v1))) {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;

    }
      
    case IF_ICMPGT: {
       c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v2)) > (INT(v1))) {
          
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;
    }
      
    case IF_ICMPLE: {
      c0_value v1 = pop(S);
      c0_value v2 = pop(S);
      if((INT(v2)) <= (INT(v1))) {
          
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
      }
      else
         pc+=3;
      break;
    }
      
    case GOTO: {
          uint8_t va1 = P[pc+1];
          uint8_t va2 = P[pc+2];
	  pc = pc + (int16_t)((va1<<8|va2));
        break;
    }
      
    case ATHROW: {
      char *a1 = pop(S);
       c0_user_error(a1);
       pc++;
      break;
    }

    case ASSERT: {
      char* v1 = pop(S);
      c0_value v2 = pop(S);
      if((((INT)(v2)) == 0))
	c0_assertion_failure(v1);
      pc++;
      break;
    }

    /* Function call operations: */

    case INVOKESTATIC: {
      struct frame* old = xcalloc(1, sizeof(struct frame));
      old->V = V;
      old->S = S;
      old->P = P;
      old->pc = pc;
      push(callStack,old);
      uint8_t va1 = P[pc+1];
      uint8_t va2 = P[pc+2]; 
      struct function_info g = bc0->function_pool[(uint16_t)((va1<<8)|va2)];
      c0_value *v = xcalloc(g.num_vars, sizeof(c0_value));
      for (int i = g.num_args - 1; i >= 0; i--) {
	v[i] = pop(S);
      }
      V = v;
      S = stack_new();
      P = g.code;
      pc = 0;
      break;
    }
      
    case INVOKENATIVE: {

      uint8_t va1 = P[pc+1];
      uint8_t va2 = P[pc+2]; 
      struct native_info g = bc0->native_pool[(uint16_t)((va1<<8)|va2)];
      c0_value *v = xcalloc(g.num_args, sizeof(c0_value));
      for (int i = g.num_args - 1; i >= 0; i--) {
	v[i] = pop(S);
      }
      push(S,native_function_table[g.function_table_index](v));
      pc+=3;
      free(v);
    
      break;
    }
      

    /* Memory allocation operations: */

    case NEW: {
      int s = P[pc+1];
      c0_value a = xcalloc(1,s);
      push(S,a);
      pc+=2;
      break;
    }
      
    case NEWARRAY: {
      int s = P[pc+1];
      if(s < 0)
          c0_memory_error("Invalid array input!");
      c0_value b = pop(S);
      int size = INT(b);
      if(size < 0)
          c0_memory_error("Invalid array input!");
      c0_value a = xcalloc(size,s);
      struct c0_array_header *f = xmalloc(sizeof(struct c0_array_header));
      f->elems = a;
      f->count = size;
      f->elt_size = s;     
      push(S,(c0_value)(f));
      pc+=2;
      break;
    }
      
    case ARRAYLENGTH: {
      c0_value b = pop(S);
      struct c0_array_header *a = (struct c0_array_header*) (b);
      int size = a->count;
      push(S,VAL(size));
      pc++;
      break;
    }


    /* Memory access operations: */

    case AADDF: {
      int s = P[pc+1];
        c0_value b = pop(S);
        if(b == NULL)
            c0_memory_error("Dereferencing NULL pointer!");
      char *a = (char *)(b);
      push(S,(a+s));
      pc+=2;
      break;
    }
      
    case AADDS:  {
      
      c0_value a = pop(S);
      c0_value b = pop(S);
      if(b == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      struct c0_array_header *c = (struct c0_array_header*) (b);
      int x = INT(a);
      if(x < 0 || x >= c->count)
	c0_memory_error("Dereferencing NULL pointer!");
      // printf("%p    %d\n", c->elems, c->elt_size);
      c0_value f = (c0_value)(((char *)(c->elems)) + (c->elt_size) * x);
      push(S,f);
      //  printf("This is the c0 value being pushed onto the stack.  %p\n",f);
      
      pc++;
      break;
    }
      
    case IMLOAD: {
      c0_value a = pop(S);
  
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      int *x = (int *)(a);
      push(S, VAL(*x));
      pc++;
      break;
    }
      
    case IMSTORE: {
      c0_value b = pop(S);
      c0_value a = pop(S);
      // printf("%p",a);
      //printf("\n");
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      int x = INT(b);
      *((int *)(a)) = x;
      pc++;
      break;
    }
      
    case AMLOAD: {
      c0_value a = pop(S);
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      c0_value *b = (c0_value *)(a);
      push(S, *b);
      pc++;
      break;
    }
      
    case AMSTORE: {
      c0_value b = pop(S);
      c0_value a = pop(S);
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      *((c0_value *)(a)) = b;
      
      pc++;
      break;
    }
      
    case CMLOAD: {
      c0_value a = pop(S);
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      char *x = ((char *)(a));
      push(S, VAL(*x));
      pc++;
      break;
    }
      
    case CMSTORE: {
      c0_value b = pop(S);
      c0_value a = pop(S);
      if(a == NULL)
	c0_memory_error("Dereferencing NULL pointer!");
      int x = INT(b);
      *((char *)(a)) = x;
      pc++;
      break;
    }
      
    default:
      fprintf(stderr, "invalid opcode: 0x%02x\n", P[pc]);
      stack_free(S,NULL);
      free(V);
      stack_free(callStack,NULL);
      abort();
    }
  }
  
  /* cannot get here from infinite loop */
  assert(false);
}

