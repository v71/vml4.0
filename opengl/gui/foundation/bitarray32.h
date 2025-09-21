#pragma once

////////////////////////////////////////////////////////////////////////////////////
//	This source file is part of v71's engine
//
//	Copyright (c) 2011-2020 v71 
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

		///////////////////////////////////////////////////////////
		// 32 bit set class , this class is to be used as
		// a kind of static class.

		namespace bits32
		{
			
				//////////////////////////////////////////////////////
				// bits positions

				const static unsigned int BIT0  =          1;
				const static unsigned int BIT1  =		   2;
				const static unsigned int BIT2  =		   4;
				const static unsigned int BIT3  =		   8;
				const static unsigned int BIT4  =	 	  16;
				const static unsigned int BIT5  =		  32;
				const static unsigned int BIT6  =		  64;
				const static unsigned int BIT7  =        128;
				const static unsigned int BIT8  =        256;
				const static unsigned int BIT9  =	     512;
				const static unsigned int BIT10 =       1024;
				const static unsigned int BIT11 =	    2048;
				const static unsigned int BIT12 =       4096;
				const static unsigned int BIT13 =       8192;
				const static unsigned int BIT14 =      16384;
				const static unsigned int BIT15 =	   32768;
				const static unsigned int BIT16 =      65536;
				const static unsigned int BIT17 =	  131072;
				const static unsigned int BIT18 =     262144;
				const static unsigned int BIT19 =	  524288;
				const static unsigned int BIT20 =    1048576;
				const static unsigned int BIT21 =    2097152;
				const static unsigned int BIT22 =    4194304;
				const static unsigned int BIT23 =    8388608;
				const static unsigned int BIT24 =	16777216;
				const static unsigned int BIT25 =   33554432;
				const static unsigned int BIT26 =	67108864;
				const static unsigned int BIT27 =  134217728;
				const static unsigned int BIT28 =  268435456;
				const static unsigned int BIT29 =  536870912;
				const static unsigned int BIT30 = 1073741824;
				const static unsigned int BIT31 = 2147483648;
		
				/////////////////////////////////////////////////////////////////
				// clear all bit flags

				static void Clear( unsigned int &bitflag )
				{
					bitflag=0;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to true 

				static void SetToTrue( unsigned int &bitflag,unsigned int bit )
				{
					bitflag |=  bit;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to false

				static void SetToFalse( unsigned int &bitflag,unsigned int bit )
				{
					bitflag &= ~bit;
				}

				/////////////////////////////////////////////////////////////////
				// xor a single bit

				static void Toggle( unsigned int &bitflag,unsigned int bit )
				{
					bitflag ^= bit;
				}

				/////////////////////////////////////////////////////////////////
				// gets value of single bit

				static bool Get( const unsigned int &bitflag,unsigned int bit ) 
				{
					return ( ( bitflag & bit )!=0 );
				}

				/////////////////////////////////////////////////////////////////
				// sets value of single bit

				static void SetMask( unsigned int &bitflag,unsigned int mask )
				{
					bitflag=mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Bit functions based on bitmask

				static unsigned int SetBitsExcept( unsigned int &bitflag, unsigned int mask )
				{
					return bitflag | ~mask;
				}

				static unsigned int ClearBits( unsigned int &bitflag, unsigned int mask )
				{
					return bitflag & ~mask;
				}

				static unsigned int ClearBitsExcept(unsigned int &bitflag, unsigned int mask)
				{
					return bitflag & mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if any bit is set based on the bitmask

				static bool IsAnyBitSet( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask ) != 0;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bits are 1 based on the bitmask

				static bool AreAllBitsSet( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask) == mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bit are 0 based on the bitmask

				static bool AreAllBitsClear( unsigned int &bitflag, unsigned int mask )
				{
					return ( bitflag & mask ) == 0;
				}

				
		}
	
	} // end oif namespace utils

} // end of vml namespace

