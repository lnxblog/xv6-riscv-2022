#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  backtrace();
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}


uint64 sys_sigalarm()
{
  struct proc *p = myproc();

  argint(0,&(p->interval));
  argaddr(1,&(p->alarm_handler));
  p->ticks = 0;
  p->handler_active = 0;
  return 0;
}

uint64 sys_sigreturn()
{
  // load saved pc before timer intr occured
  struct proc *p = myproc();
  p->trapframe->ra = p->alarm_regs.ra;
  p->trapframe->sp = p->alarm_regs.sp;
  p->trapframe->gp = p->alarm_regs.gp;
  p->trapframe->tp = p->alarm_regs.tp;
  p->trapframe->t0 = p->alarm_regs.t0;
  p->trapframe->t1 = p->alarm_regs.t1;
  p->trapframe->t2 = p->alarm_regs.t2;
  p->trapframe->s0 = p->alarm_regs.s0;
  p->trapframe->s1 = p->alarm_regs.s1;
  p->trapframe->a1 = p->alarm_regs.a1;
  p->trapframe->a2 = p->alarm_regs.a2;
  p->trapframe->a3 = p->alarm_regs.a3;
  p->trapframe->a4 = p->alarm_regs.a4;
  p->trapframe->a5 = p->alarm_regs.a5;
  p->trapframe->a6 = p->alarm_regs.a6;
  p->trapframe->a7 = p->alarm_regs.a7;
  p->trapframe->s2 = p->alarm_regs.s2;
  p->trapframe->s3 = p->alarm_regs.s3;
  p->trapframe->s4 = p->alarm_regs.s4;
  p->trapframe->s5 = p->alarm_regs.s5;
  p->trapframe->s6 = p->alarm_regs.s6;
  p->trapframe->s7 = p->alarm_regs.s7;
  p->trapframe->s8 = p->alarm_regs.s8;
  p->trapframe->s9 = p->alarm_regs.s9;
  p->trapframe->s10 = p->alarm_regs.s10;
  p->trapframe->s11 = p->alarm_regs.s11;
  p->trapframe->t4 = p->alarm_regs.t4;
  p->trapframe->t5 = p->alarm_regs.t5;
  p->trapframe->t6 = p->alarm_regs.t6;
  p->trapframe->a0 = p->alarm_regs.a0;
  p->trapframe->epc = p->alarm_regs.epc;
  
  p->handler_active = 0;
  return 0;
}