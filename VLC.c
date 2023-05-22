//file name:VLC.c
//this file contains the main algorithm for performing the vlc application.
//@author: Jishnuraj Prakasan
//@username:jp4154
//the header file "node_heap.h" contains some function declarations that we use 
//in this file along with the help of function defenitions in node_heap.c

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<math.h>
#include"node_heap.h"

//heap is a Heap type variable declared in the BSS.
//syms is a Symbol type array used to store the information about various symbols.

Heap heap;
Symbol syms[1000];

///this function is used to display the output in the required manner.
///@param n1 is the node that needs to be displayed that contains all the 
///symbols.
///@param z is a variable used as a flag to determine what needs to be
///displayed.
///@returns nothing.
///This function is specific to this file.

static void display(Node n1, int z)
{
    size_t i;int max=strlen(n1.syms[0].codeword);float avglen;
    float f=0;float s=0;float ans;avglen=0;ans=0;
    if(n1.num_valid==1)
    {
        ans=1;
    }
    for(i=0;i<n1.num_valid;i++)
    {   int l=strlen(n1.syms[i].codeword);
        if(l>max)
        {
            max=l;
        }
       
        if(z!=1) 
        {
            s=s+strlen(n1.syms[i].codeword)*n1.syms[i].frequency;
            f=f+n1.syms[i].frequency;
        }
        
    }    
    if(f!=0 && z!=1)
        avglen=s/f;
    printf("Variable Length Code Information\n");
    printf("================================\n");
    for(i=0;i<n1.num_valid;i++)
        {
            if(n1.syms[i].symbol=='\n')
                printf("symbol: '\\n' frequency: %9zu", n1.syms[i].frequency);
            else if(n1.syms[i].symbol=='\t')
            {
                printf("symbol: '\\t'  frequency: %9zu", n1.syms[i].frequency);
            }
            else if(n1.syms[i].symbol==' ')
                printf("symbol: ' '  frequency: %9zu", n1.syms[i].frequency);
            else if(!isprint(n1.syms[i].symbol))
            {
                unsigned char c=n1.syms[i].symbol;
                printf("symbol: 0x%02hhx frequency: %9zu",c,
                n1.syms[i].frequency);
            }
            else
            {

                printf("symbol: '%c'  frequency: %9zu",n1.syms[i].symbol,
                n1.syms[i].frequency);
            }
                printf("  codeword: %*s\n", max, n1.syms[i].codeword);
        }
        if(z!=1 && n1.num_valid!=1)
        {
             ans=ceil(log2(n1.num_valid));
        }
        printf("Average VLC code length:        %.4f\n",avglen);
        printf("Fixed length code length:       %.4f\n",ans);
        printf("Longest variable code length:   %d\n",max);
        printf("Node cumulative frequency:      %d\n",(int)f);
        printf("Number of distinct symbols:     %zu\n",n1.num_valid);
        
}

///this function helps to prepend the code for the application.
///it does that bit by bit.
///@param *s is the pointer that can store the address of Symbol
///type. The codeword needs to be made for that symbol.
///@param num is the number either '1' or '0' that needs to be prepended.
///@returns nothing.
///this function is specific to this file.


static void prepend(Symbol *s, char num)
{
    int k=s->bit;
    while(k-1>=0)
    {
        s->codeword[k]=s->codeword[k-1];
        k--;
    }
    s->codeword[k]=num;
    s->bit++;
}

///this is the main algorithm that is used to find unique and efficient
///codes for each symbol.
///@param *heap is the pointer to the heap in BSS.
///@returns nothing.
///this function is specific to this file.

static void variable_algo(Heap *heap)
{   int ch=0;
    Node n1,n2;int c=0;size_t j;
    int z=0; 
    while(heap->size>=2)
    {   ch=1;
        n1=heap_remove(heap);
        n2=heap_remove(heap);
        if(n1.frequency<=n2.frequency)
        {
            for(j=0;j<n1.num_valid;j++)
            {
                prepend(&n1.syms[j],'0');
            }
            for(j=0;j<n2.num_valid;j++)
            {
                prepend(&n2.syms[j],'1');
            }

        }
        else if(n1.frequency>n2.frequency)
        {
            for(j=0;j<n1.num_valid;j++)
            {
                prepend(&n1.syms[j],'1');
            }
            for(j=0;j<n2.num_valid;j++)
            {
                prepend(&n2.syms[j],'0');
            }

        }
    
        n2.frequency=n2.frequency+n1.frequency;
        for(j=n2.num_valid;j<n2.num_valid+n1.num_valid;j++)
        {
            n2.syms[j]=n1.syms[c];
            c++;
        }
        n2.num_valid=n2.num_valid+n1.num_valid;
        heap_add(heap,n2);
        c=0;
    }
    if(heap->size==0)
    {
        z=1;
        display(n1,z);
    }
    else if(heap->size==1 && ch==0)
    {
        n1=heap_remove(heap);
        prepend(&n1.syms[0],'0');
        display(n1,z);
    }
    else
    {
         n1=heap_remove(heap);

         display(n1,z);
    }
}

///this is the main function that calls different functions to 
///do the necessary jobs.
///@returns an integer value 0 on successful completion.
int main()
{
    int no;
    no= read_symbols(MAX_SYMS, syms); 
    heap_make(&heap, no-1, syms);
    variable_algo(&heap);
}

