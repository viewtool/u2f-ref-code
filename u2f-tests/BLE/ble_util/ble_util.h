/*
 *   Copyright (C) 2016, VASCO Data Security Int.
 *   Author: Johan.Verrept@vasco.com
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#include <string>
#include <iostream>

#include "u2f.h"
#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif

#define CHECK_INFO __FUNCTION__ << "[" << __LINE__ << "]:"

#ifdef PLATFORM_WINDOWS
#	define REDSTART	 (arg_ansi ? "\x1b[31m" : "")
#	define COLOREND  (arg_ansi ? "\x1b[0m"  : "")
#	define GREENSTART (arg_ansi ? "\x1b[32m" : "")
#else
#	define REDSTART	"\x1b[31m"
#	define COLOREND	"\x1b[0m"
#	define GREENSTART "\x1b[32m"
#endif

#define CHECK_EQ(a,b) { if ((a)!=(b)) { std::cerr << REDSTART << "CHECK_EQ fail at " << CHECK_INFO#a << " != "#b << ":" << COLOREND << std::endl; AbortOrNot(); }}
#define CHECK_NE(a,b) { if ((a)==(b)) { std::cerr << REDSTART << "CHECK_NE fail at " << CHECK_INFO#a << " == "#b << ":" << COLOREND << std::endl; AbortOrNot(); }}
#define CHECK_GE(a,b) { if ((a)<(b))  { std::cerr << REDSTART << "CHECK_GE fail at " << CHECK_INFO#a << " < " #b << ":" << COLOREND << std::endl; AbortOrNot(); }}
#define CHECK_GT(a,b) { if ((a)<=(b)) { std::cerr << REDSTART << "CHECK_GT fail at " << CHECK_INFO#a << " < " #b << ":" << COLOREND << std::endl; AbortOrNot(); }}
#define CHECK_LT(a,b) { if ((a)>=(b)) { std::cerr << REDSTART << "CHECK_LT fail at " << CHECK_INFO#a << " >= "#b << ":" << COLOREND << std::endl; AbortOrNot(); }}
#define CHECK_LE(a,b) { if ((a)>(b))  { std::cerr << REDSTART << "CHECK_LE fail at " << CHECK_INFO#a << " > " #b << ":" << COLOREND << std::endl; AbortOrNot(); }}

#define PASS(x) { (x); std::cout << GREENSTART << "PASS("#x")" << COLOREND << std::endl; }

#define WARN_EQ(a,b) { if ((a)!=(b)) { std::cerr << REDSTART << "WARN_NE fail at " << CHECK_INFO#a << " != "#b << ":" << COLOREND << std::endl; if (arg_LethalWarn) AbortOrNot(); }}

class U2F_info {
 public:
	U2F_info(const char *func, int line) {
		std::cout << " " << func << "[" << line << "]";
	} ~U2F_info() {
		std::cout << std::endl;
	}
	std::ostream & operator<<(const char *s) {
		std::cout << s;
		return std::cout;
	}
};

extern bool arg_Abort;
extern bool arg_Pause;
extern int arg_Verbose;
extern bool arg_LethalWarn;
extern bool arg_ansi;

#define INFO if (arg_Verbose) U2F_info(__FUNCTION__, __LINE__) << ": "

std::string bytes2ascii(const char *ptr, int len);
std::string bytes2ascii(const std::string & s);

uint16_t bytes2short(const unsigned char *buffer, uint32_t offset);

void AbortOrNot();

bool getCertificate(const U2F_REGISTER_RESP & rsp, std::string * cert);
bool getSignature(const U2F_REGISTER_RESP & rsp, int certsize,
		  std::string * sig);
bool getSubjectPublicKey(const std::string & cert, std::string * pk);
