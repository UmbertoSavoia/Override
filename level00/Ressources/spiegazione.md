# Level 00
Controlliamo le stringhe presenti nel binario:
```
level00@OverRide:~$ readelf -p '.rodata' level00

String dump of section '.rodata':
  [     8]  ***********************************
  [    2c]  * ^I     -Level00 -^I^I  *
  [    44]  Password:
  [    4e]  %d
  [    52]  Authenticated!
  [    61]  /bin/sh
  [    6a]  Invalid Password!
```
Analizziamo con `gdb`:
```
level00@OverRide:~$ gdb -q level00
Reading symbols from /home/users/level00/level00...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048338  _init
0x08048380  printf
0x08048380  printf@plt
0x08048390  puts
0x08048390  puts@plt
0x080483a0  system
0x080483a0  system@plt
0x080483b0  __gmon_start__
0x080483b0  __gmon_start__@plt
0x080483c0  __libc_start_main
0x080483c0  __libc_start_main@plt
0x080483d0  __isoc99_scanf
0x080483d0  __isoc99_scanf@plt
0x080483e0  _start
0x08048410  __do_global_dtors_aux
0x08048470  frame_dummy
0x08048494  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```
### Funzione `main`
La funzione richiede tramite `scanf` la password trasformandola in un intero
e controlla se il valore è uguale a `0x149c` (`5276`)
```
   0x080484de <+74>:    call   0x80483d0 <__isoc99_scanf@plt>
   0x080484e3 <+79>:    mov    eax,DWORD PTR [esp+0x1c]
   0x080484e7 <+83>:    cmp    eax,0x149c
```
Così da continuare con la chiamata alla funzione `system` la quale ci darà accesso
alla shell.

### Soluzione
Basta scrivere nella password il numero `5276`:
```
level00@OverRide:~$ ./level00
***********************************
*            -Level00 -           *
***********************************
Password:5276

Authenticated!
$ cat /home/users/level01/.pass
uSq2ehEGT6c9S24zbshexZQBXUGrncxn5sD5QfGL
$ exit
level00@OverRide:~$ su level01
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level01/level01
level01@OverRide:~$
```