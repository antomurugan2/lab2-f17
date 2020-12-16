#include "param.h"
#include "types.h"
#include "defs.h"
#include "x86.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"
#include "spinlock.h"

struct {
  struct spinlock lock;
  struct shm_page {
    uint id;
    char *frame;
    int refcnt;
  } shm_pages[64];
} shm_table;

void shminit() {
  int i;
  initlock(&(shm_table.lock), "SHM lock");
  acquire(&(shm_table.lock));
  for (i = 0; i< 64; i++) {
    shm_table.shm_pages[i].id =0;
    shm_table.shm_pages[i].frame =0;
    shm_table.shm_pages[i].refcnt =0;
  }
  release(&(shm_table.lock));
}

int shm_open(int id, char **pointer) {

//you write this

  // Acquire lock on the shm table
  acquire(&(shm_table.lock));
  struct proc* currProc = myproc();

  // Find the entry matching the id
  int i;
  for(i = 0; i < 64; i++) {
    //  If the entry is found then a process has already allocated a physical page 
    // Increment the reference count of that page
    if(shm_table.shm_pages[i].id == id) {
      char* memory = shm_table.shm_pages[i].frame;
      uint vir = PGROUNDUP(currProc->sz);
      mappages(currProc->pgdir, (void*)vir, PGSIZE, V2P(memory), PTE_W|PTE_U);
      shm_table.shm_pages[i].refcnt++;
      *pointer = (char*) vir;
      currProc->sz = PGROUNDUP(currProc->sz) + PGSIZE; 

      // Release lock on the shm_table
      release(&(shm_table.lock));
      return 0;
    }
  }
  
  // No entry is found
  shm_table.shm_pages[id-1].id = id;
  shm_table.shm_pages[id-1].frame = kalloc(); 
  memset(shm_table.shm_pages[id-1].frame, 0, PGSIZE);

  
  char* memory = shm_table.shm_pages[id-1].frame;
  uint vir = PGROUNDUP(currProc->sz);
  mappages(currProc->pgdir, (void*)vir, PGSIZE, V2P(memory), PTE_W|PTE_U);
  shm_table.shm_pages[id-1].refcnt = 1;
  *pointer = (char*) vir;
  currProc->sz = PGROUNDUP(currProc->sz) + PGSIZE; 
  
  // Release lock on the shm_table
  release(&(shm_table.lock));
  return 0; //added to remove compiler warning -- you should decide what to return
}


int shm_close(int id) {
//you write this too!




return 0; //added to remove compiler warning -- you should decide what to return
}
