# Cache
* AVEL offers facilities for read and write prefetching
* prefetches are performed with a hint for a particular level in the cache 
  hierarchy which is an enum value of type `avel::Cache_level` 
* `avel::Cache_level` features three values:
  * `L1_CACHE` 
  * `L2_CACHE` 
  * `L3_CACHE` 
* AVEL by default assumes that cache lines for all three level are 64 bytes 
  in size
  * this can be configured by defining the `AVEL_L1_CACHE_LINE_SIZE`, 
    `AVEL_L2_CACHE_LINE_SIZE`, and `AVEL_L3_CACHE_LINE_SIZE` macros to the 
    desired values
  * these macros may also be defined by setting CMake variables of the same name 
    to the desired values
* as a rule of thumb, software prefetching is not beneficial, and may even 
  be actively detrimental, in cases where the hardware prefetcher is capable 
  predicting future memory accesses, such as with a linear access pattern
* software prefetching may be more useful in cases where the data being 
  processed is laid out in non-contiguous blocks such as an image with 
  a tiled representation
  * even then, depending on the size of said blocks, hardware prefetching is 
    likely to supersede software prefetching after a short while, meaning it's 
    likely only beneficial to software prefetch a small portion of each block

## Prefetch Functions

* `void prefetch_read<Cache_level>(const void* ptr, std::size_t n = 1)`
  * prefetches data for reading in the near future
  * prefetches as many cache lines as required to ensure that `n` bytes have 
    been prefetched

* `void prefetch_read<Cache_level, T>(const T* ptr, std::size_t n = 1)`
  * prefetches data for reading in the near future
  * prefetches as many cache lines as required to ensure that `n` objects of 
    type `T` have been prefetched

* `void prefetch_write<Cache_evel>(const void* ptr, std::size_t n = 1)`
  * prefetches data for writing in the near future
  * prefetches as many cache lines as required to ensure that `n` bytes have 
    been prefetched

* `void prefetch_write<Cache_level, T>(const T* ptr, std::size_t n = 1)`
  * prefetches data for writing in the near future
  * prefetches as many cache lines as required to ensure that `n` objects of 
    type `T` have been prefetched
