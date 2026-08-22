# MiniOS — OSTEP-driven C++ learning project

Build one small Unix-like environment in user space. Each OSTEP chapter adds a
subsystem and connects to the ones before it.

## Phase 1 (current): Process / PCB

Maps to OSTEP Process API notes (§4–§14) and xv6 `struct proc`:

| OSTEP concept | MiniOS code |
| --- | --- |
| PCB / process descriptor | `minios::Process` |
| Process table / `proc[]` | `minios::ProcessTable` |
| `NPROC` / `UNUSED` slot | `kMaxProcesses`, empty `std::optional` slot |
| Ready / Running / Blocked / Zombie | `ProcessState` |
| `fork()` | `ProcessTable::fork()` |
| `wait()` / reap | `ProcessTable::wait()` |
| `exit()` / zombie | `ProcessTable::exitProcess()` |

## Build & run

```bash
cd OS/MiniOS
make
./minios
```

## Study loop (repeat every chapter)

1. Read OSTEP + your notes under `OS/OSTEP/`
2. Run host POSIX demos (`OS/OSTEP/.../code/figure5.x.c`)
3. Implement the matching MiniOS subsystem
4. Wire it to existing code + extend `main` or shell

## Next (Phase 2)

- `Scheduler` interface + `RoundRobinScheduler`
- `Kernel::tick()` moves Running → Ready → next process
- Shell commands: `run`, `ps`, `sched`
