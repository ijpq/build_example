```
tangke@tangke:~/build_example/ex2$ objdump -d a.o
```
```
a.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <call_func>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   90                      nop
   9:   5d                      pop    %rbp
   a:   c3                      retq
   ```

```
tangke@tangke:~/build_example/ex2$ objdump -d b.o
```
```
b.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:   f3 0f 1e fa             endbr64
   4:   55                      push   %rbp
   5:   48 89 e5                mov    %rsp,%rbp
   8:   c7 05 *00 00 00 00* 0a    movl   $0xa,0x0(%rip)        # 12 <main+0x12>
   f:   00 00 00
  12:   b8 00 00 00 00          mov    $0x0,%eax
  17:   e8 *00 00 00 00*          callq  1c <main+0x1c>
  1c:   b8 00 00 00 00          mov    $0x0,%eax
  21:   5d                      pop    %rbp
  22:   c3                      retq
  ```

```
tangke@tangke:~/build_example/ex2$ objdump -d ab.out
  // 省略部分内容
  0000000000001134 <main>:
    1134:       f3 0f 1e fa             endbr64
    1138:       55                      push   %rbp
    1139:       48 89 e5                mov    %rsp,%rbp
    113c:       c7 05 ca 2e 00 00 0a    movl   $0xa,0x2eca(%rip)        # 4010 <shared>
    1143:       00 00 00
    1146:       b8 00 00 00 00          mov    $0x0,%eax
    114b:       e8 d9 ff ff ff          callq  1129 <call_func>
    1150:       b8 00 00 00 00          mov    $0x0,%eax
    1155:       5d                      pop    %rbp
    1156:       c3                      retq
    1157:       66 0f 1f 84 00 00 00    nopw   0x0(%rax,%rax,1)
    115e:       00 00
    ```
    可以看到重定位之后，shared和call_func的虚拟地址都确定了