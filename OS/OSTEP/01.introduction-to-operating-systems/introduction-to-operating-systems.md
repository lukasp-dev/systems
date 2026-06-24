# Introduction to Operating Systems

> Combined notes for OSTEP Part 1 intro: abstraction vs arbitration, OS elements, virtualization (CPU & memory), demos, and kernel architectures.

## Table of contents

1. [Abstraction or Arbitration](#abstraction-or-arbitration)
2. [OS Elements](#os-elements)
3. [Virtualization](#virtualization)
4. [OSTEP Figure 2.1 — CPU process demo](#ostep-figure-21--cpu-process-demo)
5. [Virtualizing memory](#virtualizing-memory)
6. [OSTEP Figure 2.5 — race on shared memory](#ostep-figure-25--race-on-shared-memory)
7. [Kernel Architectures: Monolithic vs Modular vs Microkernel](#kernel-architectures-monolithic-vs-modular-vs-microkernel)

---

## Abstraction or Arbitration

> **TL;DR** Two core OS jobs — **abstraction** hides hardware complexity behind uniform interfaces; **arbitration** shares limited resources among competing processes.

> **How to tell them apart:** ask *"is the OS hiding messy details (→ abstraction)"* or *"is it dividing one limited resource among many (→ arbitration)?"*

#### Abstraction (Hiding Complexity)
Hides low-level hardware complexity behind high-level, simplified interfaces (APIs), providing high-level, uniform interfaces to apps.

#### Arbitration (Managing Resources)
Controls and allocates shared resources among multiple users or processes to prevent conflicts.

#### Examples

- distributing memory between multiple processes -> Arbitration
- supporting different types of speakers -> Abstraction
- Interchangeable access of hard disk or SSD -> Abstraction

---

## OS Elements

> **TL;DR** The OS exposes **abstractions** (what you see), realized by **mechanisms** (how) and governed by **policies** (which/when). Key principles: separate mechanism from policy, optimize the common case, and remember that crossing the user/kernel boundary has a cost.

#### Abstractions
- process, thread, file, socket, memory page

#### Mechanisms
- create, schedule, open, write, allocate

#### policies
- least-recently used (LRU), earliest deadline first (EDF), etc.

### OS Design Principles
**1. Separation of Mechanism & Policy**
This principle is about modularity. It suggests that the "how" (mechanism) should be independent from "why/when" (policy)

- Mechanism: The low level code that provides a capability. For example, a context switch routine that saves registers and swaps page tables is a mechanism.
- Policy: The high-level logic that decides which process should run next.

By separating them, you can change your scheduling algorithm (e.g., switching from First-Come-First-Served to a Multi-Level Feedback Queue) without ever touching the complex hardware-dependent context-switching code.

**2. Optimize for the Common Case**
This is a core tenet of computer architecture (often attributed to Gene Amdahl). It suggests that since resources (time, developer effort, silicon) are finite, you should focus performance gains where they will be felt the most.
To apply this, you must analyze your workload requirements:

- **Instruction level:** If 90% of your instructions are simple additions and only 1% are complex divisions, you make the adder as fast as possible, even if it makes the divider slightly slower.

- **OS level:** If most processes are short-lived interactive tasks, the scheduler should be tuned to minimize response time for those, rather than optimizing for long-running batch jobs.

- **Memory:** Caching is the ultimate "common case" optimization. It assumes that programs will likely access the same data repeatedly (temporal locality), so we optimize for the case where data is in the L1 cache.

### Crossing the OS Boundary
**user/Kernel Transitions**
They are not chip!!
- hardware supported
    - e.g., traps on illegal instructions or memory accesses requiring special privilege.
- involves a number of instructions
    - e.g., ~50~100ns on a 2GHz machine running Linux
- switches locality
    - affects hardware cache!!

---

## Virtualization

In OSTEP, **Virtualization** is Part 1: the OS makes **one physical machine** look like **many simpler, private machines** to programs.

> **TL;DR** The OS gives each process the illusion of its own CPU, its own memory, and clean device interfaces — by time-sharing and mapping shared hardware underneath.

#### Cheat sheet

| Resource | Illusion each process gets | How |
| --- | --- | --- |
| **CPU** | its own CPU, always running | time-sharing / scheduling |
| **Memory** | its own large private address space | address spaces / paging |
| **Devices** | clean files, sockets, drivers — not raw hardware | abstraction layers |

#### What the OS virtualizes

- **CPU** — many processes appear to run at once; the kernel **time-shares** one (or a few) physical cores via scheduling.
- **Memory** — each process gets its own private **address space**; see [Virtualizing memory](#virtualizing-memory).
- **Devices** — disks, network, and devices are exposed through **files, sockets, and drivers** so apps do not touch raw hardware.

#### CPU virtualization

Turning a single CPU (or a small set of them) into a seemingly infinite number of CPUs, and thus allowing many programs to seemingly run at once, is what we call **CPU virtualization**.

#### Memory virtualization

Each process gets a **private virtual address space** mapped onto physical RAM, so it can use its memory as if it owned the machine. See [Virtualizing memory](#virtualizing-memory).

#### Why it matters

Without virtualization, every program would need to know hardware details, coordinate with every other program, and one bug could corrupt the whole machine. The OS **abstracts** hardware and **arbitrates** who gets what.

#### Topics under this section (OSTEP Part 1)

- processes, limited direct execution, context switch
- scheduling policies (FCFS, MLFQ, lottery, etc.)
- address spaces, paging, TLBs, swapping

#### Relation to other intro notes

Virtualization is the **first major job** the OS does for applications: run many programs **as if** each had its own CPU and memory. See [Abstraction or Arbitration](#abstraction-or-arbitration) · [OS Elements](#os-elements) · [Kernel Architectures](#kernel-architectures-monolithic-vs-modular-vs-microkernel).

**Code examples:** [Figure 2.1 CPU process demo](#ostep-figure-21--cpu-process-demo) · [figure2.1.c](./code/figure2.1.c) · [Virtualizing memory](#virtualizing-memory) · [figure2.3.c](./code/figure2.3.c) · [Figure 2.5 race demo](#ostep-figure-25--race-on-shared-memory) · [figure2.5.c](./code/figure2.5.c)

---

## OSTEP Figure 2.1 — CPU process demo

> **TL;DR** A CPU-bound demo that spins then prints; run several copies and the scheduler **interleaves** them — CPU virtualization in action.

**Source:** [figure2.1.c](./code/figure2.1.c) · [common.h](./code/common.h)

#### What it does

Infinite loop: **spin for ~1 second**, then `printf` one line. Models a CPU-bound process that periodically prints a label.

```c
while (1) {
    Spin(1);
    printf("%s\n", str);
}
```

`Spin(1)` (in [common.h](./code/common.h)) busy-waits until `GetTime()` says 1 second passed — CPU keeps running in a tight loop instead of calling `sleep()`.

#### `if (argc != 2)`

Program expects **exactly one argument** after the program name:

| invocation | `argc` | result |
| :--- | :---: | :--- |
| `./figure2.1` | 1 | usage error |
| `./figure2.1 hello` | 2 | OK — prints `hello` every second |
| `./figure2.1 a b` | 3 | usage error |

`argv[1]` is the string to print.

#### Compile and run

```bash
cd OS/OSTEP/01.introduction-to-operating-systems/code
gcc -Wall -Wextra -std=c11 figure2.1.c -o figure2.1
./figure2.1 "hello"
```

Stop foreground run: `Ctrl+C`.

#### Multiple processes (time sharing)

```bash
./figure2.1 A & ./figure2.1 B & ./figure2.1 C &
```

Three **separate processes** share one CPU (on a single core). Output **interleaves** (`A`, `B`, `C` in varying order) because the scheduler switches between them — classic intro to **virtualizing the CPU**.

#### Stop background instances

Executable name is **`figure2.1`** (not `figuare2.1`):

```bash
killall figure2.1
# or
jobs
kill %1 %2 %3
```

Check nothing left: `ps aux | grep figure2.1`

#### Takeaway

**CPU virtualization:** turning a single CPU (or a small set of them) into a seemingly infinite number of CPUs, and thus allowing many programs to seemingly run at once, is what we call it.

One machine, one core, three programs — each thinks it runs continuously, but the OS **multiplexes** the CPU so they take turns. That is this idea in practice. See [Virtualization](#virtualization).

---

## Virtualizing memory

> **TL;DR** Each process gets a private **virtual address space** mapped to physical RAM, so the *same* virtual address in two processes points to *different* memory — giving isolation.

Each process accesses its own **private virtual address space**, which the OS somehow maps onto **physical memory**.

#### Virtual vs physical

- **Virtual address** — what the program sees (`p`, `%p`, pointers in your code).
- **Physical address** — where bytes actually live in RAM; programs normally **do not** touch this directly.

The OS (with the MMU) maintains **per-process page tables**: virtual page → physical frame. Same virtual address number in two processes can map to **different** physical pages.

#### Why it matters

- One buggy process cannot scribble over another’s memory by accident (isolation).
- Each process can use `malloc`, stack, code layout as if it owns the machine.
- The OS can move or swap pages without the program knowing (later: paging, swapping).

#### Same address, different memory (Figure 2.3)

Run two copies:

```bash
cd OS/OSTEP/01.introduction-to-operating-systems/code
./figure2.3 & ./figure2.3
```

Each prints its own `getpid()` and a heap address from `malloc`. They increment `*p` independently — **no shared counter**, because each `p` lives in **that process’s** address space only.

**Code:** [figure2.3.c](./code/figure2.3.c) · [common.h](./code/common.h)

---

## OSTEP Figure 2.5 — race on shared memory

> **TL;DR** Two threads share one `counter`; `counter++` is **not atomic** (load–add–store), so concurrent increments interleave and **lose updates**. Needs locks/atomics.

**Source:** [figure2.5.c](./code/figure2.5.c) · [common_threads.h](./code/common_threads.h)

#### What it does

Two **threads** (same process) each run `worker`, which does `loops` times:

```c
counter++;
```

Expected final value: **`2 × loops`** (each thread increments `loops` times).

#### Compile and run

```bash
cd OS/OSTEP/01.introduction-to-operating-systems/code
gcc -Wall -Wextra -std=c11 -pthread figure2.5.c -o figure2.5
./figure2.5 1000000
```

Example output:

```text
Initial value: 0
Final value: 1047497
```

You asked for **2,000,000** but got **1,047,497** — and the wrong number **changes every run**. That weird behavior is the point.

#### Why: `counter++` is not atomic

`counter++` looks like one step, but the CPU/compilers treat it as **three**:

1. **Load** `counter` from memory into a register  
2. **Add** 1 in the register  
3. **Store** the result back to `counter`

Two threads can **interleave** those steps:

```text
Thread A: load counter (= 5)
Thread B: load counter (= 5)   ← both read the same value
Thread A: add → 6, store 6
Thread B: add → 6, store 6     ← one increment lost; should be 7
```

So updates are **lost**. More loops → more chances to collide → final value often **far below** `2 × loops`.

`volatile` on `counter` only forces reads/writes to go to memory; it does **not** make the load–add–store sequence **indivisible**. You still need locks, atomics (`std::atomic` / C11 `_Atomic`), or other synchronization.

#### Takeaway

Same address space, shared `counter` — unlike [Virtualizing memory](#virtualizing-memory) (separate **processes**), threads **do** share memory, so unsynchronized updates race. Correctness requires **atomic** operations or explicit locking.

---

## Kernel Architectures: Monolithic vs Modular vs Microkernel

> **TL;DR** Three ways to structure a kernel, trading **performance** against **isolation/maintainability**: monolithic = everything in the kernel (fast, fragile); microkernel = almost nothing in the kernel (robust, slower); modular = a middle ground (lean core + loadable modules).

#### At a glance

| Dimension | Monolithic | Modular | Microkernel |
| --- | --- | --- | --- |
| Where services run | all in kernel space (privilege bit = 0) | core kernel + loadable modules | tiny kernel; most services in **user space** |
| Kernel size | large | lean core, load on demand | minimal (only IPC, memory, scheduling) |
| Performance | fastest (direct calls) | slight indirection cost | slowest (IPC / boundary crossings) |
| Fault isolation | poor — one driver crash kills the OS | better — modules are separable | excellent — a driver crash ≠ kernel crash |
| Maintainability | hard (tangled "spaghetti") | update one module independently | clean, small, auditable |
| Memory footprint | large (everything loaded) | small (load only what you need) | small core |
| Examples | classic UNIX | Linux (loadable kernel modules) | MINIX, seL4, QNX |

### Monolithic OS
A Monolithic OS is an operating system where every service runs inside one large program called the **Kernel**.

Think of it like SwissArmy Knife that has everything you need (tools for files, memory, and the CPU) is built into one single handle. In technical terms, all these services run in **Kernel Mode** (Privilege Bit = 0)

**The Good(+)**
- High Performance: Because everything is in one place, the "tools" can talk to each other instantly. There is no need to jump back and forth between different memory areas.

- Compile-Time Optimization: Since the entire kernel is one big piece of code, the compiler can optimize it all at once (e.g., inlining functions to save time).

- Simple Communication: Modules don't need complex messaging; they just call each other directly.

**The Bad (-)**
- Hard to Manage: The code becomes massive and "tangled." If you want to change one small part, you might accidentally break something else (the "Spaghetti Code" problem).

- System Fragility: If one driver (like a printer driver) crashes, the entire operating system crashes because everything shares the same space.

- Large Memory Footprint: Even if you don't use certain features, they are still loaded into the kernel memory.

### Modular OS
The kernel has a core "base" and can plug in extra pieces (modules) only when they are needed. Think of it like a desktop computer. You have the main tower (core kernel), and you can plug in a USB webcam or printer (modules) whenever you want.

**The Pros (+)**
- Maintainability: You can fix or update one specific module (like a network driver) without rewriting the entire operating system.

- Smaller Footprint: You only load what you need. If you don't have a printer, the printer driver doesn't take up any RAM.

- Less Resource Needs: Because the kernel stays lean, it uses less memory and CPU power for background tasks.

**The Cons (-)**
- Indirection Impacts Performance: To talk to a module, the OS has to go through an interface (the box labeled "Module Interface" in your image). This extra step is called "indirection," and it can make things slightly slower than a pure Monolithic system where everything is already connected.

- Maintenance Issues: Even though it’s modular, you still have to make sure the modules are compatible with the core version of the OS. If the core changes, the module might break.

### Microkernel
A Microkernel is a special way to build an Operating System. Its main goal is to keep the "heart" of the system as small as possible by pushing almost everything into the software layer.

In a regular OS (like Linux), the kernel is huge because it contains everything: the file system, the drivers, and the network. If one small part breaks, the whole system crashes.

A Microkernel does the opposite. It is a "minimalist" design. It takes all those extra parts and pushes them out of the kernel and into the User Space (the software layer).

Imagine a manager who refuses to do any manual labor. He only handles the most important rules:

**The Kernel (The Manager):** Only handles 3 things: Sending messages (IPC), giving out memory addresses, and managing CPU timing (Threads).

**The Software (The Workers):** Everything else—like your Wi-Fi driver, your Disk driver, and your File System—is pushed out. They run as separate, independent software programs.

**The Big Benefits (+)**
- It is "Crash-Proof": If your Disk Driver has a bug and crashes, it only crashes that one software "bubble." The Kernel (the heart) stays alive, and you can just restart that one driver.

- It is Secure: Because the Kernel is so tiny, it is very easy for engineers to check every single line of code for mistakes or hackers.

**The Big Problem (-)**
- The "Crossing" Cost: Because everything is pushed out into separate software pieces, they have to talk to each other constantly.

The Speed Penalty: To send a message from the "File System software" to the "Disk Driver software," the message must go:

1. Down into the Kernel.
2. Up into the other software.

This "back and forth" (called context switching) takes time. This makes Microkernels slower than the big, "all-in-one" kernels.
