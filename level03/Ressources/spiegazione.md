# Level 03
Controlliamo le stringhe presenti nel binario:
```
level03@OverRide:~$ readelf -p '.rodata' level03

String dump of section '.rodata':
  [     8]  %u
  [     b]  Congratulations!
  [    1c]  /bin/sh
  [    25]  Invalid Password
  [    38]  J<0x88>^D^Hu<0x87>^D^H<0x85><0x87>^D^H<0x95><0x87>^D^H<0xa5><0x87>^D^H<0xb5><0x87>^D^H<0xc5><0x87>^D^H<0xd5><0x87>^D^H<0xe2><0x87>^D^H<0xef><0x87>^D^HJ
  [    b4]  *^I^Ilevel03^I^I**
  [    c3]  Password:
  [    cd]  %d
```
Analizziamo con `gdb`:
level03@OverRide:~$ gdb -q level03
```
Reading symbols from /home/users/level03/level03...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0804843c  _init
0x08048480  printf
0x08048480  printf@plt
0x08048490  fflush
0x08048490  fflush@plt
0x080484a0  getchar
0x080484a0  getchar@plt
0x080484b0  time
0x080484b0  time@plt
0x080484c0  __stack_chk_fail
0x080484c0  __stack_chk_fail@plt
0x080484d0  puts
0x080484d0  puts@plt
0x080484e0  system
0x080484e0  system@plt
0x080484f0  __gmon_start__
0x080484f0  __gmon_start__@plt
0x08048500  srand
0x08048500  srand@plt
0x08048510  __libc_start_main
0x08048510  __libc_start_main@plt
0x08048520  rand
0x08048520  rand@plt
0x08048530  __isoc99_scanf
0x08048530  __isoc99_scanf@plt
0x08048540  _start
0x08048570  __do_global_dtors_aux
0x080485d0  frame_dummy
0x080485f4  clear_stdin
0x08048617  get_unum
0x0804864f  prog_timeout
0x08048660  decrypt
0x08048747  test
0x0804885a  main
0x080488f0  __libc_csu_init
0x08048960  __libc_csu_fini
0x08048962  __i686.get_pc_thunk.bx
0x08048970  __do_global_ctors_aux
0x0804899c  _fini
```

### Funzione `main`
Riceve come input un numero che verrà poi passato alla funzione `test`.

### Funzione `test`
Sottrae alla costante `322424845` l'input dell'utente
e solo se il numero è minore o uguale a 21 allora verrà inviato alla funzione `decrypt`
altrimenti verrà inviato un numero casuale.

### Funzione `decrypt`
Viene fatto lo xor tra una stringa costante e il numero risultante dalla sottrazione
precedente, solo se la stringa risulterà uguale a `Congratulations!` avremo la shell
altrimenti verrà stampato `Invalid Password`

### Soluzione
Basta trovare la key per lo xor e deve essere un numero compreso tra 0 e 21,
quindi ho creato un piccolo programma (il codice è presente nella cartella Ressources)
dove passo la stringa e il numero massimo che può avere la key e calcola in quel range
tutte le stringhe.
La key è 18, quindi ora basta fare `322424845` (numero costante presente nel programma)
meno `18` e abbiamo l'input da inserire:
```
level03@OverRide:~$ ./level03
***********************************
*               level03         **
***********************************
Password:322424827
$ cat /home/users/level04/.pass
kgv3tkEb9h2mLkRsPkXRfc2mHbjMxQzvb2FrgKkf
$ exit
level03@OverRide:~$ su level04
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level04/level04
level04@OverRide:~$
```