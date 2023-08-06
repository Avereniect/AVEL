# Misc. Facilities
* ```c++
  template<class T, class U>
  AVEL_FINL T bit_cast(const U& u);
  ```
  * performs a cast such that the returned object has the same byte-wise 
    representation as the parameter `u`
  * relies on implementation-defined behavior pre C++ 20
  * relies on `std::bit_cast` if compiling with C++ 20