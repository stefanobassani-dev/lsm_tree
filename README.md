# LSM Tree

## Compile and build

If you don't have CMake, install it.

`sudo apt install cmake`

Then proceed with

* `mkdir build`
* `cd build`
* `cmake ..`
* `cmake --build .`

## Command

* **insert**: `p \<key\> \<value\>`
* **search**: `g \<key\>`


## TODO

* Persistent File Tracking: Implement a mechanism to store and retrieve the last used file_id. This can be achieved by saving the ID to a metadata file or by implementing a scanning logic that checks the storage directory on startup to identify the most recent SSTable.

* Size-Tiered Compaction Strategy: Transition from the current basic merge logic to a Size-Tiered Compaction Strategy (STCS). This will allow for more efficient merging of SSTables by grouping files of similar sizes, reducing write amplification and improving overall disk I/O performance.

* Tombstone Management: Refine the compaction logic to safely evict "Tombstone" records only during the final merge level to ensure data is not accidentally resurrected while optimizing disk space.

* Metadata & Indexing: Add sparse indexing for SSTables to speed up get operations by avoiding full file scans during disk searches.
