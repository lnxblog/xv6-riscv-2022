 ## Q & A ##
 1. Looking at the backtrace output, which function called syscall? 
 #0  syscall () at kernel/syscall.c:133
 #1  0x0000000080001d18 in usertrap () at kernel/trap.c:67
 #2  0x0505050505050505 in ?? ()

 2. What is the value of p->trapframe->a7 and what does that value represent? (Hint: look user/initcode.S, the first user program xv6 starts.) 
    p p->trapframe->a7
    $2 = 7
    #define SYS_exec    7
    Represents exec call

 3. What was the previous mode that the CPU was in? 
    p/x $sstatus
    0x200000022
    **SPP bit is bit 8 which is zero here. so previously in user mode.**
    The sstatus register is an XLEN-bit read/write register formatted as shown in Figure 4.1 for RV32
    and Figure 4.2 for RV64 and RV128. The sstatus register keeps track of the processor’s current
    operating state.
    The SPP bit indicates the privilege level at which a hart was executing before entering supervisor
    mode. When a trap is taken, SPP is set to 0 if the trap originated from user mode, or 1 otherwise.
    When an SRET instruction (see Section 3.2.2) is executed to return from the trap handler, the
    privilege level is set to user mode if the SPP bit is 0, or supervisor mode if the SPP bit is 1; SPP
    is then set to 0.

4. Write down the assembly instruction the kernel is panicing at. Which register corresponds to the varialable num? 
    sepc=0x0000000080001ff8 stval=0x0000000000000000
    num = * (int *) 0;
    80001ff8:	00002683          	lw	a3,0(zero) # 0 <_entry-0x80000000>
