# Level 06
Analizziamo il binario tramite `gdb`:
```
level06@OverRide:~$ gdb -q level06
Reading symbols from /home/users/level06/level06...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x080484d0  _init
0x08048510  printf
0x08048510  printf@plt
0x08048520  strcspn
0x08048520  strcspn@plt
0x08048530  fflush
0x08048530  fflush@plt
0x08048540  getchar
0x08048540  getchar@plt
0x08048550  fgets
0x08048550  fgets@plt
0x08048560  signal
0x08048560  signal@plt
0x08048570  alarm
0x08048570  alarm@plt
0x08048580  __stack_chk_fail
0x08048580  __stack_chk_fail@plt
0x08048590  puts
0x08048590  puts@plt
0x080485a0  system
0x080485a0  system@plt
0x080485b0  __gmon_start__
0x080485b0  __gmon_start__@plt
0x080485c0  __libc_start_main
0x080485c0  __libc_start_main@plt
0x080485d0  strnlen
0x080485d0  strnlen@plt
0x080485e0  __isoc99_scanf
0x080485e0  __isoc99_scanf@plt
0x080485f0  ptrace
0x080485f0  ptrace@plt
0x08048600  _start
0x08048630  __do_global_dtors_aux
0x08048690  frame_dummy
0x080486b4  clear_stdin
0x080486d7  get_unum
0x0804870f  prog_timeout
0x08048720  enable_timeout_cons
0x08048748  auth
0x08048879  main
0x08048990  __libc_csu_init
0x08048a00  __libc_csu_fini
0x08048a02  __i686.get_pc_thunk.bx
0x08048a10  __do_global_ctors_aux
0x08048a3c  _fini
```
### Funzione `main`
In questa funzione vengono richiesti due input, il primo sarà una stringa
mentre il secondo un numero intero senza segno, ed entrambi verranno passati
alla funzione `auth` per poi valutare il suo ritorno e solo nel caso in cui
ritorna `0` ci viene fornita una shell.

### Funzione `auth`
Applica un hash al primo input inserito dall'utente e poi verifica se il
ritorno dell'hash è diverso dal secondo input inserito, nel caso in cui fossero
diversi ritorna `0`, altrimenti `1`.

### Soluzione
Basta applicare due breakpoint, il primo per bypassare la funzione `ptrace` usata
per controllare se il programma è sotto debug, e il secondo per leggere l'hash generato
in base al primo input:
```
level06@OverRide:~$ gdb -q level06
Reading symbols from /home/users/level06/level06...(no debugging symbols found)...done.
(gdb) b *auth+114
Breakpoint 1 at 0x80487ba
(gdb) b *auth+283
Breakpoint 2 at 0x8048863
(gdb) r
Starting program: /home/users/level06/level06
***********************************
*               level06           *
***********************************
-> Enter Login: usavoia
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 42

Breakpoint 1, 0x080487ba in auth ()
(gdb) set $eax=1
(gdb) c
Continuing.

Breakpoint 2, 0x08048863 in auth ()
(gdb) x /d $ebp-0x10
0xffffd698:     6233804
```
```
level06@OverRide:~$ ./level06
***********************************
*               level06           *
***********************************
-> Enter Login: usavoia
***********************************
***** NEW ACCOUNT DETECTED ********
***********************************
-> Enter Serial: 6233804
Authenticated!
$ cat /home/users/level07/.pass
GbcPDRgsFK77LNnnuh7QyFYA2942Gp8yKj9KrWD8
$ exit
```
```
level06@OverRide:~$ su level07
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level07/level07
level07@OverRide:~$
```