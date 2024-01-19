#include "../../sys/types.h"


/* from stdint.h */
#define	UINT64_MAX		0xffffffffffffffffULL

/* witness.h */
#define	LOP_NEWORDER	0x00000001	/* Define a new lock order. */
#define	LOP_EXCLUSIVE	0x00000008	/* Exclusive lock. */
#define	WITNESS_CHECKORDER(lock, flags, interlock)			\
witness_checkorder((lock), (flags), (interlock))

/* from mutex.h */
#define MUTEX_LOCK_OBJECT(mtx)	(&(mtx)->mtx_lock_obj)
struct mutex {
	volatile void *mtx_owner;
	int mtx_wantipl;
	int mtx_oldipl;
#ifdef WITNESS
	struct lock_object mtx_lock_obj;
#endif
};
#ifdef WITNESS
#define MUTEX_INITIALIZER_FLAGS(ipl, name, flags) \
{ NULL, __MUTEX_IPL((ipl)), IPL_NONE, MTX_LO_INITIALIZER(name, flags) }
#else
#define MUTEX_INITIALIZER_FLAGS(ipl, name, flags) \
{ NULL, __MUTEX_IPL((ipl)), IPL_NONE }
#endif

#ifdef MULTIPROCESSOR
#define __MUTEX_IPL(ipl) \
(((ipl) > IPL_NONE && (ipl) < IPL_MPFLOOR) ? IPL_MPFLOOR : (ipl))
#else
#define __MUTEX_IPL(ipl) (ipl)
#endif

#define MUTEX_INITIALIZER(ipl) \
MUTEX_INITIALIZER_FLAGS(ipl, __MTX_NAME, 0)

