//Group Members: 
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <new>

#define MIN_PID 100
#define MAX_PID 1000

using namespace std;

class PIDManager {
public:
    // Initializing bitmap for MIN_PID to MAX_PID & Return 1 on success, -1 on failure
    int allocate_map(){
        try {
        // Compute the number of PID
        const size_t number_of_pid = static_cast<size_t>(MAX_PID - MIN_PID + 1); // 901 total PIDs

        // (allocate size if needed and reset to all 0s (free))
        if (bitmap_.size() != number_of_pid) {
            bitmap_.assign(number_of_pid, 0);
        } else {
            fill(bitmap_.begin(), bitmap_.end(), 0);
        }
        // PIDs are now available
        is_ready_ = true;
        return 1;   // success

        // allocation failure (return -1)
        } catch (const bad_alloc&) {
            is_ready_ = false;
            bitmap_.clear();
            return -1;
        // unexpected failure (return -1)
        } catch (...) {
            is_ready_ = false;
            bitmap_.clear();
            return -1;
        }
    }

    // to test my funcion allocate_map() in main()
    size_t bitmap_size() const { return bitmap_.size(); }
    bool is_initialized() const { return is_ready_; }


    int allocate_pid(){return -1;} // Allocates and returns a pid; returns −1 if unable to allocate a pid (all pids are in use)
    void releaser_pid(){} // Releases a pid

    private:
        vector<unsigned char> bitmap_; // 0 or 1
        bool is_ready_ = false; // initialized to false, will be true if allocate_map() is successful
};

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

    // testing allocate_map() function with output of bitmap size and success/failure message
    PIDManager pid_manager;
    int result = pid_manager.allocate_map();
    if (result == 1) {
        cout << "allocate_map() success; bitmap size = " << pid_manager.bitmap_size() << '\n';
    } else {
        cout << "allocate_map() failed\n";
    }
return 0;
}
