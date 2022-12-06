#ifndef INCLUDE_SHARED_DATA_H
#define INCLUDE_SHARED_DATA_H

#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

enum ReqOperation {
    ReqOperationInsert = 0,
    ReqOperationDel = 1,
    ReqOperationFind = 2
};

struct ReqStruct {
    ReqOperation req_operation;
    uint64_t key;
};

struct ResStruct {
    bool success;
    uint64_t value;
};

struct req_queue {

    //Mutex to protect access to the queue
    boost::interprocess::interprocess_mutex mutex;

    //Condition to wait when the queue is empty
    boost::interprocess::interprocess_condition cond_empty;

    //Condition to wait when the queue is full
    boost::interprocess::interprocess_condition cond_full;

    //Items to fill
    ReqStruct req;

    ResStruct res;

    //Is there any message
    bool message_in;

    req_queue()
        : message_in(false)
    {
    }
};

#endif