/* Diagnostic macros from malloc.h */
#ifndef DIAGNOSTIC
#define KASSERTMSG(e, msg, ...) ((void)0)
#define KASSERT(e) ((void)0)
#else
#define KASSERTMSG(e, msg, ...) ((e) ? (void)0 : \
panic(__KASSERTSTR " " msg, "diagnostic ", #e, \
__FILE__, __LINE__, ## __VA_ARGS__))
#define KASSERT(e) ((e) ? (void)0 : \
__assert("diagnostic ", __FILE__, __LINE__, #e))
#endif

#define PAGE_SIZE 4096 /* 1 mem page size */
#define M_CANFAIL 0x0004 /* Flag from malloc.h */

/* Time structures from time.h */
struct bintime {
	time_t sec;
	uint64_t frac;
};

#ifndef _TIMESPEC_DECLARED
#define _TIMESPEC_DECLARED
struct timespec {
	time_t tv_sec; /* seconds */
	long tv_nsec;  /* nanoseconds */
};
#endif

#ifndef _TIMEVAL_DECLARED
#define _TIMEVAL_DECLARED
struct timeval {
	time_t tv_sec;     /* seconds */
	suseconds_t tv_usec; /* microseconds */
};
#endif

#define	timersub(tvp, uvp, vvp)						\
do {								\
(vvp)->tv_sec = (tvp)->tv_sec - (uvp)->tv_sec;		\
(vvp)->tv_usec = (tvp)->tv_usec - (uvp)->tv_usec;	\
if ((vvp)->tv_usec < 0) {				\
(vvp)->tv_sec--;				\
(vvp)->tv_usec += 1000000;			\
}							\
} while (0)

static inline void
BINTIME_TO_TIMEVAL(const struct bintime *bt, struct timeval *tv)
{
	tv->tv_sec = bt->sec;
	tv->tv_usec = (long)(((uint64_t)1000000 * (uint32_t)(bt->frac >> 32)) >> 32);
}

/* Memory statistics structure from malloc.h */
struct kmemstats {
	long ks_inuse;      /* currently in use */
	long ks_calls;      /* total allocated */
	long ks_memuse;     /* total memory held in bytes */
	u_short ks_limblocks; /* limit blocks */
	long ks_maxused;    /* maximum used */
	long ks_limit;      /* limit */
	long ks_size;       /* size of allocations */
	long ks_spare;
};

/* Singly-linked list entry from queue.h */
#define SIMPLEQ_HEAD(name, type)					\
struct name {								\
struct type *sqh_first;	/* first element */			\
struct type **sqh_last;	/* addr of last next element */		\
}
#define SLIST_ENTRY(type) \
struct { \
struct type *sle_next; /* next element */ \
}
#define TAILQ_HEAD(name, type)						\
struct name {								\
struct type *tqh_first;	/* first element */			\
struct type **tqh_last;	/* addr of last next element */		\
}

/* Atomic operations from atomic.h */
#ifndef atomic_add_int_nv
static inline unsigned int atomic_add_int_nv(volatile unsigned int *p, unsigned int v) {
	return __sync_add_and_fetch(p, v);
}
#endif

#ifndef atomic_inc_int_nv
#define atomic_inc_int_nv(_p) atomic_add_int_nv((_p), 1)
#endif

#ifndef membar_consumer
#define membar_consumer() __sync_synchronize()
#endif

/* Timecounter structures and functions from timetc.h */
struct timecounter;
typedef u_int timecounter_get_t(struct timecounter *);

/*
 * Locks used to protect struct members in this file:
 *	I	immutable after initialization
 *	T	tc_lock
 *	W	windup_mtx
 */

struct timecounter {
	timecounter_get_t	*tc_get_timecount;	/* [I] */
	/*
	 * This function reads the counter.  It is not required to
	 * mask any unimplemented bits out, as long as they are
	 * constant.
	 */
	u_int 			tc_counter_mask;	/* [I] */
	/* This mask should mask off any unimplemented bits. */
	u_int64_t		tc_frequency;		/* [I] */
	/* Frequency of the counter in Hz. */
	char			*tc_name;		/* [I] */
	/* Name of the timecounter. */
	int			tc_quality;		/* [I] */
	/*
	 * Used to determine if this timecounter is better than
	 * another timecounter higher means better.  Negative
	 * means "only use at explicit request".
	 */
	void			*tc_priv;		/* [I] */
	/* Pointer to the timecounter's private parts. */
	int			tc_user;		/* [I] */
	/* Expose this timecounter to userland. */
	SLIST_ENTRY(timecounter) tc_next;		/* [I] */
	/* Pointer to the next timecounter. */
	int64_t			tc_freq_adj;		/* [T,W] */
	/* Current frequency adjustment. */
	u_int64_t		tc_precision;		/* [I] */
	/* Precision of the counter.  Computed in tc_init(). */
};

struct timehands {
	struct timecounter *th_counter;
	int64_t th_adjtimedelta;
	struct bintime th_next_ntp_update;
	int64_t th_adjustment;
	u_int64_t th_scale;
	u_int th_offset_count;
	struct bintime th_boottime;
	struct bintime th_offset;
	struct bintime th_naptime;
	struct timeval th_microtime;
	struct timespec th_nanotime;
	volatile u_int th_generation;
	struct timehands *th_next;
};

/* Dummy timecounter functions and initialization from kern_tc.c */
u_int dummy_get_timecount(struct timecounter *tc) {
	static u_int now;
	return atomic_inc_int_nv(&now);
}

static struct timecounter dummy_timecounter = {
	.tc_get_timecount = dummy_get_timecount,
	.tc_counter_mask = ~0u,
	.tc_frequency = 1000000,
	.tc_name = "dummy",
	.tc_quality = -1000000,
	.tc_priv = NULL,
	.tc_user = 0,
	};

static struct timehands th0;
static struct timehands th1 = {
	.th_next = &th0
	};

static struct timehands th0 = {
	.th_counter = &dummy_timecounter,
	.th_scale = UINT64_MAX / 1000000,
	.th_offset = { .sec = 1, .frac = 0 },
	.th_generation = 1,
	.th_next = &th1
	};

static struct timehands *volatile timehands = &th0;

void
getmicrouptime(struct timeval *tvp)
{
	struct timehands *th;
	u_int gen;

	do {
		th = timehands;
		gen = th->th_generation;
		membar_consumer();
		BINTIME_TO_TIMEVAL(&th->th_offset, tvp);
		membar_consumer();
	} while (gen == 0 || gen != th->th_generation);
}
/* sched.h */
#define CPUSTATES	6
#define LIST_HEAD(name, type)						\
struct name {								\
struct type *lh_first;	/* first element */			\
}
#define	SCHED_NQS	32			/* 32 run queues. */
struct schedstate_percpu {
	struct proc *spc_idleproc;	/* idle proc for this cpu */
	TAILQ_HEAD(prochead, proc) spc_qs[SCHED_NQS];
	// LIST_HEAD(,proc) spc_deadproc;
	struct timespec spc_runtime;	/* time curproc started running */
	volatile int spc_schedflags;	/* flags; see below */
	u_int spc_schedticks;		/* ticks for schedclock() */
	u_int64_t spc_cp_time[CPUSTATES]; /* CPU state statistics */
	u_char spc_curpriority;		/* usrpri of curproc */

	struct clockintr *spc_itimer;	/* [o] itimer_update handle */
	struct clockintr *spc_profclock; /* [o] profclock handle */
	struct clockintr *spc_roundrobin; /* [o] roundrobin handle */
	struct clockintr *spc_statclock; /* [o] statclock handle */

	u_int spc_nrun;			/* procs on the run queues */

	volatile uint32_t spc_whichqs;
	volatile u_int spc_spinning;	/* this cpu is currently spinning */

	SIMPLEQ_HEAD(, smr_entry) spc_deferred; /* deferred smr calls */
	u_int spc_ndeferred;		/* number of deferred smr calls */
	u_int spc_smrdepth;		/* level of smr nesting */
	u_char spc_smrexpedite;		/* if set, dispatch smr entries
					 * without delay */
	u_char spc_smrgp;		/* this CPU's view of grace period */
};

/* clockintr.h */
struct clockintr_stat {
	uint64_t cs_dispatched;		/* total time in dispatch (ns) */
	uint64_t cs_early;		/* number of early dispatch calls */
	uint64_t cs_earliness;		/* total earliness (ns) */
	uint64_t cs_lateness;		/* total lateness (ns) */
	uint64_t cs_prompt;		/* number of prompt dispatch calls */
	uint64_t cs_run;		/* number of events dispatched */
	uint64_t cs_spurious;		/* number of spurious dispatch calls */
};

struct intrclock {
	void *ic_cookie;
	void (*ic_rearm)(void *, uint64_t);
	void (*ic_trigger)(void *);
};

struct clockrequest {
	uint64_t cr_expiration;			/* [o] copy of dispatch time */
	struct clockintr_queue *cr_queue;	/* [I] enclosing queue */
	uint32_t cr_flags;			/* [o] CR_* flags */
};

struct clockintr_queue {
	struct clockrequest cq_request;	/* [o] callback request object */
	struct mutex cq_mtx;		/* [a] per-queue mutex */
	uint64_t cq_uptime;		/* [o] cached uptime */
	TAILQ_HEAD(, clockintr) cq_all;	/* [m] established clockintr list */
	TAILQ_HEAD(, clockintr) cq_pend;/* [m] pending clockintr list */
	struct clockintr *cq_running;	/* [m] running clockintr */
	struct clockintr *cq_hardclock;	/* [o] hardclock handle */
	struct intrclock cq_intrclock;	/* [I] local interrupt clock */
	struct clockintr_stat cq_stat;	/* [o] dispatch statistics */
	volatile uint32_t cq_gen;	/* [o] cq_stat update generation */
	volatile uint32_t cq_dispatch;	/* [o] dispatch is running */
	uint32_t cq_flags;		/* [m] CQ_* flags; see below */
};

/* cpu.h */
struct cpu_info {
	struct device *ci_dev; /* Device corresponding to this CPU */
	struct cpu_info *ci_next;
	struct schedstate_percpu ci_schedstate; /* scheduler state */

	u_int32_t ci_cpuid;
	uint64_t ci_hartid;
	int ci_node;
	struct cpu_info *ci_self;

	struct proc *ci_curproc;
	struct pmap *ci_curpm;
	u_int32_t ci_randseed;

	struct pcb *ci_curpcb;
	struct pcb *ci_idle_pcb;

	struct clockintr_queue ci_queue;
	volatile int ci_timer_deferred;

	uint32_t ci_cpl;
	uint32_t ci_ipending;
	uint32_t ci_idepth;
#ifdef DIAGNOSTIC
	int ci_mutex_level;
#endif
	int ci_want_resched;

	struct opp_table *ci_opp_table;
	volatile int ci_opp_idx;
	volatile int ci_opp_max;
	uint32_t ci_cpu_supply;

#ifdef MULTIPROCESSOR
	struct srp_hazard ci_srp_hazards[SRP_HAZARD_NUM];
	volatile int ci_flags;
	uint64_t ci_satp;
	vaddr_t ci_initstack_end;
	int ci_ipi_reason;

	volatile int ci_ddb_paused;
#define CI_DDB_RUNNING 0
#define CI_DDB_SHOULDSTOP 1
#define CI_DDB_STOPPED 2
#define CI_DDB_ENTERDDB 3
#define CI_DDB_INDDB 4

#endif

#ifdef GPROF
	struct gmonparam *ci_gmon;
	struct clockintr *ci_gmonclock;
#endif

	char ci_panicbuf[512];
};
static inline struct cpu_info *
curcpu(void)
{
	struct cpu_info *__ci = NULL;
	__asm volatile("mv %0, tp" : "=&r"(__ci));
	return (__ci);
}

/* kern_lock.c */
void
mtx_enter(struct mutex *mtx)
{
	struct schedstate_percpu *spc = &curcpu()->ci_schedstate;
#ifdef MP_LOCKDEBUG
	int nticks = __mp_lock_spinout;
#endif

	WITNESS_CHECKORDER(MUTEX_LOCK_OBJECT(mtx),
	    LOP_EXCLUSIVE | LOP_NEWORDER, NULL);

	spc->spc_spinning++;
	while (mtx_enter_try(mtx) == 0) {
		CPU_BUSY_CYCLE();

#ifdef MP_LOCKDEBUG
		if (--nticks == 0) {
			db_printf("%s: %p lock spun out\n", __func__, mtx);
			db_enter();
			nticks = __mp_lock_spinout;
		}
#endif
	}
	spc->spc_spinning--;
}

/*
 * kern_time.c
 */
struct mutex ratecheck_mtx = MUTEX_INITIALIZER(IPL_HIGH);
int
ratecheck(struct timeval *lasttime, const struct timeval *mininterval)
{
	struct timeval tv, delta;
	int rv = 0;

	getmicrouptime(&tv);

	mtx_enter(&ratecheck_mtx);
	timersub(&tv, lasttime, &delta);

	/*
	 * check for 0,0 is so that the message will be seen at least once,
	 * even if interval is huge.
	 */
	if (timercmp(&delta, mininterval, >=) ||
	    (lasttime->tv_sec == 0 && lasttime->tv_usec == 0)) {
		*lasttime = tv;
		rv = 1;
	    }
	mtx_leave(&ratecheck_mtx);

	return (rv);
}

void *
malloc(size_t size, int type, int flags)
{
	struct kmembuckets *kbp;
	struct kmemusage *kup;
	struct kmem_freelist *freep;
	long indx, npg, allocsize;
	caddr_t va, cp;
	int s;
#ifdef DIAGNOSTIC
	int freshalloc;
	char *savedtype;
#endif
#ifdef KMEMSTATS
	struct kmemstats *ksp = &kmemstats[type];
	int wake;

	if (((unsigned long)type) <= 1 || ((unsigned long)type) >= M_LAST)
		panic("malloc: bogus type %d", type);
#endif

	KASSERT(flags & (M_WAITOK | M_NOWAIT));

#ifdef DIAGNOSTIC
	if ((flags & M_NOWAIT) == 0) {
		extern int pool_debug;
		assertwaitok();
		if (pool_debug == 2)
			yield();
	}
#endif

	if (size > 65535 * PAGE_SIZE) {
		if (flags & M_CANFAIL) {
#ifndef SMALL_KERNEL
			if (ratecheck(&malloc_lasterr, &malloc_errintvl))
				printf("malloc(): allocation too large, "
				    "type = %d, size = %lu\n", type, size);
#endif
			return (NULL);
		} else
			panic("malloc: allocation too large, "
			    "type = %d, size = %lu", type, size);
	}

	indx = BUCKETINDX(size);
	if (size > MAXALLOCSAVE)
		allocsize = round_page(size);
	else
		allocsize = 1 << indx;
	kbp = &bucket[indx];
	mtx_enter(&malloc_mtx);
#ifdef KMEMSTATS
	while (ksp->ks_memuse >= ksp->ks_limit) {
		if (flags & M_NOWAIT) {
			mtx_leave(&malloc_mtx);
			return (NULL);
		}
#ifdef DIAGNOSTIC
		if (ISSET(flags, M_WAITOK) && curproc == &proc0)
			panic("%s: cannot sleep for memory during boot",
			    __func__);
#endif
		if (ksp->ks_limblocks < 65535)
			ksp->ks_limblocks++;
		msleep_nsec(ksp, &malloc_mtx, PSWP+2, memname[type], INFSLP);
	}
	ksp->ks_memuse += allocsize; /* account for this early */
	ksp->ks_size |= 1 << indx;
#endif
	if (XSIMPLEQ_FIRST(&kbp->kb_freelist) == NULL) {
		mtx_leave(&malloc_mtx);
		npg = atop(round_page(allocsize));
		s = splvm();
		va = (caddr_t)uvm_km_kmemalloc_pla(kmem_map, NULL,
		    (vsize_t)ptoa(npg), 0,
		    ((flags & M_NOWAIT) ? UVM_KMF_NOWAIT : 0) |
		    ((flags & M_CANFAIL) ? UVM_KMF_CANFAIL : 0),
		    no_constraint.ucr_low, no_constraint.ucr_high,
		    0, 0, 0);
		splx(s);
		if (va == NULL) {
			/*
			 * Kmem_malloc() can return NULL, even if it can
			 * wait, if there is no map space available, because
			 * it can't fix that problem.  Neither can we,
			 * right now.  (We should release pages which
			 * are completely free and which are in buckets
			 * with too many free elements.)
			 */
			if ((flags & (M_NOWAIT|M_CANFAIL)) == 0)
				panic("malloc: out of space in kmem_map");

#ifdef KMEMSTATS
			mtx_enter(&malloc_mtx);
			ksp->ks_memuse -= allocsize;
			wake = ksp->ks_memuse + allocsize >= ksp->ks_limit &&
			    ksp->ks_memuse < ksp->ks_limit;
			mtx_leave(&malloc_mtx);
			if (wake)
				wakeup(ksp);
#endif
			return (NULL);
		}
		mtx_enter(&malloc_mtx);
#ifdef KMEMSTATS
		kbp->kb_total += kbp->kb_elmpercl;
#endif
		kup = btokup(va);
		kup->ku_indx = indx;
#ifdef DIAGNOSTIC
		freshalloc = 1;
#endif
		if (allocsize > MAXALLOCSAVE) {
			kup->ku_pagecnt = npg;
			goto out;
		}
#ifdef KMEMSTATS
		kup->ku_freecnt = kbp->kb_elmpercl;
		kbp->kb_totalfree += kbp->kb_elmpercl;
#endif
		cp = va + (npg * PAGE_SIZE) - allocsize;
		for (;;) {
			freep = (struct kmem_freelist *)cp;
#ifdef DIAGNOSTIC
			/*
			 * Copy in known text to detect modification
			 * after freeing.
			 */
			poison_mem(cp, allocsize);
			freep->kf_type = M_FREE;
#endif /* DIAGNOSTIC */
			XSIMPLEQ_INSERT_HEAD(&kbp->kb_freelist, freep,
			    kf_flist);
			if (cp <= va)
				break;
			cp -= allocsize;
		}
	} else {
#ifdef DIAGNOSTIC
		freshalloc = 0;
#endif
	}
	freep = XSIMPLEQ_FIRST(&kbp->kb_freelist);
	XSIMPLEQ_REMOVE_HEAD(&kbp->kb_freelist, kf_flist);
	va = (caddr_t)freep;
#ifdef DIAGNOSTIC
	savedtype = (unsigned)freep->kf_type < M_LAST ?
		memname[freep->kf_type] : "???";
	if (freshalloc == 0 && XSIMPLEQ_FIRST(&kbp->kb_freelist)) {
		int rv;
		vaddr_t addr = (vaddr_t)XSIMPLEQ_FIRST(&kbp->kb_freelist);

		vm_map_lock(kmem_map);
		rv = uvm_map_checkprot(kmem_map, addr,
		    addr + sizeof(struct kmem_freelist), PROT_WRITE);
		vm_map_unlock(kmem_map);

		if (!rv)  {
			printf("%s %zd of object %p size 0x%lx %s %s"
			    " (invalid addr %p)\n",
			    "Data modified on freelist: word",
			    (int32_t *)&addr - (int32_t *)kbp, va, size,
			    "previous type", savedtype, (void *)addr);
		}
	}

	/* Fill the fields that we've used with poison */
	poison_mem(freep, sizeof(*freep));

	/* and check that the data hasn't been modified. */
	if (freshalloc == 0) {
		size_t pidx;
		uint32_t pval;
		if (poison_check(va, allocsize, &pidx, &pval)) {
			panic("%s %zd of object %p size 0x%lx %s %s"
			    " (0x%x != 0x%x)\n",
			    "Data modified on freelist: word",
			    pidx, va, size, "previous type",
			    savedtype, ((int32_t*)va)[pidx], pval);
		}
	}

	freep->kf_spare0 = 0;
#endif /* DIAGNOSTIC */
#ifdef KMEMSTATS
	kup = btokup(va);
	if (kup->ku_indx != indx)
		panic("malloc: wrong bucket");
	if (kup->ku_freecnt == 0)
		panic("malloc: lost data");
	kup->ku_freecnt--;
	kbp->kb_totalfree--;
out:
	kbp->kb_calls++;
	ksp->ks_inuse++;
	ksp->ks_calls++;
	if (ksp->ks_memuse > ksp->ks_maxused)
		ksp->ks_maxused = ksp->ks_memuse;
#else
out:
#endif
	mtx_leave(&malloc_mtx);

	if ((flags & M_ZERO) && va != NULL)
		memset(va, 0, size);

	TRACEPOINT(uvm, malloc, type, va, size, flags);

	return (va);
}