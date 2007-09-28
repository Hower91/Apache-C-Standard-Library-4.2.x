// checking for dynamic_cast

/***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  License); you may  not use  this file
 * except in  compliance with the License.   You may obtain  a copy of
 * the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the  License is distributed on an  "AS IS" BASIS,
 * WITHOUT  WARRANTIES OR CONDITIONS  OF ANY  KIND, either  express or
 * implied.   See  the License  for  the  specific language  governing
 * permissions and limitations under the License.
 *
 * Copyright 1999-2007 Rogue Wave Software, Inc.
 * 
 **************************************************************************/

#include "config.h"

#if (defined (_WIN32) || defined (_WIN64)) && !defined (__CYGWIN__)
#  include <crtdbg.h>
#endif   // _WIN{32,64}


#ifndef _RWSTD_NO_HONOR_STD
#  ifdef _RWSTD_NO_STD_TERMINATE

namespace std {

void terminate ()
{
    for (char *p = 0; ; ++p) {
        *p = 0;   // force a SIGSEGV
    }
}

}

#  endif   // _RWSTD_NO_STD_TERMINATE
#endif   // _RWSTD_NO_HONOR_STD


struct A
{
    virtual ~A () { }
};

struct B: A { };
struct C: A { };


int main ()
{
#if (defined (_WIN32) || defined (_WIN64)) && !defined (__CYGWIN__)

    // prevent the Microsoft C library from popping up GUI windows
    _CrtSetReportMode (_CRT_WARN, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode (_CRT_ERROR, _CRTDBG_MODE_DEBUG);
    _CrtSetReportMode (_CRT_ASSERT, _CRTDBG_MODE_DEBUG);

#endif   // _WIN{32,64}

    B b;

    A *a = &b;
    
    return !(dynamic_cast<B*>(a) && !dynamic_cast<C*>(a));
}
