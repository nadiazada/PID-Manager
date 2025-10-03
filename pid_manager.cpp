//Group Members: Celeste Garlejo, Nadia Ghanizada, Sara Lee, Martin Nguyen
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <new>
#include <cstring> // for strlen
#include <cstdio>

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
    
   // Releases a PID, making it available again
void release_pid(int pid) {
    if (!is_ready_ || bitmap_.empty()) {
        // if allocate_map() hasn't been called yet, just ignore
        return;
    }
    if (pid < MIN_PID || pid > MAX_PID) {
        // invalid PID, ignore
        return;
    }
    size_t index = static_cast<size_t>(pid - MIN_PID);
    bitmap_[index] = 0; // mark it as free again
}


    private:
        vector<unsigned char> bitmap_; // 0 or 1
        bool is_ready_ = false; // initialized to false, will be true if allocate_map() is successful
};

// implement new functions here



int main() {

    // new code for PID II assignment. TASK 1. DELETE AFTER FINISHING
    // setting up two anonymous pipes
    int child_to_parent[2];
    int parent_to_child[2];

    if(pipe(child_to_parent) == -1 || pipe(parent_to_child) == -1) {
        cerr << "Pipe failed" << endl;
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        cerr << "Fork failed" << endl;
        return 1;
    }

    if (pid == 0) {
        close(child_to_parent[0]);
        close(parent_to_child[1]);

        // insert TASK 3 below
        const char *allocMsg = "REQ_ALLOC\n";
        write(child_to_parent[1], allocMsg, strlen(allocMsg));

        // wait for parent to send back the allocated pid
        char cbuf[128];
        int n = read(parent_to_child[0], cbuf, sizeof(cbuf) - 1);
        if (n > 0) {
            cbuf[n] = '\0';
            int newPID = -1;
            if (sscanf(cbuf, "%d", &newPID) == 1) {
                std::cout << "Hello from child, received PID: "
                        << newPID << std::endl;

                // ask parent to release that pid
                char relMsg[64];
                snprintf(relMsg, sizeof(relMsg), "REQ_RELEASE %d\n", newPID);
                write(child_to_parent[1], relMsg, strlen(relMsg));
            }
        }

        // tell parent we are done
        const char *doneMsg = "DONE\n";
        write(child_to_parent[1], doneMsg, strlen(doneMsg));

        // close pipe ends in child
        close(child_to_parent[1]);
        close(parent_to_child[0]);
        _exit(0);

    } else {


    // *** FOR PARENT read request, allocate, send response, wait, done
    // ===================== PARENT =====================
        close(child_to_parent[1]);   // parent doesn't write to cto p
        close(parent_to_child[0]);   // parent doesn't read from p to c

        // Task 4: handle REQ_RELEASE and DONE only.
        PIDManager manager;
        manager.allocate_map(); // prepare PID table once

        char buf[128];

        while (true) {
            // read up to 1 byte less than buffer size
            int n = (int)read(child_to_parent[0], buf, sizeof(buf) - 1);
            if (n <= 0) {
                break;
            }
            buf[n] = '\0';
            if (strncmp(buf, "DONE", 4) == 0) {
                break;
            }

            if (strncmp(buf, "REQ_ALLOC", 9) == 0) {
                int newPID = manager.allocate_pid();
                char reply[32];
                snprintf(reply, sizeof(reply), "%d\n", newPID);
                write(parent_to_child[1], reply, strlen(reply));
            }


            //"REQ_RELEASE <pid>"
            if (strncmp(buf, "REQ_RELEASE", 11) == 0) {
                int relpid = -1;
                if (sscanf(buf, "REQ_RELEASE %d", &relpid) == 1) {
                    if (relpid >= MIN_PID && relpid <= MAX_PID) {
                        manager.release_pid(relpid);
                        //print:
                        std::cout << "Parent received request to release PID: "
                                  << relpid << std::endl;
                    }
                }
                // continue to read next message
            }

            //tast 3 will implement child reading and sending REQ_RELEASE + DONE.
        }

        close(child_to_parent[0]);
        close(parent_to_child[1]);
        wait(NULL);
        std::cout << "Parent: child process finished" << std::endl;
        return 0;
    }




    // Had to disable old test cases to avoid duplicate fork/pid (it wasnt running properly)
    #if 0
    /*
    
    !!!!!!!!!!!!!!!!!!!!!!!!    CHECK IF CODE BELOW IS NEEDED !!!!!!!!!!!!!
    
    */

// what if test case: calling allocate_pid before allocate_map
    PIDManager test_manager;
    int test_pid = test_manager.allocate_pid();
    cout << "allocate_pid() before allocate_map returned: " << test_pid << " (should be -1)" << endl << endl;

// what if test case: releasing before allocate_map
    test_manager.release_pid(MIN_PID); 
    cout << "release_pid(MIN_PID) before allocate_map(): safe no-op (program continues)\n\n";


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
                // release the pids we just got
                for (int i =0; i < 3; ++i) {
                    child_manager.release_pid(pids[i]);
                    cout << "Child: Released PID: " << pids[i] << endl;
                }
                // allocate again to see if they are available
                for (int i = 0; i < 3; ++i) {
                    int again = child_manager.allocate_pid();
                    cout << "Child: Allocated again: " << again << endl;
                }
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
                // release the pids we just got
                for (int i = 0; i < 3; ++i) {
                    parent_manager.release_pid(pids[i]);
                    cout << "Parent: Released PID: " << pids[i] << endl;
                }
                // allocate again to see if they are available
                for (int i = 0; i < 3; ++i) {
                    int again = parent_manager.allocate_pid();
                    cout << "Parent: Allocated again: " << again << endl;
                }
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
#endif
}
