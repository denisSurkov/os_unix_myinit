# Simple init program

Задача 3 из [задачника](https://eykuklin.github.io/OS-UNIX/).

Простая реализация программы `init` в OS Unix.

```txt
usage: ./os_unix_myinit /path/to/config/
```

## Как запустить "тесты":
0. `cmake .` - создает `Makefile`. В репозитории уже лежит необходимый `Makefile`, так требует формат выполнения задач.
1. `chmod +x runme.sh`
2. `./runme.sh`


## Мой вывод:
```txt
[~]     Building app

[100%] Built target os_unix_myinit

[~]     Deleting /tmp/myinit.log if exists

[~]     Creating aliases for /usr/bin/sleep

[~]     Creating in & out files

[~]     All tmp files created

[~]     Creating config /tmp/myinit-config

[~]     Contents of /tmp/myinit-config

/tmp/custom-a 1000 /tmp/custom-in /tmp/custom-out
/tmp/custom-b 1500 /tmp/custom-in /tmp/custom-out
/tmp/custom-c 1000 /tmp/custom-in /tmp/custom-out

[~]     Stating daemon

[~]     Sleeping 1 sec to allow daemon to run processes

[~]     Checking started 3 processes

[+]     Checked 3 processes, all working!

[~]     Going to kill process 2

[~]     Killed process 2, going to sleep 1 sec

[~]     Wake up, checking 3 processes

[~]     Checking started 3 processes

[+]     Checked 3 processes, all working!

[~]     Changing config file

[~]     Contents of /tmp/myinit-config

/tmp/custom-a 1000 /tmp/custom-in /tmp/custom-out

[~]     Sending SIGHUP signal

[~]     Checking started /tmp/custom-a

[+]     Checked /tmp/custom-a processes, it's working!

[+]     Logs content

Sun May 14 20:23:30 2023        |       started daemon
Sun May 14 20:23:30 2023        |       got 3 processes in /tmp/myinit-config file
Sun May 14 20:23:30 2023        |       starting /tmp/custom-a
Sun May 14 20:23:30 2023        |       starting /tmp/custom-c
Sun May 14 20:23:30 2023        |       starting /tmp/custom-b
Sun May 14 20:23:32 2023        |       got sighup signal, reloading config
Sun May 14 20:23:32 2023        |       stopped previous daemon state
Sun May 14 20:23:32 2023        |       got 1 processes in /tmp/myinit-config file
Sun May 14 20:23:32 2023        |       reloaded config and start new processes
Sun May 14 20:23:32 2023        |       starting /tmp/custom-a

[~]     Clean up

```

Сурков Денис Дмитриевич, МЕН-300801 (ФТ-301), 2023.
