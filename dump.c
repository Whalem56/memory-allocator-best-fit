#include <stdio.h>
typedef struct block_hd{
  struct block_hd* next;
  int size_status;
}block_header;
extern block_header* list_head;
#ifndef alloc_size
#define alloc_size 4096
#endif

void Safe_Dump()
{
  int counter;
  block_header* current = NULL;
  char* t_Begin = NULL;
  char* Begin = NULL;
  int Size;
  int t_Size;
  char* End = NULL;
  int free_size;
  int busy_size;
  int total_size;

  free_size = 0;
  busy_size = 0;
  total_size = 0;
  current = list_head;
  counter = 1;
  fprintf(stderr,"************************************************Block list************************************************\n");
  fprintf(stderr,"No.\tt_Begin\t\tBegin\t\tsize_status\tStatus\tSize\tt_Size\tEnd\t\tnext\n");
  fprintf(stderr,"----------------------------------------------------------------------------------------------------------\n");
  while(NULL != current)
  {
    fprintf(stderr,"%d\t", counter);
    // Assume the current pointer is correct when we start a new iteration
    t_Begin = (char*)current;
    Begin = t_Begin + (int)sizeof(block_header);
    fprintf(stderr,"0x%08lx\t", (unsigned long)t_Begin);
    fprintf(stderr,"0x%08lx\t", (unsigned long)Begin);
    Size = current->size_status;
    fprintf(stderr,"%d\t\t", Size);
    if(Size & 2)
    {
      fprintf(stderr,"Err\t");
      fprintf(stderr,"\nError: The 2nd-LSB is expected to be 0, but it is not.\n");
      return;
    }
    if(Size & 1) /*LSB = 1 => busy block*/
    {
      fprintf(stderr,"Busy\t");
      Size = Size - 1; /*Minus one for ignoring status in busy block*/
      t_Size = Size + (int)sizeof(block_header);
      busy_size = busy_size + t_Size;
    }
    else
    {
      fprintf(stderr,"Free\t");
      t_Size = Size + (int)sizeof(block_header);
      free_size = free_size + t_Size;
    }
    fprintf(stderr,"%d\t", Size);
    if(Size <= 0)
    {
      fprintf(stderr,"\nError: Size is expected to be > 0, but it is not.\n");
      return;
    }
    fprintf(stderr,"%d\t", t_Size);
    total_size = total_size + t_Size;
    if(total_size > alloc_size)
    {
        fprintf(stderr,"\nError: total_size is expected to be %d, but it is at least %d.\n", alloc_size, total_size);
        return;
    }
    End = Begin + Size;
    fprintf(stderr,"0x%08lx\t", (unsigned long)End);
    char* next = (char*)(current->next);
    fprintf(stderr,"0x%08lx\n", (unsigned long)next);
    if(next != End && next != NULL)
    {
      fprintf(stderr,"\nError: next is expected to be the same as End or NULL, but it is not.\n");
      return;
    }
    current = current->next;
    counter = counter + 1;
  }
  fprintf(stderr,"----------------------------------------------------------------------------------------------------------\n");
  fprintf(stderr,"**********************************************************************************************************\n");

  fprintf(stderr,"Total busy size = %d\n",busy_size);
  fprintf(stderr,"Total free size = %d\n",free_size);
  fprintf(stderr,"Total size = %d\n",busy_size+free_size);
  fprintf(stderr,"**********************************************************************************************************\n");
  // fflush(stdout);
  return;

}

