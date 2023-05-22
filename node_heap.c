//file node_heap.c
//author: Jishnuraj Prakasan
//This file provides the basic functions to 
//implement the VLC program operations.
//user id: jp4154
//the "node_heap.h" header file contains the function declarations that
//is implememted in this file.

#include<stdio.h>
#include<stdbool.h>
#include"node_heap.h"
#include<string.h>

///this function is specific to this file. 
///@param i is the index for ehich parent index has to be found.
///@returns the index of the parent

static int Parent_index(int i)
{
    
    //printf("*****%d\n*****",(i-1)/2);
    return (i-1)/2;
}

///this function is specific to this file.
///@param i is the index 
///@returns the index of the left child of the parent.

static int Left_Child_Index(int i)
{
    return (2*i)+1;
}

///this function is specific to this file.
///@param i is the index.
///@returns the index of the right child of the parent.

static int Right_Child_Index(int i)
{
    return (2*i)+2;
}

///This function reads the set of symbols from the file, calculates 
///the frequency of unique symbols and store it in structure arrays.
///@param maxcount is the maximum capacity of the syms array
///@param syms array stores the information about the symbols.
///@returns an integer pointing to the last location of the syms array.

int read_symbols(size_t maxcount, Symbol syms[])
{
    size_t i,j;int t=0;
    size_t end=1;int k=0;
    char line[1000];
    for(j=0;j<maxcount;j++)
    {
        syms[j].symbol=NUL;
        syms[j].frequency=0;
        syms[j].bit=1;
        syms[j].codeword[0]='\0';
    }
    syms[0].symbol='\0';
    while(fgets(line,100,stdin))
    {   t++; 
        for(i=0;i<strlen(line);i++)
        {
            for(j=0;j<end;j++)
            {
                if(syms[j].symbol==line[i])
                {
                    syms[j].frequency++;
                    k=1;
                    break;
                }
            }
              if(k==0) 
            {
                syms[end-1].symbol=line[i];
                syms[end-1].frequency++;
                syms[end].symbol='\0';
                end++;
            }
            k=0;
        }
 
    }
    if(t==1 && line[strlen(line)-1]=='\n')
    {
        syms[strlen(line)-1].symbol=NUL;
        return end-1;
    }

    for(j=0;j<end;j++)
    {
        if(syms[j].symbol=='\n')
        {
           syms[j].frequency--;
           break;
        }
    }

return end;
}

///This function heapifies the nodes that are passed on to it.
///@param *heap is the pointer pointing to the heap location in the BSS.
///@length is the length of the total number of nodes to be heapified.
///This function passes its values to heap_add that adds the node to the heap.
///@param symlist is the list of symbols that needs to be included
///in the nodes. Initially each node is a symbol.
///@ returns nothing.

void heap_make(Heap *heap, size_t length, Symbol symlist[])
{
    size_t i;
    Node n;
    heap_init(heap);
    for(i=0;i<length;i++)
    {
        n.frequency=symlist[i].frequency;
        n.syms[0]=symlist[i];
        n.num_valid=1;
        heap_add(heap,n);
    }


}

///This function initializes the heap.
///@param *heap is a pointer to heap variable in BSS.
///@returns nothing.

void heap_init(Heap *heap)
{
    heap->size=0;
    heap->capacity=MAX_SYMS;
}

///This function helps to swap two nodes.
///@param *heap is the pointer to heap variable in BSS.
///@param index1 is the first index to be swapped.
///@param index2 is the second index.
///@returns  nothing.

static void swap(Heap *heap, size_t index1, size_t index2)
{
    Node  copy;
    copy=heap->array[index1];
    heap->array[index1]=heap->array[index2];
    heap->array[index2]=copy;
}

///This function compares two nodes on the basis of frequencies and 
//take a decision.
///@param *heap is the pointer to the heap variable in BSS.
///@param index1 is the first index of the Node array.
///@param index2 is the second index of the Node array and both are compared.
///@returns true or false.

static bool compare_nodes(Heap *heap,size_t index1, size_t index2)
{
    if(heap->array[index2].frequency >
          heap->array[index1].frequency)
    {
        return true;
    }
    else
    {
        return false;
    }

}

///This functions helps to heapify the nodes when a new one is added.
///It shidts up the nodes to form a min heap. This function is specific
///to this file too.
///@param *heap is the pointer to the heap in BSS.
///@param index is the index of the new node.
///@returns nothing.

static void shift_up(Heap *heap, int index)
{
    int i=index;
    bool ans;
    ans= compare_nodes(heap, i, Parent_index(i));
    while(ans==true && i> 0)
    {
        swap(heap, i, Parent_index(i));
        i=Parent_index(i);
        ans=compare_nodes(heap, i, Parent_index(i));
    }
}

///This function helps to choose from leftchild,rightchild or
///none when doing heapify.
///@param *heap is the pointer to the heap in BSS.
///@param curr is the index of the parent element.
///@returns an integer as the index to be swapped.

static int choose(Heap *heap, int curr)
{
    size_t lind=Left_Child_Index(curr);
    size_t rind=Right_Child_Index(curr);
    if(rind<heap->size)
    {
                
        if(compare_nodes(heap, lind, curr) || compare_nodes(heap, rind, curr))
        {
            if(compare_nodes(heap, lind, rind))
            {
                return lind;
            }
            else
            {
                return rind;
            }
        }
        else
        {
            return curr;
        }
    }
    else if(lind<heap->size)
    {

        if(compare_nodes(heap, lind, curr))
        {
            return lind;
        }
        else
        {
            return curr;
        }
        
    }
    else
    {
        return curr;
    }

}

///this function helps to heapify when a node is removed from the top.
///Also, this function is specific to this file.
///@param *heap is the pointer to the heap variable in BSS.
///@param index is always 0 that is the index of the root.
///@returns nothing.

static void shift_down(Heap *heap, int index)
{
    int curr=index;
    
    int swapind= choose(heap, curr);
    while(swapind!=curr)
    {
        swap(heap, swapind, curr);
        curr=swapind;
        swapind=choose(heap, curr);
    }



}

///this function helps to add a new node to the heap.
///It is responsible for calling the shift_up function when needed
///and increases the size of the heap.
///@param *heap is the pointer to the heap in BSS.
///@param node is then node that is to be added.
///@returns nothing.

void heap_add(Heap *heap, Node node)
{
    if(heap->size<heap->capacity)
    {
        heap->array[heap->size]=node;
        shift_up(heap, heap->size);
    }
    heap->size=heap->size+1;
}

///this function removes a node from the heap.
///@param *heap is the pointer to the heap variable in BSS.
///@returns the root node after heapify.

Node heap_remove(Heap *heap)
{
    Node fir= heap->array[0];
    heap->size=heap->size-1;
    heap->array[0]=heap->array[heap->size];
    shift_down(heap, 0);
    return fir;
}
