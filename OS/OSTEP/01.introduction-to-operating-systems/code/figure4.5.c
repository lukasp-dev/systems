// Define maximum number of open files per process
#define NOFILE 16
typedef unsigned int uint;

// The registers xv6 will save and restore
// to stop and subsequently restart a process
struct context {
    int eip;
    int esp;
    int ebx;
    int ecx;
    int edx;
    int esi;
    int edi;
    int ebp;
};

// the different states a process can be in
enum proc_state {
    UNUSED, EMBRYO, SLEEPING, RUNNABLE, RUNNING, ZOMBIE
};

// the information xv6 tracks about each process
// including its refister context and state
struct proc {
    char *mem;              // Start of process memory
    uint sz;                // Size of process memory
    char *kstack;           // Bottom of kernel stack for this process
    enum proc_state state;  // Process state
    int pid;                // Process id
    struct proc *parent;    // Parent process
    void *chan;             // If !zero, sleeping on chan
    int killed;             // If !zero has been killed
    struct file *ofile[NOFILE]; // open files
    struct inode *cwd;      // Current directory
    struct context context; // Switch here to run process
    struct trapframe *tf;   // Current interrupt
};

