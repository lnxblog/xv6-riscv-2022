Read the code in call.asm for the functions g, f, and main. The instruction manual for RISC-V is on the reference page. 
Here are some questions that you should answer (store the answers in a file answers-traps.txt):

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
  printf("%d %d\n", f(8)+1, 13);
  exit(0);
}

int g(int x) {
   0:	1141                	add	sp,sp,-16
   2:	e422                	sd	s0,8(sp)
   4:	0800                	add	s0,sp,16
  return x+3;
}
   6:	250d                	addw	a0,a0,3
   8:	6422                	ld	s0,8(sp)
   a:	0141                	add	sp,sp,16
   c:	8082                	ret

000000000000000e <f>:

int f(int x) {
   e:	1141                	add	sp,sp,-16
  10:	e422                	sd	s0,8(sp)
  12:	0800                	add	s0,sp,16
  return g(x);
}
  14:	250d                	addw	a0,a0,3
  16:	6422                	ld	s0,8(sp)
  18:	0141                	add	sp,sp,16
  1a:	8082                	ret

000000000000001c <main>:

void main(void) {
  1c:	1141                	add	sp,sp,-16
  1e:	e406                	sd	ra,8(sp)
  20:	e022                	sd	s0,0(sp)
  22:	0800                	add	s0,sp,16
  printf("%d %d\n", f(8)+1, 13);
  24:	4635                	li	a2,13
  26:	45b1                	li	a1,12
  28:	00000517          	auipc	a0,0x0
  2c:	7a850513          	add	a0,a0,1960 # 7d0 <malloc+0xe8>
  30:	00000097          	auipc	ra,0x0
  34:	600080e7          	jalr	1536(ra) # 630 <printf>
  exit(0);
  38:	4501                	li	a0,0
  3a:	00000097          	auipc	ra,0x0
  3e:	28e080e7          	jalr	654(ra) # 2c8 <exit>


1. Which registers contain arguments to functions? For example, which register holds 13 in main's call to printf? 
  24:	4635                	li	a2,13
  Register a2

2. Where is the call to function f in the assembly code for main? Where is the call to g? (Hint: the compiler may inline functions.) 

      26:	45b1                	li	a1,12
      line 0x26. There is no call and the result is obtained at compile time.
    ## Call to g() ##
      14:	250d                	addw	a0,a0,3
      Function g() has been inlined

3. At what address is the function printf located? 
    0x630
4. What value is in the register ra just after the jalr to printf in main? 
    address of instruction after the call
    0x38
5. 

