#include "Hashset.h"
#include "shared_data.h"
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <iostream>
#include <sstream>

namespace bip = boost::interprocess;

void loop_queue(Hashset& hs, req_queue* data)
{
    while (true) {
        bip::scoped_lock<bip::interprocess_mutex> lock(data->mutex);
        if (!data->message_in) {
            data->cond_full.wait(lock); // wait until a new message comes from client
        }

        struct ReqStruct inp = data->req;
        struct ResStruct res {
            false, 0
        };
        if (inp.req_operation == ReqOperationFind) {
            res.success = false;
            res.value = 0;

            auto e = hs.find(inp.key);
            if (e != nullptr) {
                res.success = true;
                res.value = e->value;
            }
            data->res = res;

            data->message_in = false;

            data->cond_empty.notify_one();
        } else if (inp.req_operation == ReqOperationInsert) {
            hs.insert(inp.key);
            res.success = true;
            data->res = res;

            data->message_in = false;

            data->cond_empty.notify_one();
        } else if (inp.req_operation == ReqOperationDel) {
            hs.del(inp.key);
            res.success = true;
            data->res = res;

            data->message_in = false;

            data->cond_empty.notify_one();
        }
    }
}

int main(int argc, char** argv)
{
    std::cout << "Server executing" << std::endl;

    bip::shared_memory_object::remove("SharedMem");
    bip::shared_memory_object shm { bip::create_only, "SharedMem", bip::read_write };

    shm.truncate(sizeof(req_queue));
    bip::mapped_region region { shm, bip::read_write };
    void* addr = region.get_address();
    req_queue* data = new (addr) req_queue;

    size_t hs_size = 1024 * 1024;
    if (argc >= 2) {
        std::istringstream iss(argv[1]);
        int val;
        if (iss >> val) {
            hs_size = val;
        }
    }

    std::cout << "hs_size: " << hs_size << std::endl;
    Hashset hs(hs_size);
    loop_queue(hs, data);
}
