#include "shared_data.h"
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <iostream>

namespace bip = boost::interprocess;

void make_call(req_queue* data, ReqOperation operation, uint64_t val)
{
    bip::scoped_lock<bip::interprocess_mutex> lock(data->mutex);
    if (data->message_in) {
        data->cond_empty.wait(lock);
    }

    data->req = { operation, val };
    data->message_in = true;
    data->cond_full.notify_one(); // now there are messages for the server

    // wait for the result
    data->cond_empty.wait(lock); // server is done processing our request
    if (operation == ReqOperationInsert)
        std::cout << "Insert success: " << data->res.success << std::endl;
    else if (operation == ReqOperationFind)
        std::cout << "Find result: " << data->res.value << " Success: " << data->res.success << std::endl;
    else if (operation == ReqOperationDel)
        std::cout << "Delete success: " << data->res.success << std::endl;
}

int main()
{

    std::cout << "Client executing" << std::endl;

    bip::shared_memory_object shm { bip::open_only, "SharedMem", bip::read_write };
    bip::mapped_region region { shm, bip::read_write };

    void* addr = region.get_address();

    req_queue* data = static_cast<req_queue*>(addr);

    make_call(data, ReqOperationInsert, 2);
    make_call(data, ReqOperationInsert, 3);
    make_call(data, ReqOperationFind, 2);
    make_call(data, ReqOperationFind, 10);
    make_call(data, ReqOperationDel, 2);
    make_call(data, ReqOperationFind, 2);
}
