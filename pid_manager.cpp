//Group Members: 
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MIN_PID 100
#define MAX_PID 1000

class PIDMan {
public:
int allocate_map(){} // Creates and initializes a data structure for representing pids; returns −1 if unsuccessful, 1 if successful
int allocate_pid(){} // Allocates and returns a pid; returns −1 if unable to allocate a pid (all pids are in use)
void releaser_pid(int pid){} // Releases a pid
private:
    int bitmap[SIZE]; // Array bitmap( gotta assign some SIZE)
}
int main(){
/*
Requirement test cases:
Call allocate_map to initialize the data structure.
Call allocate_pid multiple times to allocate PIDs.
Check if the allocated PIDs fall within the specified range.
Call release_pid for each allocated PID.
Check if the released PIDs become available for allocation again.

What if test cases: 
Test error handling by calling allocate_pid before calling allocate_map.
Verify that allocate_pid returns -1.
Test releasing a PID without initializing the data structure.
Allocate and release PIDs in a loop for a large number of iterations.
Test and check for memory leaks.
Randomly allocate and release PIDs multiple times.
Verify that the allocated PIDs are within the specified range.
Ensure that released PIDs become available for allocation.
Attempt to allocate a PID when the range is exhausted (MAX_PID - MIN_PID + 1 allocations).
Check if the function returns -1, indicating that all PIDs are in use.

*/
return 0;
}
