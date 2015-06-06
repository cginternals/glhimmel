#pragma once

#ifdef _MSC_VER
#   define GLHIMMEL_TEXTUREBASED_API_EXPORT_DECLARATION __declspec(dllexport)
#   define GLHIMMEL_TEXTUREBASED_API_IMPORT_DECLARATION __declspec(dllimport)
#elif __GNUC__
#	define GLHIMMEL_TEXTUREBASED_API_EXPORT_DECLARATION __attribute__ ((visibility ("default")))
#	define GLHIMMEL_TEXTUREBASED_API_IMPORT_DECLARATION __attribute__ ((visibility ("default")))
#else
#   define GLHIMMEL_TEXTUREBASED_API_EXPORT_DECLARATION
#   define GLHIMMEL_TEXTUREBASED_API_IMPORT_DECLARATION
#endif 

#ifndef TEMPLATE_STATIC
#ifdef GLHIMMEL_TEXTUREBASED_EXPORTS
#   define GLHIMMEL_TEXTUREBASED_API GLHIMMEL_TEXTUREBASED_API_EXPORT_DECLARATION
#else
#   define GLHIMMEL_TEXTUREBASED_API GLHIMMEL_TEXTUREBASED_API_IMPORT_DECLARATION
#endif
#else
#   define GLHIMMEL_TEXTUREBASED_API
#endif

#ifdef N_DEBUG
#   define IF_DEBUG(statement)
#   define IF_NDEBUG(statement) statement
#else
#   define IF_DEBUG(statement) statement
#   define IF_NDEBUG(statement)
#endif // N_DEBUG

// http://stackoverflow.com/questions/18387640/how-to-deal-with-noexcept-in-visual-studio
#ifndef NOEXCEPT
#   ifdef _MSC_VER
#       define NOEXCEPT
#   else
#       define NOEXCEPT noexcept
#   endif
#endif
