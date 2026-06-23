// lottery_scheduler.c
// OSTEP Ch.9 — Proportional Share: Lottery Scheduling demo
//
// Build:  gcc -O2 -o lottery_scheduler lottery_scheduler.c
// Run:    ./lottery_scheduler [num_slices] [seed]
//
// What it shows:
//   - Each job holds some number of tickets (its "share").
//   - Every time slice we draw a random winning ticket in [0, total_tickets).
//   - We walk the job list, accumulating tickets, until the running sum
//     exceeds the winner — that job owns the winning ticket and runs.
//   - After many slices, each job's run count converges to
//     (its tickets / total tickets).

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node {
    char         name[8];
    int          tickets;   // this job's share
    long         runs;      // how many slices this job has won
    struct node *next;
} node_t;

// Insert a job at the head of the list. Returns the new head.
static node_t *insert(node_t *head, const char *name, int tickets) {
    node_t *n = malloc(sizeof(node_t));
    if (!n) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    snprintf(n->name, sizeof(n->name), "%s", name);
    n->tickets = tickets;
    n->runs    = 0;
    n->next    = head;
    return n;
}

// Run one lottery: pick a winner among the jobs in [head] and return it.
// total_tickets is the sum of all jobs' tickets.
static node_t *lottery(node_t *head, int total_tickets) {
    // winner: random ticket in [0, total_tickets)
    int winner = rand() % total_tickets;

    // walk the list, summing tickets until we pass 'winner'
    int counter = 0;
    node_t *current = head;
    while (current) {
        counter += current->tickets;
        if (counter > winner)
            break;              // found the owner of 'winner'
        current = current->next;
    }
    return current;             // guaranteed non-NULL: winner < total_tickets
}

int main(int argc, char *argv[]) {
    long num_slices = (argc > 1) ? atol(argv[1]) : 1000000;
    unsigned seed   = (argc > 2) ? (unsigned)atoi(argv[2])
                                 : (unsigned)time(NULL);
    srand(seed);

    // Build the job list. (Tip: keeping it sorted most-tickets-first makes
    // the lottery() loop exit sooner on average — order does not affect
    // correctness, only speed.)
    node_t *head = NULL;
    head = insert(head, "C", 250);   // 250 / 400 = 62.5%
    head = insert(head, "A", 100);   // 100 / 400 = 25.0%
    head = insert(head, "B",  50);   //  50 / 400 = 12.5%

    // Compute total tickets once (no inflation/transfer in this simple demo).
    int total_tickets = 0;
    for (node_t *p = head; p; p = p->next)
        total_tickets += p->tickets;

    // Run the scheduler for num_slices time slices.
    for (long i = 0; i < num_slices; i++) {
        node_t *winner = lottery(head, total_tickets);
        winner->runs++;
    }

    // Report: expected vs. actual share. Actual converges to expected as
    // num_slices grows (law of large numbers).
    printf("seed=%u  slices=%ld  total_tickets=%d\n\n",
           seed, num_slices, total_tickets);
    printf("%-5s %8s %12s %12s\n", "job", "tickets", "expected%", "actual%");
    printf("------------------------------------------\n");
    for (node_t *p = head; p; p = p->next) {
        double expected = 100.0 * p->tickets / total_tickets;
        double actual   = 100.0 * (double)p->runs / (double)num_slices;
        printf("%-5s %8d %11.2f%% %11.2f%%\n",
               p->name, p->tickets, expected, actual);
    }

    // Free the list.
    node_t *p = head;
    while (p) {
        node_t *next = p->next;
        free(p);
        p = next;
    }
    return 0;
}
