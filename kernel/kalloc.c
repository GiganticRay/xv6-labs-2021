// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "riscv.h"
#include "defs.h"

void freerange(void *pa_start, void *pa_end);

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {  // unit element of the free list.
  struct run *next;
};

struct {  // the lock and the freelist are wrapped in a struct to make clear that the lock pretects the fields in the struct.
  struct spinlock lock;
  struct run *freelist;
} kmem;

void
kinit() // initializes the free list to hold every page between the end of the kernel and PHYSTOP.
{
  initlock(&kmem.lock, "kmem");
  freerange(end, (void*)PHYSTOP);
}

void
freerange(void *pa_start, void *pa_end) // add memory to the free-list via per-page calls to kfree.
{
  char *p;
  p = (char*)PGROUNDUP((uint64)pa_start); // why use pageGroundUp?  : 相当于约定俗成，pa_start 可能是随意指定的，没有对齐，约定是从比它大的第一个对齐的页开始 free.
  for(; p + PGSIZE <= (char*)pa_end; p += PGSIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by v,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.). Attention, the pa has already been aligned.
void
kfree(void *pa)
{
  struct run *r;

  if(((uint64)pa % PGSIZE) != 0 || (char*)pa < end || (uint64)pa >= PHYSTOP)
    panic("kfree");

  // Fill with junk to catch dangling refs. set every byte in the memory being freed to the value 1. This will cause code that uses memory after freeing it to read garbage instead of old valid contents.
  memset(pa, 1, PGSIZE);

  r = (struct run*)pa;  // 这个直接类型强转，有点东西的啊，就是直接将 r 所需要的内存空间，存到 pa 所指的内存里。

  acquire(&kmem.lock);
  r->next = kmem.freelist;  // 头插 
  kmem.freelist = r;
  release(&kmem.lock);
}

// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated. removes and returns the first element in the free list.
void *
kalloc(void)
{
  struct run *r;

  acquire(&kmem.lock);
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;
  release(&kmem.lock);

  if(r)
    memset((char*)r, 5, PGSIZE); // fill with junk
  return (void*)r;
}
