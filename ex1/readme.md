段基本信息
```
tangke@tangke:~/build_example/ex1$ readelf -S ex1.o
```
```
There are 14 section headers, starting at offset 0x4b8:

Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .text             PROGBITS         0000000000000000  00000040
       0000000000000053  0000000000000000  AX       0     0     1
  [ 2] .rela.text        RELA             0000000000000000  000003c8
       0000000000000048  0000000000000018   I      11     1     8
  [ 3] .data             PROGBITS         0000000000000000  00000094
       000000000000000c  0000000000000000  WA       0     0     4
  [ 4] .bss              NOBITS           0000000000000000  000000a0
       0000000000000008  0000000000000000  WA       0     0     4
  [ 5] .rodata           PROGBITS         0000000000000000  000000a0
       0000000000000004  0000000000000000   A       0     0     1
  [ 6] .comment          PROGBITS         0000000000000000  000000a4
       000000000000002c  0000000000000001  MS       0     0     1
  [ 7] .note.GNU-stack   PROGBITS         0000000000000000  000000d0
       0000000000000000  0000000000000000           0     0     1
  [ 8] .note.gnu.propert NOTE             0000000000000000  000000d0
       0000000000000020  0000000000000000   A       0     0     8
  [ 9] .eh_frame         PROGBITS         0000000000000000  000000f0
       0000000000000058  0000000000000000   A       0     0     8
  [10] .rela.eh_frame    RELA             0000000000000000  00000410
       0000000000000030  0000000000000018   I      11     9     8
  [11] .symtab           SYMTAB           0000000000000000  00000148
       00000000000001e0  0000000000000018          12    14     8
  [12] .strtab           STRTAB           0000000000000000  00000328
       0000000000000099  0000000000000000           0     0     1
  [13] .shstrtab         STRTAB           0000000000000000  00000440
       0000000000000074  0000000000000000           0     0     1
Key to Flags:
  W (write), A (alloc), X (execute), M (merge), S (strings), I (info),
  L (link order), O (extra OS processing required), G (group), T (TLS),
  C (compressed), x (unknown), o (OS specific), E (exclude),
  l (large), p (processor specific)
  ```
>强符号：函数和全局已初始化的变量；弱符号：未初始化的全局变量。

.data+.bss+.rodata总共24B,对应6个int,就是代码中的4个全局int和2个局部static int。main函数中的abc三个int是局部符号，链接器不用处理，因此不纳入符号表。那么具体哪些变量存在了哪些段呢？

```
tangke@tangke:~/build_example/ex1$ readelf -s ex1.o
```
```
Symbol table '.symtab' contains 20 entries:
   Num:    Value          Size Type    Bind   Vis      Ndx Name
     0: 0000000000000000     0 NOTYPE  LOCAL  DEFAULT  UND
     1: 0000000000000000     0 FILE    LOCAL  DEFAULT  ABS ex1.c
     2: 0000000000000000     0 SECTION LOCAL  DEFAULT    1
     3: 0000000000000000     0 SECTION LOCAL  DEFAULT    3
     4: 0000000000000000     0 SECTION LOCAL  DEFAULT    4
     5: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 static_global_init
     6: 0000000000000000     4 OBJECT  LOCAL  DEFAULT    4 static_global_unint
     7: 0000000000000000     0 SECTION LOCAL  DEFAULT    5
     8: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    4 stativ_car2.1922
     9: 0000000000000008     4 OBJECT  LOCAL  DEFAULT    3 static_var.1921
    10: 0000000000000000     0 SECTION LOCAL  DEFAULT    7
    11: 0000000000000000     0 SECTION LOCAL  DEFAULT    8
    12: 0000000000000000     0 SECTION LOCAL  DEFAULT    9
    13: 0000000000000000     0 SECTION LOCAL  DEFAULT    6
    14: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 global_init_var
    15: 0000000000000004     4 OBJECT  GLOBAL DEFAULT  COM global_uninit_var
    16: 0000000000000000    40 FUNC    GLOBAL DEFAULT    1 func1
    17: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND _GLOBAL_OFFSET_TABLE_
    18: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf
    19: 0000000000000028    43 FUNC    GLOBAL DEFAULT    1 main
```
可以看到：全局初始化了的(无论是否static)/局部初始化了的在section3，即.data；全局未初始化的(static)/局部未初始化的在section4，即.bss；全局未初始化的变量，是弱符号，因此放在COMMON块中。
.text段
```
tangke@tangke:~/build_example/ex1$ objdump -s -d ex1.o
```
```
ex1.o:     file format elf64-x86-64

Contents of section .text:
 0000 f30f1efa 554889e5 4883ec10 897dfc8b  ....UH..H....}..
 0010 45fc89c6 488d3d00 000000b8 00000000  E...H.=.........
 0020 e8000000 0090c9c3 f30f1efa 554889e5  ............UH..
 0030 4883ec10 c745f801 000000c7 45fc0000  H....E......E...
 0040 0000bf0a 000000e8 00000000 b8000000  ................
 0050 00c9c3                               ...
Contents of section .data:
 0000 54000000 0a000000 55000000           T.......U...
Contents of section .rodata:
 0000 25640a00                             %d..
Contents of section .comment:
 0000 00474343 3a202855 62756e74 7520392e  .GCC: (Ubuntu 9.
 0010 342e302d 31756275 6e747531 7e32302e  4.0-1ubuntu1~20.
 0020 30342e31 2920392e 342e3000           04.1) 9.4.0.
Contents of section .note.gnu.property:
 0000 04000000 10000000 05000000 474e5500  ............GNU.
 0010 020000c0 04000000 03000000 00000000  ................
Contents of section .eh_frame:
 0000 14000000 00000000 017a5200 01781001  .........zR..x..
 0010 1b0c0708 90010000 1c000000 1c000000  ................
 0020 00000000 28000000 00450e10 8602430d  ....(....E....C.
 0030 065f0c07 08000000 1c000000 3c000000  ._..........<...
 0040 00000000 2b000000 00450e10 8602430d  ....+....E....C.
 0050 06620c07 08000000                    .b......

Disassembly of section .text:

0000000000000000 <func1>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   48 83 ec 10             sub    $0x10,%rsp
   c:   89 7d fc                mov    %edi,-0x4(%rbp)
   f:   8b 45 fc                mov    -0x4(%rbp),%eax
  12:   89 c6                   mov    %eax,%esi
  14:   48 8d 3d 00 00 00 00    lea    0x0(%rip),%rdi        # 1b <func1+0x1b>
  1b:   b8 00 00 00 00          mov    $0x0,%eax
  20:   e8 00 00 00 00          callq  25 <func1+0x25>
  25:   90                      nop
  26:   c9                      leaveq
  27:   c3                      retq

0000000000000028 <main>:
  28:   f3 0f 1e fa             endbr64
  2c:   55                      push   %rbp
  2d:   48 89 e5                mov    %rsp,%rbp
  30:   48 83 ec 10             sub    $0x10,%rsp
  34:   c7 45 f8 01 00 00 00    movl   $0x1,-0x8(%rbp)
  3b:   c7 45 fc 00 00 00 00    movl   $0x0,-0x4(%rbp)
  42:   bf 0a 00 00 00          mov    $0xa,%edi
  47:   e8 00 00 00 00          callq  4c <main+0x24>
  4c:   b8 00 00 00 00          mov    $0x0,%eax
  51:   c9                      leaveq
  52:   c3                      retq

  ```

