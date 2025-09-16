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

return 0;
}
