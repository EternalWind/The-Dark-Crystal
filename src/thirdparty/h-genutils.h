#ifndef PROG_FILE_HGENUTILS 
#define PROG_FILE_HGENUTILS 
  
#include "windows.h" 
  
namespace hGenUtils { 
    // convert a char* to a wchar* 
    WCHAR* convertCStringToWString(const char* string); 
} 
  
#endif // PROG_FILE_HGENUTILS