//Group Members: Celeste Garlejo, Nadia Ghanizada, Sara Lee, Martin Nguyen
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


    int allocate_pid(){
        if(!is_ready_ || bitmap_.empty()){
                return -1;
        }
        for (size_t i = 0; i <bitmap_.size(); ++i){
                if(bitmap_[i] == 0){
                bitmap_[i] = 1; // used mark
                return static_cast<int>(MIN_PID +i);//returnPID value
                }
        }
        return -1;}
   // Allocates and returns aPID; returns −1 if unable to allocate aPID (all pids are in use)
    void releaser_pid(){} // Releases aPID

    private:
        vector<unsigned char> bitmap_; // 0 or 1
        bool is_ready_ = false; // initialized to false, will be true if allocate_map() is successful
};

int main() {
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

// what if test case: calling allocate_pid before allocate_map
    PIDManager test_manager;
    int test_pid = test_manager.allocate_pid();
    cout << "allocate_pid() before allocate_map returned: " << test_pid << " (should be -1)" << endl << endl;

    // forking to create parent and child processes to test independent PID allocation
    pid_t pid = fork();
    if (pid < 0) {
        cerr << "fork failed" << endl;
        return 1;
    }

    if (pid == 0) {
        // child process
        cout << "--- Child Process ---" << endl;
        PIDManager child_manager;
        if (child_manager.allocate_map() == 1) {
            cout << "Child: allocate_map() success, bitmap size = " << child_manager.bitmap_size() << endl;
            int pids[3];
            for (int i = 0; i < 3; ++i) {
                pids[i] = child_manager.allocate_pid();
                cout << "Child: Allocated PID: " << pids[i] << endl;
                // child_manager.release_pid(pids[i]); // UNCOMMENT WHEN IMPLEMENTED
            }
            cout << "Child: completed work, releasing PIDs..." << endl;
        } else {
            cout << "Child: allocate_map() failed" << endl;
        }
        _exit(0);
    } else {
        // parent process
        cout << "--- Parent Process ---" << endl;
        PIDManager parent_manager;
        if (parent_manager.allocate_map() == 1) {
            cout << "Parent: allocate_map() success, bitmap size = " << parent_manager.bitmap_size() << endl;
            int pids[3];
            for (int i = 0; i < 3; ++i) {
                pids[i] = parent_manager.allocate_pid();
                cout << "Parent: Allocated PID: " << pids[i] << endl;
                // parent_manager.release_pid(pids[i]); // UNCOMMENT WHEN IMPLEMENTED
            }
            cout << "Parent: completed work, releasing PIDs..." << endl;
        } else {
            cout << "Parent: allocate_map() failed" << endl;
        }

        wait(NULL); // wait for child to finish
        cout << "Parent: child process finished" << endl;
        
        return 0;
    }
}


// redundant after main implementation, delete when finished
//     // testing allocate_map() function with output of bitmap size and success/failure message
//     PIDManager pid_manager;
//     int result = pid_manager.allocate_map();
//     if (result == 1) {
//         cout << "allocate_map() success; bitmap size = " << pid_manager.bitmap_size() << '\n';
//     } else {
//         cout << "allocate_map() failed\n";
//     }
// //TEST FOR ALLOCATED_PID
//         cout << "Allocating 5 PIDS test...\n";
//         int pids[5];
//         for (int i = 0; i <5; ++i){
//                 pids[i] = pid_manager.allocate_pid();
//                 cout << "Allocated PID: " << pids[i] << endl;
//         }

// return 0;
// }
