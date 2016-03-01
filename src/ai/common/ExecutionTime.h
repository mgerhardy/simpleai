/*
The MIT License (MIT)

Copyright (c) 2015 Hideaki Suzuki

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <string>

#ifdef NO_TIME_ELAPSED_MEASUREMENT
#  define	TIME_ELAPSED_N(n,...)					time_elapsed_impl_noop<n>([&]{__VA_ARGS__;})
#  define	TIME_ELAPSED_EXP_N(n,...)				time_elapsed_impl_noop<n>([&]{__VA_ARGS__;})
#  define	TIME_ELAPSED_MARKER_N(n,marker,...)		time_elapsed_impl_noop<n>([&]{__VA_ARGS__;})
#else
#  define	TIME_ELAPSED_N(n,...)					time_elapsed_impl1<n>([&]{__VA_ARGS__;},__FILE__,__LINE__)
#  define	TIME_ELAPSED_EXP_N(n,...)				time_elapsed_impl2<n>([&]{__VA_ARGS__;},#__VA_ARGS__)
#  define	TIME_ELAPSED_MARKER_N(n,marker,...)		time_elapsed_impl_core<n>([&]{__VA_ARGS__;},marker)
#endif

#define	TIME_ELAPSED(...)							TIME_ELAPSED_N(1,__VA_ARGS__)
#define	TIME_ELAPSED_EXP(...)						TIME_ELAPSED_EXP_N(1,__VA_ARGS__)
#define	TIME_ELAPSED_MARKER(marker,...)				TIME_ELAPSED_MARKER_N(1,marker,__VA_ARGS__)

#define	TIME_ELAPSED_HEADER							"ElapsedTime: "
#define	TIME_ELAPSED_HEADER_WIDTH					48

template<const int count, typename BlockBody>
inline void time_elapsed_impl_core(BlockBody body, const std::string& header, std::ostream &o = std::cout) {
	using namespace std;
	using namespace std::chrono;

	auto t0 = high_resolution_clock::now();

	for (int i = 0; i < count; i++)
		body();

	auto t1 = high_resolution_clock::now();

	// Now, print out the elapsed time with its header
	auto prev_flags = o.flags();
	auto prev_precision = o.precision();
	auto prev_width = o.width();

	o << left << setw(TIME_ELAPSED_HEADER_WIDTH) << (TIME_ELAPSED_HEADER + header);
	o << "cnt:" << setw(8) << count;
	o << setw(12) << right << fixed << setprecision(3) << (float) duration_cast<microseconds>(t1 - t0).count() / 1000 << " msec" << endl;

	o.width(prev_width);
	o.precision(prev_precision);
	o.flags(prev_flags);
}

template<const int count, typename BlockBody>
inline void time_elapsed_impl_noop(BlockBody body, std::ostream &o = std::cout) {
	for (int i = 0; i < count; i++)
		body();
}

template<int count, typename BlockBody>
inline void time_elapsed_impl1(BlockBody body, const char * const path, const int lno, std::ostream &o = std::cout) {
	std::string basename { path + std::string(path).find_last_of("\\/") + 1 };
	time_elapsed_impl_core<count>(body, basename + " at L." + std::to_string(lno), o);
}

template<int count, typename BlockBody>
inline void time_elapsed_impl2(BlockBody body, const char * const bodystr, std::ostream &o = std::cout) {
	std::string heading { std::string(bodystr).substr(0, TIME_ELAPSED_HEADER_WIDTH - sizeof(TIME_ELAPSED_HEADER) - 2) };
	time_elapsed_impl_core<count>(body, heading, o);
}
