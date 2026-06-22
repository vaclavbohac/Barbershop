# Barbershop — Architecture & How It Works

Barbershop is a C implementation of the classic
[Sleeping Barber problem](http://en.wikipedia.org/wiki/Sleeping_barber_problem),
a textbook inter-process-communication (IPC) and synchronization exercise.

The twist in this project is that it is **networked**: customers are TCP
clients that connect to a server, and the barber is a separate process (or
thread) that coordinates with the connection handlers through System V IPC
(shared memory + semaphores).

This document explains what the project does and how the pieces fit together.

## The problem

A barber sleeps until a customer arrives. The shop has a finite number of
waiting chairs (`MAX_CHAIRS = 5`). When a customer arrives:

- If a chair is free, the customer sits and waits, waking the barber if asleep.
- If all chairs are full, the customer leaves.

The challenge is coordinating the barber and the customers without race
conditions or deadlock.

## Repository layout

The top-level `Makefile` builds four programs, with output placed in `bin/`:

| Program             | Source              | Role                                  |
| ------------------- | ------------------- | ------------------------------------- |
| `server` (processes)| `server-processes/` | Forks a child process per client      |
| `server` (threads)  | `server-threads/`   | Spawns a thread per client            |
| `client`            | `client/`           | A single customer                     |
| `clrsems`/`clrshm`  | `tools/`            | Cleanup utilities for IPC objects     |

Code shared by the servers lives under `shared/`:

- `barber/`     — the barber loop and `cut_hair`
- `semaphores/` — System V semaphore wrappers (`up`/`down`/init)
- `shmemory/`   — System V shared-memory attach helper and `struct shared`
- `protocol/`   — sockets, request/response framing, the `process()` handler
- `messages/`   — the text wire-protocol encoder/parser and message types
- `tools/`      — small helpers (`rtrim`, help text, arg parsing)

### Why two server variants?

The two server flavors implement the **same synchronization solution under two
concurrency models** — one using `fork()` (`server-processes`), one using
POSIX threads (`server-threads`). The barber logic is identical; only the
concurrency primitive differs. This contrast is the core educational value of
the project.

## The two actors

### The barber (`shared/barber/barber.c`)

One long-lived loop, started at server boot — forked as a child process in
`server-processes/main.c` or run as a detached thread in
`server-threads/main.c`:

```
loop forever:
  down(CUSTOMMERS)   # sleep until a customer signals ("sleeping barber")
  down(MUTEX)        # enter critical section
  customers -= 1
  read haircut time from shared memory
  up(BARBER)         # wake the waiting customer
  up(MUTEX)          # leave critical section
  cut_hair(time)     # sleep(time) — simulates the haircut
  up(CUTTED)         # tell the customer the cut is done
  down(SITTING)      # rendezvous: wait for the customer to leave the chair
```

### A customer = one client connection (`shared/protocol/process.c`)

Each accepted connection is handled by `process()`:

```
receive "enter" from client
down(MUTEX)
if customers < MAX_CHAIRS:
    tell client "chair is empty"; handshake "ok"
    customers += 1; up(CUSTOMMERS)        # wake the barber
    tell client "sit"; receive "seconds"
    store requested time in shared memory
else:
    tell client "chairnotfree"; leave
up(MUTEX)
down(BARBER)                              # wait for the barber to take this customer
get_haircut()
up(SITTING) / down(CUTTED)               # rendezvous with the barber
send "done" to client
receive "bye"
```

## Synchronization (`shared/semaphores/`)

Coordination uses **five System V semaphores**, keyed by `getuid()`, with thin
`up()` / `down()` wrappers around `semop()`:

| Semaphore         | Init | Purpose                                                              |
| ----------------- | ---- | ------------------------------------------------------------------- |
| `SEM_MUTEX`       | 1    | Protects the critical section / shared customer count               |
| `SEM_CUSTOMMERS`  | 0    | Counts waiting customers; barber blocks here when empty (the sleep) |
| `SEM_BARBER`      | 0    | Barber signals a customer that it is their turn                     |
| `SEM_CUTTED`      | 0    | Barber signals that the haircut is finished                         |
| `SEM_SITTING`     | 0    | Customer signals it has vacated the chair (barber↔customer rendezvous) |

The `SITTING`/`CUTTED` pair forms a rendezvous that keeps the barber and the
current customer in lockstep before the barber loop repeats.

## Shared state (`shared/shmemory/`)

A `struct shared` lives in a System V shared-memory segment (`shmget` /
`shmat`, keyed by uid):

```c
struct shared {
    int custommers;   // current number of waiting customers
    int semaphores;   // the semaphore set id
    int times[5];     // each waiting customer's requested haircut duration
    int handles[5];   // (declared but currently unused)
};
```

Both the barber and every connection handler attach to the **same** segment,
which is how a request handler can hand a haircut time to the barber even
though they run in different processes.

## Network layer (`shared/protocol/`)

- `server.c` — `server_init` creates, binds, and listens on a TCP socket
  (`SO_REUSEADDR`). The process server defaults to port **4242**; the threads
  server defaults to **8080**.
- `server-processes/server.c` — uses `select()` + `accept()` + `fork()` per client.
- `server-threads/server.c` — uses `accept()` + `pthread_create` /
  `pthread_detach` per client.
- `request.c` / `response.c` — read and write framed messages over the socket.

## Wire protocol (`shared/messages/`)

A small text protocol. The message format (`builder.c`) is:

```
?NN:text\n
^^ ^^   ^
12 34   5
```

1. **Type** — `C`ommand, `A`nswer, `W`arning, `E`rror, `I`nformation.
2. **Code** — optional two-digit **octal** number (e.g. the haircut seconds).
3. **Separator** — `:`.
4. **Body** — the message text.
5. **Terminator** — `\n`.

Messages are encoded with `sprintf(buf, "%c%02o%c%s", ...)` and parsed back by
`message_from_string`. A full client↔server conversation looks like:

```
client → enter
server → chair is empty   (or "chairnotfree" → done)
client → ok
server → sit
client → seconds (with the haircut time as the octal code)
server → done
client → bye
```

The client (`client/`) connects, walks through that handshake, and sends a
configurable haircut length. Recognized flags: `--host=`, `--port=`,
`--time=`, and `--help`.

## Building and running

```bash
make                  # build all four programs into bin/
cd bin
./server              # start the barbershop
./client --time=5     # a customer wanting a 5-second haircut
```

If the IPC objects get into a bad state, clear them with:

```bash
make clrshared        # runs tools/clrsems and tools/clrshm
```

There are also unit tests under `tests/` (requiring `libgtest-dev`), runnable
via `tests/run_tests.sh`:

```bash
sudo apt-get install libgtest-dev
cd tests
make
./run_tests.sh
```
