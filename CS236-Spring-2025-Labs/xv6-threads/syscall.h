// System call numbers
#define SYS_fork    1
#define SYS_exit    2
#define SYS_wait    3
#define SYS_pipe    4
#define SYS_read    5
#define SYS_kill    6
#define SYS_exec    7
#define SYS_fstat   8
#define SYS_chdir   9
#define SYS_dup    10
#define SYS_getpid 11
#define SYS_sbrk   12
#define SYS_sleep  13
#define SYS_uptime 14
#define SYS_open   15
#define SYS_write  16
#define SYS_mknod  17
#define SYS_unlink 18
#define SYS_link   19
#define SYS_mkdir  20
#define SYS_close  21

///////////////// New addition ///////////////
#define SYS_thread_create 22
#define SYS_thread_exit 23
#define SYS_thread_join 24

#define SYS_waitpid             30

#define SYS_barrier_init        31
#define SYS_barrier_check       32
//////////////// End of new addition /////////

/////////// Parts D and E of threads lab/////////
#define SYS_sleepChan           36
#define SYS_getChannel          37
#define SYS_sigChan             38
#define SYS_sigOneChan          39
/////////// End of Parts D and E of threads lab/////////
