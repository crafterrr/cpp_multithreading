#pragma once

#include <vector>
#include <stdint.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <vector>
#include <stack>
#include <condition_variable>
// #include "exceptions.h"

using namespace std;

class InputParallelizer {
public:
    
    InputParallelizer(string in, string out);
    
    void controlWorker();
    void readWorker();
    void writeWorker();
    
    template<class T, class T2, class T3>
    void funcWorker(T (T3::*func)(T2& arg), T3& obj, int id=0){
        while(!finished_reading || tasks.size() != 0){
            unique_lock<mutex> lck(mtx);
            while(paused) fworker.wait(lck);
            while(tasks.size() == 0){
                //            ioworker.notify_all(); // should it be here?
#ifdef DEBUG
                cout << "Func worker waits task" << endl;
#endif
                
                fworker.wait(lck);
                
#ifdef DEBUG
                cout << "Func worker waits no longer" << endl;
#endif
                
            }
            T2 a;
            stringstream conv;
            conv << tasks.top();
            tasks.pop();
            lck.unlock();
            conv >> a;
            T b = (obj.*func)(a);
            
#ifdef DEBUG
            this_thread::sleep_for(chrono::milliseconds(rand() % 5 * 1000));
#endif
            
            lck.lock();
            completed_tasks.push(string(b));
            
#ifdef DEBUG
            cout << "Thread " << id << " finished applying func: " << b << endl;
#endif
            
            lck.unlock();
            ioworker.notify_all(); // or here?
        }
    }
    
    template<class T, class T2, class T3>
    const void parallelize(T (T3::*func)(T2& arg), T3& obj, const int n_jobs=1){
        finished_reading = false;
        finished_calc = false;
        paused = false;
        stopped = false;
        
        vector<thread> func_workers;
        thread read_worker = thread(&InputParallelizer::readWorker, this);
        thread write_worker = thread(&InputParallelizer::writeWorker, this);
        thread control = thread(&InputParallelizer::controlWorker, this);
        
        for (int i=0; i<n_jobs; i++){
            func_workers.push_back(thread(&InputParallelizer::funcWorker<T, T2, T3>,
                                          this, func, ref(obj),
                                          i));
        }
        
        read_worker.join();
        
#ifdef DEBUG
        cout << "Read worker joined" << endl;
#endif
        
        for (auto &s : func_workers){
            s.join();
        }
        finished_calc = true;
        
#ifdef DEBUG
        cout << "Calculations ended" << endl;
#endif
        
        ioworker.notify_all();
        write_worker.join();
        
#ifdef DEBUG
        cout << "Write worker joined" << endl;
#endif
        
        if (!stopped) cout << "Finished applying func on file, type something to quit" << endl;
        control.join();
        
#ifdef DEBUG
        cout << "Control worker joined" << endl;
#endif
        
        finished_reading = false;
        finished_calc = false;
        paused = false;
        stopped = false;
    }
    
private:
    ifstream input;
    ofstream output;
    mutex mtx;
    condition_variable fworker, ioworker;
    bool finished_reading = false;
    bool finished_calc = false;
    bool paused = false;
    bool stopped = false;
    stack<string> tasks;
    stack<string> completed_tasks;
    
};
