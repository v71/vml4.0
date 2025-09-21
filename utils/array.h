#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2050 v71 
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in
//	all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//	THE SOFTWARE.

namespace vml
{

	namespace utils
	{

        template <typename T>
        class Array
        {
            private:

                // ------------------------------------------------------------
                // growth rate 

                static const uint32 DefaultGrowth = 16;

                // ------------------------------------------------------------
                // private data

                T* Data;           // allocated data
                size_t Size;       // Occupied size
                size_t Capacity;   // Allocated capacity

                // ------------------------------------------------------------
                // Grows array according to default growth value

                void Grow()
                {
                    size_t new_capacity = Capacity + DefaultGrowth;
                    T* new_data = new T[new_capacity];
                    memcpy(new_data, Data, Capacity * sizeof(T));
                    vml::os::SafeDeleteArray(Data);
                    Data = new_data;
                    Capacity = new_capacity;

                    //     std::cout << "growing" << " " << capacity << std::endl;

                }

            public:

                // ------------------------------------------------------------
                // Clear array , this function does not 
                // delete the allocated array

                void Clear()
                {
                    Size = 0;
                }

                // ------------------------------------------------------------
                // Adds an element to the array

                void Push(const T& element)
                {
                    // if size is greater than capacity 
                    // grow the array to accomodate the new data
                    // the array is expanded by DefaultGrowth size

                    if (Size >= Capacity)
                        Grow();

                    // add new element to the array

                    Data[Size++] = element;
                }

                // ------------------------------------------------------------
                // Erase an element swapping the element to delete
                // with the last element 

                void Delete(uint32 index)
                {
                    #ifdef _DEBUG   
                    if (Size <= 0 || index >= Size)
                        vml::os::Message::Error("index out of range");
                    #endif

                    // swap last element with the element to delete

                    Data[index] = Data[--Size];
                }

                // ------------------------------------------------
                // Get the array size

                size_t GetSize() const
                {
                    return Size;
                }

                // ------------------------------------------------
                // Get the array capacity

                size_t GetCapacity() const
                {
                    return Capacity;
                }

                // ------------------------------------------------
                // Get array size in bytes

                size_t GetSizeInBytes() const
                {
                    return Size * sizeof(T);
                }

                // ------------------------------------------------
                // Get the array capacity in bytes

                size_t GetCapacityInBytes() const
                {
                    return Capacity * sizeof(T);
                }

                // ------------------------------------------------
                // subscript access 

                T& operator [](size_t index)
                {
                    #ifdef _DEBUG   
                    if (index >= Size || index < 0)
                        vml::os::Message::Error("index out of range");
                    #endif

                    // return value at index

                    return Data[index];
                }

                // ------------------------------------------------
                // const subscript access 

                const T& operator [](size_t index) const
                {
                    #ifdef _DEBUG   
                    if (index >= Size || index < 0)
                        vml::os::Message::Error("index out of range");
                    #endif

                    // return value at index

                    return Data[index];
                }

                // ------------------------------------------------
                // get data array const pointer

                const T* GetPtr() const
                {
                    return Data;
                }

                // ------------------------------------------------
                // get data array pointer

                T* GetPtr()
                {
                    return Data;
                }

                // ------------------------------------------------
                // const subscript access 

               T &At(size_t index) const
                {
                    #ifdef _DEBUG   
                    if (index >= Size || index < 0)
                        vml::os::Message::Error("index out of range");
                    #endif

                    // return value at index

                    return Data[index];
                }

                // ------------------------------------------------
                // ctor / dtor

                Array(int new_capacity = DefaultGrowth)
                {
                    if (new_capacity <= 0)
                        vml::os::Message::Error("capacity must be greater than zero");
                    if (new_capacity < DefaultGrowth)
                        Capacity = DefaultGrowth;
                    Size = 0;
                    Capacity = new_capacity;
                    Data = new T[Capacity];
                }

                ~Array()
                {
                    Size = 0;
                    Capacity = 0;
                    vml::os::SafeDeleteArray(Data);
                }

        };

	}
}
