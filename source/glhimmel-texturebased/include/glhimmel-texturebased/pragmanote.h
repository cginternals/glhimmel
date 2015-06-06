
#pragma once
#ifndef __PRAGMANOTE_H__
#define __PRAGMANOTE_H__

// Macros to Generate Compile Time Messages - modified from sources:
// http://www.codeguru.com/cpp/misc/misc/compilerandpre-compiler/article.php/c14797)
// http://stackoverflow.com/questions/471935/user-warnings-on-msvc-and-gcc

#define STRINGISE_IMPL(x) #x
#define STRINGISE(x) STRINGISE_IMPL(x)

// Use: #pragma NOTE("...")
#if _MSC_VER
#   define FILE_LINE_LINK __FILE__ "(" STRINGISE(__LINE__) ") : "
#   define NOTE(exp) message (FILE_LINE_LINK "note: " exp)
#    define NOTE_wARG(exp, arg)  message (FILE_LINE_LINK "note: " exp STRINGISE(arg))
#else //__GNUC__ - may need other defines for different compilers
#   define NOTE(exp) ("note: " exp)
#endif

#endif // __PRAGMANOTE_H__