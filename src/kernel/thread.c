// src/kernel/thread.c

//    src/kernel/thread.c is part of Kylux.
//
//    Kylux is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kylux is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kylux.  If not, see <http://www.gnu.org/licenses/>.

// Author: Kyle Racette
// Date: 2010-02-16 14:16

#include "type.h"
#include "common.h"
#include "debug.h"
#include "kernel/thread.h"
#include "lib/kernel/list.h"
#include "lib/string.h"
#include "kernel/frame_mgr.h"
#include "kernel/spinlock.h"
#include "kernel/lock.h"
#include "kernel/interrupt.h"

struct kernel_thread_frame
{
    void* eip;
    thread_func* func;
    void* aux;
};

struct switch_entry_frame
{
    void (*eip) (void);
};

struct switch_threads_frame
{
    uint32 edi;
    uint32 esi;
    uint32 ebp;
    uint32 ebx;
    /* A return address */
    void (*eip) (void);
    /* Arguments to switch_threads () */
    struct thread* cur;
    struct thread* next;
};

static struct list all_list;
static struct list ready_list;
static struct lock all_list_lock;
static struct lock ready_list_lock;

static struct spinlock tid_lock;
static tid_t next_tid;
static uint32 thread_ticks; /* The number of ticks the current thread
                               has been running */

static struct thread* idle_thread;  /* The idle thread */
static struct thread* initial_thread;  /* The initial thread */

static tid_t alloc_tid ();
static bool thread_init (struct thread*, int priority, const char* name); 

/* Function used to start a kernel thread */
static void start_kernel_thread (thread_func*, void*);

static struct thread* running_thread ();
static struct thread* switch_threads (struct thread*, struct thread*);
static void switch_entry (void);
static void schedule (void);
void schedule_tail (struct thread* prev);
static struct thread* next_thread ();

/* The routine used for the idle thread */
static int idle (void* aux);

uint32 thread_stack_offset;

void
init_thread ()
{
    struct thread* t;

    list_init (&all_list);
    list_init (&ready_list);
    lock_init (&all_list_lock);
    lock_init (&ready_list_lock);
    spinlock_init (&tid_lock);

    next_tid = 1;
    thread_stack_offset = offsetof (struct thread, stack);

    t = running_thread ();
    initial_thread = t;

    thread_init (t, PRI_MED, "main");
    t->tid = alloc_tid ();
    t->status = THREAD_RUNNING;
};

void
start_threading ()
{
    ASSERT (interrupt_get_state () == INTERRUPT_OFF);
    struct semaphore thread_created;
    sema_init (&thread_created, 0);

    framebuf_printf ("Creating idle thread \n");
    thread_create (PRI_MIN, &idle, &thread_created);
    interrupt_on ();

    framebuf_printf ("idle thread created... blocking \n");
    while (1);
    sema_down (&thread_created);
    framebuf_printf ("unblocked.  Idle thread has ran and is in ready queue\n");
};

    tid_t
thread_create (int priority, thread_func* func, void* aux)
{
    struct thread* t;
    struct kernel_thread_frame* kf;
    struct switch_entry_frame* ef;
    struct switch_threads_frame* sf;

    ASSERT (func != NULL);
    ASSERT (priority <= PRI_MAX);
    ASSERT (priority >= PRI_MIN);

    t = frame_mgr_alloc (PAGE_SIZE, false, true);
    if (t == NULL)
    {
        return TID_ERROR;    
    }
    t->tid = alloc_tid ();
    thread_init (t, priority, "test");

    kf = (struct kernel_thread_frame*) (t->stack - sizeof (*kf));
    kf->eip = NULL;
    kf->func = func;
    kf->aux = aux;

    ef = (struct switch_entry_frame*) (t->stack - sizeof (*ef));
    ef->eip = (void (*) (void)) start_kernel_thread;

    sf = (struct switch_threads_frame*) (t->stack - sizeof (*sf));
    sf->eip = switch_entry;
    sf->ebp = 0;

    lock_acquire (&all_list_lock);
    list_push_back (&all_list, &t->all_elem);
    lock_release (&all_list_lock);

    thread_unblock (t);
    return t->tid;
};

    void
