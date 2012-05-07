#pragma once

#include <algorithm>


///////////////
void SwapBytes(unsigned char* memory, size_t size);

/////////////////
template <typename Number_>
void SwapBytes(Number_& number);


///////////////
inline void SwapBytes(unsigned char* memory, size_t size)
{
   register size_t i = 0;
   register size_t j = size - 1;
   while (i < j) {
      std::swap(memory[i], memory[j]);
      i++, j--;
   }
}

template <typename Number_>
inline void SwapBytes(Number_& number)
{
	SwapBytes(reinterpret_cast<unsigned char*>(&number), sizeof(number));
}
