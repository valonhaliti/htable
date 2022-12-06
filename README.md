# ht - hash table using shared memory

Server initiliazes the hash table. Client contacts with this server using a shared memory which is created by the server.

## How to run it

Compile it:

```
mkdir build
cd build
cmake -G 'Unix Makefiles' -DCMAKE_BUILD_TYPE=Debug  ..
make
```

After this you see the executables in the current directory.

You can run the server: `./htserver 1000` where 1000 is the hash table size, then `./htclient` (which for the momemnt it does some example requests inside).
