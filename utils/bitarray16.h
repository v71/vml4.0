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
		// 16 bit set class , this class is to be used as
		// a kind of static class.

		namespace bits16
		{

				///////////////////////////////////////////////
				// this array block is a 9*256 raw memory block
				// which contains in the first position the
				// number of activer bits making up the 
				// corresponding value , the other 8 values
				// represent the corresponding active bits
				// example value = 8
				// memory block : 1, 0, 0, 0, 1, 0, 0, 0, 0
				// number of active bits is the first 
				// value in the memory block, which is 1
				// starting from the first value at the forth 
				// position we have a 1 which is 2 raised 
				// to the power of 3 , thus we have the value of 8 

				static const unsigned char PopData[] =
				{
					0, 0, 0, 0, 0, 0, 0, 0, 0,
					1, 1, 0, 0, 0, 0, 0, 0, 0,
					1, 0, 1, 0, 0, 0, 0, 0, 0,
					2, 1, 1, 0, 0, 0, 0, 0, 0,
					1, 0, 0, 1, 0, 0, 0, 0, 0,
					2, 1, 0, 1, 0, 0, 0, 0, 0,
					2, 0, 1, 1, 0, 0, 0, 0, 0,
					3, 1, 1, 1, 0, 0, 0, 0, 0,
					1, 0, 0, 0, 1, 0, 0, 0, 0,
					2, 1, 0, 0, 1, 0, 0, 0, 0,
					2, 0, 1, 0, 1, 0, 0, 0, 0,
					3, 1, 1, 0, 1, 0, 0, 0, 0,
					2, 0, 0, 1, 1, 0, 0, 0, 0,
					3, 1, 0, 1, 1, 0, 0, 0, 0,
					3, 0, 1, 1, 1, 0, 0, 0, 0,
					4, 1, 1, 1, 1, 0, 0, 0, 0,
					1, 0, 0, 0, 0, 1, 0, 0, 0,
					2, 1, 0, 0, 0, 1, 0, 0, 0,
					2, 0, 1, 0, 0, 1, 0, 0, 0,
					3, 1, 1, 0, 0, 1, 0, 0, 0,
					2, 0, 0, 1, 0, 1, 0, 0, 0,
					3, 1, 0, 1, 0, 1, 0, 0, 0,
					3, 0, 1, 1, 0, 1, 0, 0, 0,
					4, 1, 1, 1, 0, 1, 0, 0, 0,
					2, 0, 0, 0, 1, 1, 0, 0, 0,
					3, 1, 0, 0, 1, 1, 0, 0, 0,
					3, 0, 1, 0, 1, 1, 0, 0, 0,
					4, 1, 1, 0, 1, 1, 0, 0, 0,
					3, 0, 0, 1, 1, 1, 0, 0, 0,
					4, 1, 0, 1, 1, 1, 0, 0, 0,
					4, 0, 1, 1, 1, 1, 0, 0, 0,
					5, 1, 1, 1, 1, 1, 0, 0, 0,
					1, 0, 0, 0, 0, 0, 1, 0, 0,
					2, 1, 0, 0, 0, 0, 1, 0, 0,
					2, 0, 1, 0, 0, 0, 1, 0, 0,
					3, 1, 1, 0, 0, 0, 1, 0, 0,
					2, 0, 0, 1, 0, 0, 1, 0, 0,
					3, 1, 0, 1, 0, 0, 1, 0, 0,
					3, 0, 1, 1, 0, 0, 1, 0, 0,
					4, 1, 1, 1, 0, 0, 1, 0, 0,
					2, 0, 0, 0, 1, 0, 1, 0, 0,
					3, 1, 0, 0, 1, 0, 1, 0, 0,
					3, 0, 1, 0, 1, 0, 1, 0, 0,
					4, 1, 1, 0, 1, 0, 1, 0, 0,
					3, 0, 0, 1, 1, 0, 1, 0, 0,
					4, 1, 0, 1, 1, 0, 1, 0, 0,
					4, 0, 1, 1, 1, 0, 1, 0, 0,
					5, 1, 1, 1, 1, 0, 1, 0, 0,
					2, 0, 0, 0, 0, 1, 1, 0, 0,
					3, 1, 0, 0, 0, 1, 1, 0, 0,
					3, 0, 1, 0, 0, 1, 1, 0, 0,
					4, 1, 1, 0, 0, 1, 1, 0, 0,
					3, 0, 0, 1, 0, 1, 1, 0, 0,
					4, 1, 0, 1, 0, 1, 1, 0, 0,
					4, 0, 1, 1, 0, 1, 1, 0, 0,
					5, 1, 1, 1, 0, 1, 1, 0, 0,
					3, 0, 0, 0, 1, 1, 1, 0, 0,
					4, 1, 0, 0, 1, 1, 1, 0, 0,
					4, 0, 1, 0, 1, 1, 1, 0, 0,
					5, 1, 1, 0, 1, 1, 1, 0, 0,
					4, 0, 0, 1, 1, 1, 1, 0, 0,
					5, 1, 0, 1, 1, 1, 1, 0, 0,
					5, 0, 1, 1, 1, 1, 1, 0, 0,
					6, 1, 1, 1, 1, 1, 1, 0, 0,
					1, 0, 0, 0, 0, 0, 0, 1, 0,
					2, 1, 0, 0, 0, 0, 0, 1, 0,
					2, 0, 1, 0, 0, 0, 0, 1, 0,
					3, 1, 1, 0, 0, 0, 0, 1, 0,
					2, 0, 0, 1, 0, 0, 0, 1, 0,
					3, 1, 0, 1, 0, 0, 0, 1, 0,
					3, 0, 1, 1, 0, 0, 0, 1, 0,
					4, 1, 1, 1, 0, 0, 0, 1, 0,
					2, 0, 0, 0, 1, 0, 0, 1, 0,
					3, 1, 0, 0, 1, 0, 0, 1, 0,
					3, 0, 1, 0, 1, 0, 0, 1, 0,
					4, 1, 1, 0, 1, 0, 0, 1, 0,
					3, 0, 0, 1, 1, 0, 0, 1, 0,
					4, 1, 0, 1, 1, 0, 0, 1, 0,
					4, 0, 1, 1, 1, 0, 0, 1, 0,
					5, 1, 1, 1, 1, 0, 0, 1, 0,
					2, 0, 0, 0, 0, 1, 0, 1, 0,
					3, 1, 0, 0, 0, 1, 0, 1, 0,
					3, 0, 1, 0, 0, 1, 0, 1, 0,
					4, 1, 1, 0, 0, 1, 0, 1, 0,
					3, 0, 0, 1, 0, 1, 0, 1, 0,
					4, 1, 0, 1, 0, 1, 0, 1, 0,
					4, 0, 1, 1, 0, 1, 0, 1, 0,
					5, 1, 1, 1, 0, 1, 0, 1, 0,
					3, 0, 0, 0, 1, 1, 0, 1, 0,
					4, 1, 0, 0, 1, 1, 0, 1, 0,
					4, 0, 1, 0, 1, 1, 0, 1, 0,
					5, 1, 1, 0, 1, 1, 0, 1, 0,
					4, 0, 0, 1, 1, 1, 0, 1, 0,
					5, 1, 0, 1, 1, 1, 0, 1, 0,
					5, 0, 1, 1, 1, 1, 0, 1, 0,
					6, 1, 1, 1, 1, 1, 0, 1, 0,
					2, 0, 0, 0, 0, 0, 1, 1, 0,
					3, 1, 0, 0, 0, 0, 1, 1, 0,
					3, 0, 1, 0, 0, 0, 1, 1, 0,
					4, 1, 1, 0, 0, 0, 1, 1, 0,
					3, 0, 0, 1, 0, 0, 1, 1, 0,
					4, 1, 0, 1, 0, 0, 1, 1, 0,
					4, 0, 1, 1, 0, 0, 1, 1, 0,
					5, 1, 1, 1, 0, 0, 1, 1, 0,
					3, 0, 0, 0, 1, 0, 1, 1, 0,
					4, 1, 0, 0, 1, 0, 1, 1, 0,
					4, 0, 1, 0, 1, 0, 1, 1, 0,
					5, 1, 1, 0, 1, 0, 1, 1, 0,
					4, 0, 0, 1, 1, 0, 1, 1, 0,
					5, 1, 0, 1, 1, 0, 1, 1, 0,
					5, 0, 1, 1, 1, 0, 1, 1, 0,
					6, 1, 1, 1, 1, 0, 1, 1, 0,
					3, 0, 0, 0, 0, 1, 1, 1, 0,
					4, 1, 0, 0, 0, 1, 1, 1, 0,
					4, 0, 1, 0, 0, 1, 1, 1, 0,
					5, 1, 1, 0, 0, 1, 1, 1, 0,
					4, 0, 0, 1, 0, 1, 1, 1, 0,
					5, 1, 0, 1, 0, 1, 1, 1, 0,
					5, 0, 1, 1, 0, 1, 1, 1, 0,
					6, 1, 1, 1, 0, 1, 1, 1, 0,
					4, 0, 0, 0, 1, 1, 1, 1, 0,
					5, 1, 0, 0, 1, 1, 1, 1, 0,
					5, 0, 1, 0, 1, 1, 1, 1, 0,
					6, 1, 1, 0, 1, 1, 1, 1, 0,
					5, 0, 0, 1, 1, 1, 1, 1, 0,
					6, 1, 0, 1, 1, 1, 1, 1, 0,
					6, 0, 1, 1, 1, 1, 1, 1, 0,
					7, 1, 1, 1, 1, 1, 1, 1, 0,
					1, 0, 0, 0, 0, 0, 0, 0, 1,
					2, 1, 0, 0, 0, 0, 0, 0, 1,
					2, 0, 1, 0, 0, 0, 0, 0, 1,
					3, 1, 1, 0, 0, 0, 0, 0, 1,
					2, 0, 0, 1, 0, 0, 0, 0, 1,
					3, 1, 0, 1, 0, 0, 0, 0, 1,
					3, 0, 1, 1, 0, 0, 0, 0, 1,
					4, 1, 1, 1, 0, 0, 0, 0, 1,
					2, 0, 0, 0, 1, 0, 0, 0, 1,
					3, 1, 0, 0, 1, 0, 0, 0, 1,
					3, 0, 1, 0, 1, 0, 0, 0, 1,
					4, 1, 1, 0, 1, 0, 0, 0, 1,
					3, 0, 0, 1, 1, 0, 0, 0, 1,
					4, 1, 0, 1, 1, 0, 0, 0, 1,
					4, 0, 1, 1, 1, 0, 0, 0, 1,
					5, 1, 1, 1, 1, 0, 0, 0, 1,
					2, 0, 0, 0, 0, 1, 0, 0, 1,
					3, 1, 0, 0, 0, 1, 0, 0, 1,
					3, 0, 1, 0, 0, 1, 0, 0, 1,
					4, 1, 1, 0, 0, 1, 0, 0, 1,
					3, 0, 0, 1, 0, 1, 0, 0, 1,
					4, 1, 0, 1, 0, 1, 0, 0, 1,
					4, 0, 1, 1, 0, 1, 0, 0, 1,
					5, 1, 1, 1, 0, 1, 0, 0, 1,
					3, 0, 0, 0, 1, 1, 0, 0, 1,
					4, 1, 0, 0, 1, 1, 0, 0, 1,
					4, 0, 1, 0, 1, 1, 0, 0, 1,
					5, 1, 1, 0, 1, 1, 0, 0, 1,
					4, 0, 0, 1, 1, 1, 0, 0, 1,
					5, 1, 0, 1, 1, 1, 0, 0, 1,
					5, 0, 1, 1, 1, 1, 0, 0, 1,
					6, 1, 1, 1, 1, 1, 0, 0, 1,
					2, 0, 0, 0, 0, 0, 1, 0, 1,
					3, 1, 0, 0, 0, 0, 1, 0, 1,
					3, 0, 1, 0, 0, 0, 1, 0, 1,
					4, 1, 1, 0, 0, 0, 1, 0, 1,
					3, 0, 0, 1, 0, 0, 1, 0, 1,
					4, 1, 0, 1, 0, 0, 1, 0, 1,
					4, 0, 1, 1, 0, 0, 1, 0, 1,
					5, 1, 1, 1, 0, 0, 1, 0, 1,
					3, 0, 0, 0, 1, 0, 1, 0, 1,
					4, 1, 0, 0, 1, 0, 1, 0, 1,
					4, 0, 1, 0, 1, 0, 1, 0, 1,
					5, 1, 1, 0, 1, 0, 1, 0, 1,
					4, 0, 0, 1, 1, 0, 1, 0, 1,
					5, 1, 0, 1, 1, 0, 1, 0, 1,
					5, 0, 1, 1, 1, 0, 1, 0, 1,
					6, 1, 1, 1, 1, 0, 1, 0, 1,
					3, 0, 0, 0, 0, 1, 1, 0, 1,
					4, 1, 0, 0, 0, 1, 1, 0, 1,
					4, 0, 1, 0, 0, 1, 1, 0, 1,
					5, 1, 1, 0, 0, 1, 1, 0, 1,
					4, 0, 0, 1, 0, 1, 1, 0, 1,
					5, 1, 0, 1, 0, 1, 1, 0, 1,
					5, 0, 1, 1, 0, 1, 1, 0, 1,
					6, 1, 1, 1, 0, 1, 1, 0, 1,
					4, 0, 0, 0, 1, 1, 1, 0, 1,
					5, 1, 0, 0, 1, 1, 1, 0, 1,
					5, 0, 1, 0, 1, 1, 1, 0, 1,
					6, 1, 1, 0, 1, 1, 1, 0, 1,
					5, 0, 0, 1, 1, 1, 1, 0, 1,
					6, 1, 0, 1, 1, 1, 1, 0, 1,
					6, 0, 1, 1, 1, 1, 1, 0, 1,
					7, 1, 1, 1, 1, 1, 1, 0, 1,
					2, 0, 0, 0, 0, 0, 0, 1, 1,
					3, 1, 0, 0, 0, 0, 0, 1, 1,
					3, 0, 1, 0, 0, 0, 0, 1, 1,
					4, 1, 1, 0, 0, 0, 0, 1, 1,
					3, 0, 0, 1, 0, 0, 0, 1, 1,
					4, 1, 0, 1, 0, 0, 0, 1, 1,
					4, 0, 1, 1, 0, 0, 0, 1, 1,
					5, 1, 1, 1, 0, 0, 0, 1, 1,
					3, 0, 0, 0, 1, 0, 0, 1, 1,
					4, 1, 0, 0, 1, 0, 0, 1, 1,
					4, 0, 1, 0, 1, 0, 0, 1, 1,
					5, 1, 1, 0, 1, 0, 0, 1, 1,
					4, 0, 0, 1, 1, 0, 0, 1, 1,
					5, 1, 0, 1, 1, 0, 0, 1, 1,
					5, 0, 1, 1, 1, 0, 0, 1, 1,
					6, 1, 1, 1, 1, 0, 0, 1, 1,
					3, 0, 0, 0, 0, 1, 0, 1, 1,
					4, 1, 0, 0, 0, 1, 0, 1, 1,
					4, 0, 1, 0, 0, 1, 0, 1, 1,
					5, 1, 1, 0, 0, 1, 0, 1, 1,
					4, 0, 0, 1, 0, 1, 0, 1, 1,
					5, 1, 0, 1, 0, 1, 0, 1, 1,
					5, 0, 1, 1, 0, 1, 0, 1, 1,
					6, 1, 1, 1, 0, 1, 0, 1, 1,
					4, 0, 0, 0, 1, 1, 0, 1, 1,
					5, 1, 0, 0, 1, 1, 0, 1, 1,
					5, 0, 1, 0, 1, 1, 0, 1, 1,
					6, 1, 1, 0, 1, 1, 0, 1, 1,
					5, 0, 0, 1, 1, 1, 0, 1, 1,
					6, 1, 0, 1, 1, 1, 0, 1, 1,
					6, 0, 1, 1, 1, 1, 0, 1, 1,
					7, 1, 1, 1, 1, 1, 0, 1, 1,
					3, 0, 0, 0, 0, 0, 1, 1, 1,
					4, 1, 0, 0, 0, 0, 1, 1, 1,
					4, 0, 1, 0, 0, 0, 1, 1, 1,
					5, 1, 1, 0, 0, 0, 1, 1, 1,
					4, 0, 0, 1, 0, 0, 1, 1, 1,
					5, 1, 0, 1, 0, 0, 1, 1, 1,
					5, 0, 1, 1, 0, 0, 1, 1, 1,
					6, 1, 1, 1, 0, 0, 1, 1, 1,
					4, 0, 0, 0, 1, 0, 1, 1, 1,
					5, 1, 0, 0, 1, 0, 1, 1, 1,
					5, 0, 1, 0, 1, 0, 1, 1, 1,
					6, 1, 1, 0, 1, 0, 1, 1, 1,
					5, 0, 0, 1, 1, 0, 1, 1, 1,
					6, 1, 0, 1, 1, 0, 1, 1, 1,
					6, 0, 1, 1, 1, 0, 1, 1, 1,
					7, 1, 1, 1, 1, 0, 1, 1, 1,
					4, 0, 0, 0, 0, 1, 1, 1, 1,
					5, 1, 0, 0, 0, 1, 1, 1, 1,
					5, 0, 1, 0, 0, 1, 1, 1, 1,
					6, 1, 1, 0, 0, 1, 1, 1, 1,
					5, 0, 0, 1, 0, 1, 1, 1, 1,
					6, 1, 0, 1, 0, 1, 1, 1, 1,
					6, 0, 1, 1, 0, 1, 1, 1, 1,
					7, 1, 1, 1, 0, 1, 1, 1, 1,
					5, 0, 0, 0, 1, 1, 1, 1, 1,
					6, 1, 0, 0, 1, 1, 1, 1, 1,
					6, 0, 1, 0, 1, 1, 1, 1, 1,
					7, 1, 1, 0, 1, 1, 1, 1, 1,
					6, 0, 0, 1, 1, 1, 1, 1, 1,
					7, 1, 0, 1, 1, 1, 1, 1, 1,
					7, 0, 1, 1, 1, 1, 1, 1, 1,
					8, 1, 1, 1, 1, 1, 1, 1, 1,
				};

				//////////////////////////////////////////////////////
				// bits positions

				const static unsigned short int BIT0  =     1;
				const static unsigned short int BIT1  =     2;
				const static unsigned short int BIT2  =     4;
				const static unsigned short int BIT3  =     8;
				const static unsigned short int BIT4  =    16;
				const static unsigned short int BIT5  =    32;
				const static unsigned short int BIT6  =    64;
				const static unsigned short int BIT7  =   128;
				const static unsigned short int BIT8  =   256;
				const static unsigned short int BIT9  =   512;
				const static unsigned short int BIT10 =  1024;
				const static unsigned short int BIT11 =  2048;
				const static unsigned short int BIT12 =  4096;
				const static unsigned short int BIT13 =  8192;
				const static unsigned short int BIT14 = 16384;
				const static unsigned short int BIT15 = 32768;
			
				/////////////////////////////////////////////////////////////////
				// clear all bit flags

				static void Clear(unsigned short int &bitflag)
				{
					bitflag=0;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to true

				static void SetToTrue(unsigned short int &bitflag, unsigned short int bit)
				{
					bitflag |=  bit  ;
				}

				/////////////////////////////////////////////////////////////////
				// sets a single bit to false

				static void SetToFalse(unsigned short int &bitflag, unsigned short int bit)
				{
					bitflag &= ~ bit ;
				}

				/////////////////////////////////////////////////////////////////
				// xor a single bit

				static void Toggle(unsigned short int &bitflag, unsigned short int bit)
				{
					bitflag ^= bit;
				}

				/////////////////////////////////////////////////////////////////
				// gets value of single bit

				static bool Get(const unsigned short int &bitflag, unsigned short int bit)
				{
					return ( ( bitflag & bit )!=0 );
				}

				/////////////////////////////////////////////////////////////////
				// sets value of single bit

				static void SetMask(unsigned short int &bitflag, unsigned short int mask)
				{
					bitflag=mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Bit functions based on bitmask

				static unsigned short int SetBitsExcept(unsigned short int &bitflag, unsigned short int mask)
				{
					return bitflag | ~mask;
				}

				static unsigned short int ClearBits(unsigned short int &bitflag, unsigned short int mask)
				{
					return bitflag & ~mask;
				}

				static unsigned short int ClearBitsExcept(unsigned short int &bitflag, unsigned short int mask)
				{
					return bitflag & mask;
				}

				static int PosFromBitValue(int bitvalue)
				{
					switch (bitvalue)
					{
						case BIT0  : return  0; break;
						case BIT1  : return  1; break;
						case BIT2  : return  2; break;
						case BIT3  : return  3; break;
						case BIT4  : return  4; break;
						case BIT5  : return  5; break;
						case BIT6  : return  6; break;
						case BIT7  : return  7; break;
						case BIT8  : return  8; break;
						case BIT9  : return  9; break;
						case BIT10 : return 10; break;
						case BIT11 : return 11; break;
						case BIT12 : return 12; break;
						case BIT13 : return 13; break;
						case BIT14 : return 14; break;
						case BIT15 : return 15; break;
					}
				
					return -1;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if any bit is set based on the bitmask

				static bool IsAnyBitSet( unsigned short int &bitflag, unsigned short int mask )
				{
					return ( bitflag & mask ) != 0;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bits are 1 based on the bitmask

				static bool AreAllBitsSet( unsigned short int &bitflag, unsigned short int mask )
				{
					return ( bitflag & mask) == mask;
				}

				/////////////////////////////////////////////////////////////////////////////
				// counts if all bit are 0 based on the bitmask

				static bool AreAllBitsClear( unsigned short int &bitflag, unsigned short int mask )
				{
					return ( bitflag & mask ) == 0;
				}

				/////////////////////////////////////////////////////////////////////////////
				/// Returns the number of 1's set in the given value.

				static unsigned short int PopCount(unsigned short int &bitflag)
				{
					unsigned short int bits = 0;
					if (bitflag & BIT0) bits++;
					if (bitflag & BIT1) bits++;
					if (bitflag & BIT2) bits++;
					if (bitflag & BIT3) bits++;
					if (bitflag & BIT4) bits++;
					if (bitflag & BIT5) bits++;
					if (bitflag & BIT6) bits++;
					if (bitflag & BIT7) bits++;
					if (bitflag & BIT8) bits++;
					if (bitflag & BIT9) bits++;
					if (bitflag & BIT10) bits++;
					if (bitflag & BIT11) bits++;
					if (bitflag & BIT12) bits++;
					if (bitflag & BIT13) bits++;
					if (bitflag & BIT14) bits++;
					if (bitflag & BIT15) bits++;
					return bits;
				}

				/////////////////////////////////////////////////////////////////////////////
				/// Returns the number of 0's set in the given value.

				static unsigned short int ZeroPopCount(unsigned short int &bitflag)
				{
					unsigned short int bits = 16;
					if (bitflag & BIT0) bits--;
					if (bitflag & BIT1) bits--;
					if (bitflag & BIT2) bits--;
					if (bitflag & BIT3) bits--;
					if (bitflag & BIT4) bits--;
					if (bitflag & BIT5) bits--;
					if (bitflag & BIT6) bits--;
					if (bitflag & BIT7) bits--;
					if (bitflag & BIT8) bits--;
					if (bitflag & BIT9) bits--;
					if (bitflag & BIT10) bits--;
					if (bitflag & BIT11) bits--;
					if (bitflag & BIT12) bits--;
					if (bitflag & BIT13) bits--;
					if (bitflag & BIT14) bits--;
					if (bitflag & BIT15) bits--;
					return bits;
				}

				/////////////////////////////////////////////////////////////////////////////
				// Clears the LSB bit and returns the zero-based index of where the bit was set.

				static int ExtractLSB(unsigned short int *bitflag)
				{
					if ((*bitflag & (1<< 0)) != 0) {*bitflag &= ~(1<< 0); return  0; }
					if ((*bitflag & (1<< 1)) != 0) {*bitflag &= ~(1<< 1); return  1; }
					if ((*bitflag & (1<< 2)) != 0) {*bitflag &= ~(1<< 2); return  2; }
					if ((*bitflag & (1<< 3)) != 0) {*bitflag &= ~(1<< 3); return  3; }
					if ((*bitflag & (1<< 4)) != 0) {*bitflag &= ~(1<< 4); return  4; }
					if ((*bitflag & (1<< 5)) != 0) {*bitflag &= ~(1<< 5); return  5; }
					if ((*bitflag & (1<< 6)) != 0) {*bitflag &= ~(1<< 6); return  6; }
					if ((*bitflag & (1<< 7)) != 0) {*bitflag &= ~(1<< 7); return  7; }
					if ((*bitflag & (1<< 8)) != 0) {*bitflag &= ~(1<< 8); return  8; }
					if ((*bitflag & (1<< 9)) != 0) {*bitflag &= ~(1<< 9); return  9; }
					if ((*bitflag & (1<<10)) != 0) {*bitflag &= ~(1<<10); return 10; }
					if ((*bitflag & (1<<11)) != 0) {*bitflag &= ~(1<<11); return 11; }
					if ((*bitflag & (1<<12)) != 0) {*bitflag &= ~(1<<12); return 12; }
					if ((*bitflag & (1<<13)) != 0) {*bitflag &= ~(1<<13); return 13; }
					if ((*bitflag & (1<<14)) != 0) {*bitflag &= ~(1<<14); return 14; }
					if ((*bitflag & (1<<15)) != 0) {*bitflag &= ~(1<<15); return 15; }
					return -1;
				}

				/////////////////////////////////////////////////////////////////
				// convert bitarray into a string 
			
				static unsigned short int ConvertFromString( const std::string &bitstring )
				{
					if ( bitstring.size()!=32 )	
						vml::os::Message::Error("bitarray16 : ", "size must be 32 characthers");
					unsigned short int flag = 0;
					if (bitstring[ 0] == '1') flag |= BIT0;
					if (bitstring[ 1] == '1') flag |= BIT1;
					if (bitstring[ 2] == '1') flag |= BIT2;
					if (bitstring[ 3] == '1') flag |= BIT3;
					if (bitstring[ 4] == '1') flag |= BIT4;
					if (bitstring[ 5] == '1') flag |= BIT5;
					if (bitstring[ 6] == '1') flag |= BIT6;
					if (bitstring[ 7] == '1') flag |= BIT7;
					if (bitstring[ 8] == '1') flag |= BIT8;
					if (bitstring[ 9] == '1') flag |= BIT9;
					if (bitstring[10] == '1') flag |= BIT10;
					if (bitstring[11] == '1') flag |= BIT11;
					if (bitstring[12] == '1') flag |= BIT12;
					if (bitstring[13] == '1') flag |= BIT13;
					if (bitstring[14] == '1') flag |= BIT14;
					if (bitstring[15] == '1') flag |= BIT15;
					return flag;
				}
				
				/////////////////////////////////////////////////////////////////
				// dumps bit contents into a string

				static std::string ConvertToString(const unsigned short int &bitflag)
				{
					std::string bitstring = "00000000000000000000000000000000";
					if (bitflag & BIT0)  bitstring[ 0] = '1'; else bitstring[ 0] = '0';
					if (bitflag & BIT1)  bitstring[ 1] = '1'; else bitstring[ 1] = '0';
					if (bitflag & BIT2)  bitstring[ 2] = '1'; else bitstring[ 2] = '0';
					if (bitflag & BIT3)  bitstring[ 3] = '1'; else bitstring[ 3] = '0';
					if (bitflag & BIT4)  bitstring[ 4] = '1'; else bitstring[ 4] = '0';
					if (bitflag & BIT5)  bitstring[ 5] = '1'; else bitstring[ 5] = '0';
					if (bitflag & BIT6)  bitstring[ 6] = '1'; else bitstring[ 6] = '0';
					if (bitflag & BIT7)  bitstring[ 7] = '1'; else bitstring[ 7] = '0';
					if (bitflag & BIT8)  bitstring[ 8] = '1'; else bitstring[ 8] = '0';
					if (bitflag & BIT9)  bitstring[ 9] = '1'; else bitstring[ 9] = '0';
					if (bitflag & BIT10) bitstring[10] = '1'; else bitstring[10] = '0';
					if (bitflag & BIT11) bitstring[11] = '1'; else bitstring[11] = '0';
					if (bitflag & BIT12) bitstring[12] = '1'; else bitstring[12] = '0';
					if (bitflag & BIT13) bitstring[13] = '1'; else bitstring[13] = '0';
					if (bitflag & BIT14) bitstring[14] = '1'; else bitstring[14] = '0';
					if (bitflag & BIT15) bitstring[15] = '1'; else bitstring[15] = '0';
					return bitstring;
				}

				//////////////////////////////////////////////////////////////////////
				// counts bits population using the memory data block
				// the offset is computed as :
				// bitflag*9 = bitflag*8+bitflag = (bitflag <<3 ) +bitflag 
				// unsigned *v is an 16 bit array
				// bits contains the active bits ( bits set to 1 )
				// the 16 bit unsigned int is decomposed into 2 bytes

				static void PopList(unsigned short int bitflag, unsigned char *v, unsigned int &bits)
				{
					size_t b1, b2;
					size_t offset;

					// split unsigned int into first byte

					b1 = bitflag & 0xFF000000UL;
					b2 = bitflag >> 8;

					offset = (b1 << 3) + b1;
					bits = PopData[offset];

					v[ 0] = PopData[offset + 1];
					v[ 1] = PopData[offset + 2];
					v[ 2] = PopData[offset + 3];
					v[ 3] = PopData[offset + 4];
					v[ 4] = PopData[offset + 5];
					v[ 5] = PopData[offset + 6];
					v[ 6] = PopData[offset + 7];
					v[ 7] = PopData[offset + 8];

					// split unsigned int into second byte

					offset = (b2 << 3) + b2;
					bits += PopData[offset];

					v[ 8] = PopData[offset + 1];
					v[ 9] = PopData[offset + 2];
					v[10] = PopData[offset + 3];
					v[11] = PopData[offset + 4];
					v[12] = PopData[offset + 5];
					v[13] = PopData[offset + 6];
					v[14] = PopData[offset + 7];
					v[15] = PopData[offset + 8];
				}

		};
		
	} // end of utils namespace

} // end of vml namespace

