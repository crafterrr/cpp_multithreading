#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <stack>
#include <condition_variable>
#include "InputParallelizer.h"
// #include "exceptions.h"

using namespace std;

    
InputParallelizer::InputParallelizer(string in, string out){
        input.open(in);
        output.open(out);
    }
    
void InputParallelizer::controlWorker(){
        string cmd;
        while (!finished_calc){
            cin >> cmd;
            if (cmd == "pause") paused = true;
            else if (cmd == "resume" && paused) {
                paused = false;
                fworker.notify_all();
            } else if (cmd == "exit") {
                stopped = true;
                break;
            }
        }
    }

void InputParallelizer::readWorker(){
        string s;
        unique_lock<mutex> lck(mtx);
        while (getline(input, s) && !stopped){
            while(tasks.size() >= 10) {
#ifdef DEBUG
                cout << "Reader waits until task completion" << endl;
#endif
                ioworker.wait(lck);
            }
            if (stopped) break;
            tasks.push(s);
            fworker.notify_one();
        }
        finished_reading = true;
    }
    
void InputParallelizer::writeWorker(){
        string s;
        unique_lock<mutex> lck(mtx);
        while (!finished_calc || completed_tasks.size() != 0){
            while(completed_tasks.size() == 0){
#ifdef DEBUG
                cout << "Writer has 0 completed tasks" << endl;
#endif
                ioworker.wait(lck);
                if (finished_calc) return;
            }
            output << completed_tasks.top() << endl;
            completed_tasks.pop();
        }
    }