thread_exit (int exit_code)
{

};

    struct thread*
thread_current ()
{
    struct thread* t = running_thread ();

    ASSERT (t->magic == THREAD_MAGIC);
    ASSERT (t->status == THREAD_RUNNING);

    return t;
};

    void
thread_block ()
{
    ASSERT (interrupt_get_state () == INTERRUPT_OFF);

    thread_current ()->status = THREAD_BLOCKED;
    schedule ();
};

    void
thread_unblock (struct thread* t)
{
    ASSERT (t != NULL);

//    lock_acquire (&ready_list_lock);
    enum interrupt_state state = interrupt_off ();
    t->status = THREAD_READY;
    list_push_back (&ready_list, &t->elem);
//    lock_release (&ready_list_lock);
    interrupt_restore (state);
};

    static struct thread*
running_thread ()
{
    uint8* esp;
    struct thread* t;

    asm volatile ("mov %%esp, %0" : "=g" (esp));
    t = (struct thread*) ROUND_DOWN((uint32) esp, PAGE_SIZE);
    framebuf_printf ("running_thread: %p \n", t);

    return t;
};

    static tid_t
alloc_tid (void)
{
    tid_t tid;

    spinlock_acquire (&tid_lock);
    tid = next_tid;
    next_tid++;
    spinlock_release (&tid_lock);

    return tid;
};

    static bool 
thread_init (struct thread* t, int priority, const char* name)
{
    ASSERT (t != NULL);
    ASSERT (priority >= PRI_MIN && priority <= PRI_MAX);
    ASSERT (name != NULL);

    memset (t, 0, sizeof *t);

    t->status = THREAD_BLOCKED;
    t->priority = priority;
    t->stack = (uint8*) t + PAGE_SIZE;
    t->magic = THREAD_MAGIC;

    list_push_back (&all_list, &t->all_elem);

    return true;
}; 

    void 
thread_yield ()
{
    struct thread* cur = thread_current ();
    enum interrupt_state state = interrupt_off ();

    //    lock_acquire (&ready_list_lock);
    if (cur != idle_thread)
        list_push_back (&ready_list, &cur->elem);
    cur->status = THREAD_READY;
    //    lock_release (&ready_list_lock);
    schedule ();

    interrupt_restore (state);
};

    void
thread_tick ()
{
    thread_ticks++;

    if (thread_ticks >= TIME_SLICE)
        thread_yield ();
};

    static void
start_kernel_thread (thread_func* func, void* aux)
{
    ASSERT (func);

    /* Execute the function and then clean up the thread */
    func (aux);
    thread_exit (0);
};

    void 
schedule_tail (struct thread* prev)
{
    struct thread* cur = running_thread ();

    ASSERT (prev != NULL);

    cur->status = THREAD_RUNNING;

    thread_ticks = 0;

    if (prev != NULL && prev->status == THREAD_DYING && prev != initial_thread)
    {
        ASSERT (prev != cur);
        frame_mgr_free (prev);
    }
};

    static void 
schedule (void)
{
    struct thread* cur = running_thread ();
    struct thread* next = next_thread ();
    struct thread* prev = NULL;

    ASSERT (cur->status != THREAD_RUNNING);
    ASSERT (interrupt_get_state () == INTERRUPT_OFF);
    ASSERT (next->magic == THREAD_MAGIC);

    if (cur != next)
        prev = switch_threads (cur, next);
    schedule_tail (prev);
};

/* Schedules the next thread to run */
static struct thread* 
next_thread ()
{
    if (!list_empty (&ready_list))
        return LIST_ENTRY (list_pop_front (&ready_list), 
            struct thread, elem);
    else
        return running_thread ();
};


static int 
idle (void* aux)
{
    struct semaphore* sema = aux;

    ASSERT (aux != NULL);

    framebuf_printf ("IDLE: upping semaphore \n");
    sema_up (sema);
    framebuf_printf ("IDLE: semaphore upped \n");

    while (1)
    {
        framebuf_printf ("idle thread running \n");
        thread_yield ();
    };

    NOT_REACHED;
    return 0;
};
