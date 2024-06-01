CC = gcc
CFLAGS = -Wall -pthread

# Targets for both schedulers
all: fcfs_scheduler sjf_scheduler srtf_scheduler np_priority_scheduler p_priority_scheduler rr_scheduler

# FCFS Scheduler target
fcfs_scheduler: main_fcfs.o process.o fcfs_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o fcfs_scheduler main_fcfs.o process.o fcfs_scheduler.o scheduler.o

# SJF Scheduler target
sjf_scheduler: main_sjf.o process.o sjf_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o sjf_scheduler main_sjf.o process.o sjf_scheduler.o scheduler.o

# SRTF Scheduler target
srtf_scheduler: main_srtf.o process.o srtf_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o srtf_scheduler main_srtf.o process.o srtf_scheduler.o scheduler.o

# N_P_PRIORITY Scheduler target
np_priority_scheduler: main_np_priority.o process.o np_priority_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o np_priority_scheduler main_np_priority.o process.o np_priority_scheduler.o scheduler.o

# P_PRIORITY Scheduler target
p_priority_scheduler: main_p_priority.o process.o p_priority_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o p_priority_scheduler main_p_priority.o process.o p_priority_scheduler.o scheduler.o

# RR Scheduler target
rr_scheduler: main_rr.o process.o rr_scheduler.o scheduler.o
	$(CC) $(CFLAGS) -o rr_scheduler main_rr.o process.o rr_scheduler.o scheduler.o

# Main object file for FCFS
main_fcfs.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=1 -c main.c -o main_fcfs.o

# Main object file for SJF
main_sjf.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=2 -c main.c -o main_sjf.o

# Main object file for SRTF
main_srtf.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=3 -c main.c -o main_srtf.o

# Main object file for N_P_PRIORITY
main_np_priority.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=4 -c main.c -o main_np_priority.o

# Main object file for P_PRIORITY
main_p_priority.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=5 -c main.c -o main_p_priority.o

# Main object file for RR
main_rr.o: main.c
	$(CC) $(CFLAGS) -DSCHEDULER_TYPE=6 -c main.c -o main_rr.o

# Process object file
process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c -o process.o

# FCFS Scheduler object file
fcfs_scheduler.o: fcfs_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c fcfs_scheduler.c -o fcfs_scheduler.o

# SJF Scheduler object file
sjf_scheduler.o: sjf_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c sjf_scheduler.c -o sjf_scheduler.o

# SRTF Scheduler object file
srtf_scheduler.o: srtf_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c srtf_scheduler.c -o srtf_scheduler.o

# N_P_PRIORITY Scheduler object file
np_priority_scheduler.o: np_priority_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c np_priority_scheduler.c -o np_priority_scheduler.o

# P_PRIORITY Scheduler object file
p_priority_scheduler.o: p_priority_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c p_priority_scheduler.c -o p_priority_scheduler.o

# RR Scheduler object file
rr_scheduler.o: rr_scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c rr_scheduler.c -o rr_scheduler.o

# Scheduler object file
scheduler.o: scheduler.c scheduler.h
	$(CC) $(CFLAGS) -c scheduler.c -o scheduler.o

# Clean target to remove object files and executables
clean:
	rm -f *.o fcfs_scheduler sjf_scheduler srtf_scheduler np_priority_scheduler p_priority_scheduler rr_scheduler