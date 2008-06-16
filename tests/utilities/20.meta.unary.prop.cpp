// -*- C++ -*-
/***************************************************************************
 *
 * 20.meta.unary.prop.cpp - test exercising meta.unary.prop
 *
 * $Id$
 *
 ***************************************************************************
 *
 * Licensed to the Apache Software  Foundation (ASF) under one or more
 * contributor  license agreements.  See  the NOTICE  file distributed
 * with  this  work  for  additional information  regarding  copyright
 * ownership.   The ASF  licenses this  file to  you under  the Apache
 * License, Version  2.0 (the  "License"); you may  not use  this file
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
 * Copyright 1999-2008 Rogue Wave Software, Inc.
 *
 **************************************************************************/

#include <stddef.h>

#include <rw_driver.h>

// compile out all test code if extensions disabled
#ifndef _RWSTD_NO_EXT_CXX_0X

#include <type_traits>

/**************************************************************************/

struct struct_t {
    int i_; float f_;
};

class class_t {
    int i_; float f_;
};

union union_t {
    int i_; float f_;
};

union empty_union {
};

enum enum_t {
    E_a, E_b, E_c
};

// for is_empty
struct empty_t
{
};

struct non_empty_t
{
    char c;
};

// for is_polymorphic
struct polymorphic_t
{
    virtual ~polymorphic_t() { } // need not be defined
    virtual void vfunc ();
};

struct non_polymorphic_t
{
    void vfunc ();
};

// for is_abstract
struct abstract_t
{
    virtual ~abstract_t () { }
    virtual void vfunc () = 0;
};

template <class T>
struct member_t
{
    T val;
};

// used by is_polymorphic and is_abstract
template <class T>
struct public_derived_t : T
{
};

template <class T, class U>
struct public_derived_with_member : T
{
    U val;
};

template <class T>
struct protected_derived_t : protected T
{
};

template <class T>
struct private_derived_t : private T
{
};

struct access_controlled_t 
{
    // just to avoid warning without a ctor
    static access_controlled_t make (char, char);
private:
    char pc1;
protected:
    char pc2;
};

template <class T>
struct special_case_t : public_derived_t<T>
{
    T val;
};

template <class T>
struct virtual_derived_t : virtual T
{
};

struct no_trivial_ctor_t {
    no_trivial_ctor_t () { }
};

struct no_trivial_copy_t {
    no_trivial_copy_t (const no_trivial_copy_t&) { }
};

struct no_trivial_assign_t {
    no_trivial_assign_t&
    operator= (const no_trivial_assign_t&) {
        return *this;
    }
};

struct no_trivial_dtor_t {
    ~no_trivial_dtor_t () { }
};

struct throwing_assign_t
{
    throwing_assign_t (int v)
        : val_ (v) { }

    throwing_assign_t&
    operator= (const throwing_assign_t& rhs)
    {
        if (rhs.val_ != 0)
            throw empty_t ();
        return *this;
    }

    int val_;
};

struct throwing_ctor_t
{
    throwing_ctor_t (int v)
        : val_ (v)
    {
        if (val_)
            throw empty_t ();
    }

    int val_;
};

struct throwing_copy_t
{
    throwing_copy_t (int v)
        : val_ (v) { }

    throwing_copy_t(const throwing_copy_t& rhs)
        : val_ (rhs.val_)
    {
        if (rhs.val_ != 0)
            throw empty_t ();
    }

    int val_;
};


/**************************************************************************/

void test_trait (int line, bool value, bool expect,
                 const char* trait, const char* type)
{
    rw_assert (value == expect, 0, line,
               "%s<%s>::value is %b, expected %b",
               trait, type, value, expect);
}

void test_trait (int line,
                 const char* trait, const char* type,
                 size_t value, size_t expect)
{
    rw_assert (value == expect, 0, line,
               "%s<%s>::value is %zu, expected %zu",
               trait, type, value, expect);
}

void test_trait (int line, size_t depth,
                 size_t value, size_t expect,
                 const char* trait, const char* type)
{
    rw_assert (value == expect, 0, line,
               "%s<%s,%zu>::value is %zu, expected %zu",
               trait, type, depth,
               value, expect);
}

#define _TEST(Trait, Type, Expect)                                   \
    test_trait (__LINE__, Trait<Type>::value, Expect, #Trait, #Type)

#define TEST(Trait, Type, Expect)                                    \
    _TEST (Trait, Type, Expect)

/**************************************************************************/

#define C  const
#define V  volatile
#define CV const volatile

/**************************************************************************/

void test_is_const ()
{
    TEST (std::is_const,    void, false);
    TEST (std::is_const, C  void, true);
    TEST (std::is_const,  V void, false);
    TEST (std::is_const, CV void, true);


    TEST (std::is_const,    int, false);
    TEST (std::is_const, C  int, true);
    TEST (std::is_const,  V int, false);
    TEST (std::is_const, CV int, true);

    // pointer types
    TEST (std::is_const,    int*, false);
    TEST (std::is_const, C  int*, false);
    TEST (std::is_const,  V int*, false);
    TEST (std::is_const, CV int*, false);

    TEST (std::is_const,    int* C, true);
    TEST (std::is_const, C  int* C, true);
    TEST (std::is_const,  V int* C, true);
    TEST (std::is_const, CV int* C, true);

    TEST (std::is_const,    int*  V, false);
    TEST (std::is_const, C  int*  V, false);
    TEST (std::is_const,  V int*  V, false);
    TEST (std::is_const, CV int*  V, false);

    TEST (std::is_const,    int* CV, true);
    TEST (std::is_const, C  int* CV, true);
    TEST (std::is_const,  V int* CV, true);
    TEST (std::is_const, CV int* CV, true);

    // reference types
    TEST (std::is_const,    int&, false);
    TEST (std::is_const, C  int&, false);
    TEST (std::is_const,  V int&, false);
    TEST (std::is_const, CV int&, false);

    // array types
    TEST (std::is_const,    int [2], false);
    TEST (std::is_const, C  int [2], true);
    TEST (std::is_const,  V int [2], false);
    TEST (std::is_const, CV int [2], true);
}

void test_is_volatile ()
{
    TEST (std::is_volatile,    void, false);
    TEST (std::is_volatile, C  void, false);
    TEST (std::is_volatile,  V void, true);
    TEST (std::is_volatile, CV void, true);

    TEST (std::is_volatile,    int, false);
    TEST (std::is_volatile, C  int, false);
    TEST (std::is_volatile,  V int, true);
    TEST (std::is_volatile, CV int, true);

    // pointer types
    TEST (std::is_volatile,    int*, false);
    TEST (std::is_volatile, C  int*, false);
    TEST (std::is_volatile,  V int*, false);
    TEST (std::is_volatile, CV int*, false);

    TEST (std::is_volatile,    int* C, false);
    TEST (std::is_volatile, C  int* C, false);
    TEST (std::is_volatile,  V int* C, false);
    TEST (std::is_volatile, CV int* C, false);

    TEST (std::is_volatile,    int*  V, true);
    TEST (std::is_volatile, C  int*  V, true);
    TEST (std::is_volatile,  V int*  V, true);
    TEST (std::is_volatile, CV int*  V, true);

    TEST (std::is_volatile,    int* CV, true);
    TEST (std::is_volatile, C  int* CV, true);
    TEST (std::is_volatile,  V int* CV, true);
    TEST (std::is_volatile, CV int* CV, true);

    // reference types
    TEST (std::is_volatile,    int&, false);
    TEST (std::is_volatile, C  int&, false);
    TEST (std::is_volatile,  V int&, false);
    TEST (std::is_volatile, CV int&, false);

    // array types
    TEST (std::is_volatile,    int [2], false);
    TEST (std::is_volatile, C  int [2], false);
    TEST (std::is_volatile,  V int [2], true);
    TEST (std::is_volatile, CV int [2], true);
}

static void test_has_trivial_assign ()
{
    TEST (std::has_trivial_assign, long, true);
    TEST (std::has_trivial_assign, C long, false);
    TEST (std::has_trivial_assign, V long, true);
    TEST (std::has_trivial_assign, CV long, false);

    TEST (std::has_trivial_assign, long&, false);
    TEST (std::has_trivial_assign, C long&, false);
    TEST (std::has_trivial_assign, V long&, false);
    TEST (std::has_trivial_assign, CV long&, false);

    TEST (std::has_trivial_assign, long[2], true);

    TEST (std::has_trivial_assign, struct_t, true);

    TEST (std::has_trivial_assign, no_trivial_assign_t, false);
    TEST (std::has_trivial_assign, public_derived_t<no_trivial_assign_t>, false);
}

static void test_has_nothrow_assign ()
{
    TEST (std::has_nothrow_assign, long, true);
    TEST (std::has_nothrow_assign, C long, false);
    TEST (std::has_nothrow_assign, V long, true);
    TEST (std::has_nothrow_assign, CV long, false);

    TEST (std::has_nothrow_assign, long&, false);
    TEST (std::has_nothrow_assign, C long&, false);
    TEST (std::has_nothrow_assign, V long&, false);
    TEST (std::has_nothrow_assign, CV long&, false);

    TEST (std::has_nothrow_assign, long[2], true);

    TEST (std::has_nothrow_assign, struct_t, true);
    TEST (std::has_nothrow_assign, no_trivial_assign_t, true);
    TEST (std::has_nothrow_assign, throwing_assign_t, false);
}

static void test_is_trivial ()
{
    TEST (std::is_trivial, long, true);
    TEST (std::is_trivial, C long, true);
    TEST (std::is_trivial, V long, true);
    TEST (std::is_trivial, CV long, true);

    TEST (std::is_trivial, long&, false);
    TEST (std::is_trivial, C long&, false);
    TEST (std::is_trivial, V long&, false);
    TEST (std::is_trivial, CV long&, false);

    TEST (std::is_trivial, class_t, true);
    TEST (std::is_trivial, struct_t, true);
    TEST (std::is_trivial, union_t, true);

    TEST (std::is_trivial, empty_t, true);
    TEST (std::is_trivial, public_derived_t<empty_t>, true);

    TEST (std::is_trivial, non_empty_t, true);
    TEST (std::is_trivial, public_derived_t<non_empty_t>, true);

    TEST (std::is_trivial, abstract_t, false);
    TEST (std::is_trivial, public_derived_t<abstract_t>, false);

    TEST (std::is_trivial, polymorphic_t, false);
    TEST (std::is_trivial, public_derived_t<polymorphic_t>, false);

    TEST (std::is_trivial, no_trivial_ctor_t, false);
    TEST (std::is_trivial, no_trivial_copy_t, false);
    TEST (std::is_trivial, no_trivial_assign_t, false);
    TEST (std::is_trivial, no_trivial_dtor_t, false);
}


// use traits to avoid warnings on gcc when adding qualifiers to some
// types
#undef TEST
#define TEST(Trait,Type,Expect)                                      \
    _TEST(Trait, Type, Expect);                                      \
    _TEST(Trait, std::add_const<Type>::type, Expect);                \
    _TEST(Trait, std::add_volatile<Type>::type, Expect);             \
    _TEST(Trait, std::add_cv<Type>::type, Expect)

static void test_is_standard_layout ()
{
    TEST (std::is_standard_layout, long, true);
    TEST (std::is_standard_layout, non_empty_t, true);

    // no non-static data members of non-standard-layout type
    TEST (std::is_standard_layout, member_t<access_controlled_t>, false);
    TEST (std::is_standard_layout, member_t<public_derived_t<polymorphic_t> >, false);

    // no reference member
    TEST (std::is_standard_layout, member_t<int&>, false);

    // no virtual function
    TEST (std::is_standard_layout, abstract_t, false);
    TEST (std::is_standard_layout, polymorphic_t, false);

    // no virtual base
    TEST (std::is_standard_layout, virtual_derived_t<empty_t>, false);
    TEST (std::is_standard_layout, virtual_derived_t<non_empty_t>, false);

    // no access control changes for non-statics
    TEST (std::is_standard_layout, access_controlled_t, false);

    // no non-standard layout base classes
    TEST (std::is_standard_layout, public_derived_t<member_t<access_controlled_t> >, false);
    TEST (std::is_standard_layout, public_derived_t<member_t<public_derived_t<polymorphic_t> > >, false);
    TEST (std::is_standard_layout, public_derived_t<member_t<int&> >, false);
    TEST (std::is_standard_layout, public_derived_t<abstract_t>, false);
    TEST (std::is_standard_layout, public_derived_t<polymorphic_t>, false);
    TEST (std::is_standard_layout, public_derived_t<virtual_derived_t<empty_t> >, false);
    TEST (std::is_standard_layout, public_derived_t<virtual_derived_t<non_empty_t> >, false);
    TEST (std::is_standard_layout, public_derived_t<access_controlled_t>, false);

    // no non-static data members in the most-derived class
    typedef public_derived_with_member<empty_t,int> derived_from_empty_with_member;
    TEST (std::is_standard_layout, derived_from_empty_with_member, false);

    // but we are allowed non-static members in one base class
    TEST (std::is_standard_layout, public_derived_t<non_empty_t>, true);
    TEST (std::is_standard_layout, public_derived_t<public_derived_t<non_empty_t> >, true);

    // but not two base classes with non-static members
    typedef public_derived_with_member<non_empty_t, int> derived_from_non_empty_with_member;
    TEST (std::is_standard_layout, derived_from_non_empty_with_member, false);

    // no base classes of the same type as the first non-static data member
    TEST (std::is_standard_layout, special_case_t<empty_t>, false);
    TEST (std::is_standard_layout, special_case_t<non_empty_t>, false);

    // everything else should be good to go
    TEST (std::is_standard_layout, member_t<int>, false);
    TEST (std::is_standard_layout, member_t<member_t<int> >, false);
}

static void test_is_pod ()
{
    TEST (std::is_pod, long, true);
    TEST (std::is_pod, long [], true);
    TEST (std::is_pod, long [2], true);
    TEST (std::is_pod, long (*)[2], true);

    TEST (std::is_pod, void*, true);
    TEST (std::is_pod, void (*)(), true);
    TEST (std::is_pod, void (*[])(), true);
    TEST (std::is_pod, void (class_t::*)(), true);

    // standard layout and trivial
    // 
    TEST (std::is_pod, struct_t, true);
    TEST (std::is_pod, union_t, true);
    TEST (std::is_pod, enum_t, true);

    TEST (std::is_pod, empty_t, true);
    TEST (std::is_pod, public_derived_t<empty_t>, true);

    TEST (std::is_pod, non_empty_t, true);
    TEST (std::is_pod, public_derived_t<non_empty_t>, true);

    TEST (std::is_pod, abstract_t, false);
    TEST (std::is_pod, public_derived_t<abstract_t>, false);

    TEST (std::is_pod, polymorphic_t, false);
    TEST (std::is_pod, public_derived_t<polymorphic_t>, false);

    TEST (std::is_pod, no_trivial_ctor_t, false);
    TEST (std::is_pod, no_trivial_copy_t, false);
    TEST (std::is_pod, no_trivial_assign_t, false);
    TEST (std::is_pod, no_trivial_dtor_t, false);

    TEST (std::is_pod, access_controlled_t, false);
}

static void test_is_empty ()
{
    TEST (std::is_empty, void, false);

    TEST (std::is_empty, struct_t, false);
    TEST (std::is_empty, class_t, false);
    TEST (std::is_empty, union_t, false);
    TEST (std::is_empty, enum_t, false);

    TEST (std::is_empty, empty_union, false);

    TEST (std::is_empty, empty_t, true);
    TEST (std::is_empty, public_derived_t<empty_t>, true);

    TEST (std::is_empty, non_empty_t, false);
    TEST (std::is_empty, public_derived_t<non_empty_t>, false);

    TEST (std::is_empty, abstract_t, false);
    TEST (std::is_empty, public_derived_t<abstract_t>, false);

    TEST (std::is_empty, polymorphic_t, false);
    TEST (std::is_empty, public_derived_t<polymorphic_t>, false);

    TEST (std::is_empty, virtual_derived_t<empty_t>, false);
    TEST (std::is_empty, virtual_derived_t<non_empty_t>, false);
}

static void test_is_polymorphic ()
{
    TEST (std::is_polymorphic, polymorphic_t, true);
    TEST (std::is_polymorphic, public_derived_t<polymorphic_t>, true);

    TEST (std::is_polymorphic, non_polymorphic_t, false);
    TEST (std::is_polymorphic, public_derived_t<non_polymorphic_t>, false);

    TEST (std::is_polymorphic, abstract_t, true);
    TEST (std::is_polymorphic, public_derived_t<abstract_t>, true);
}

static void test_is_abstract ()
{
    TEST (std::is_abstract, abstract_t, true);
    TEST (std::is_abstract, public_derived_t<abstract_t>, true);

    TEST (std::is_abstract, polymorphic_t, false); // polymorphic_t is not an abstract type
    TEST (std::is_abstract, public_derived_t<polymorphic_t>, false);
}

static void test_has_trivial_constructor ()
{
    TEST (std::has_trivial_default_constructor, long, true);
    TEST (std::has_trivial_default_constructor, long&, false);
    TEST (std::has_trivial_default_constructor, long[2], true);
    TEST (std::has_trivial_default_constructor, struct_t, true);

    TEST (std::has_trivial_default_constructor, no_trivial_ctor_t, false);
    TEST (std::has_trivial_default_constructor, public_derived_t<no_trivial_ctor_t>, false);
}   

static void test_has_trivial_copy ()
{
    TEST (std::has_trivial_copy_constructor, long, true);
    TEST (std::has_trivial_copy_constructor, long&, true);
    TEST (std::has_trivial_copy_constructor, long[2], true);

    TEST (std::has_trivial_copy_constructor, struct_t, true);

    TEST (std::has_trivial_copy_constructor, no_trivial_copy_t, false);
    TEST (std::has_trivial_copy_constructor, public_derived_t<no_trivial_copy_t>, false);
}

static void test_has_trivial_destructor ()
{
    TEST (std::has_trivial_destructor, long, true);
    TEST (std::has_trivial_destructor, long&, true);
    TEST (std::has_trivial_destructor, long[2], true);
    TEST (std::has_trivial_destructor, struct_t, true);

    TEST (std::has_trivial_destructor, no_trivial_dtor_t, false);
    TEST (std::has_trivial_destructor, public_derived_t<no_trivial_dtor_t>, false);
}

static void test_has_nothrow_constructor ()
{
    TEST (std::has_nothrow_default_constructor, long, true);
    TEST (std::has_nothrow_default_constructor, long&, false);
    TEST (std::has_nothrow_default_constructor, long[2], true);

    TEST (std::has_nothrow_default_constructor, struct_t, true);
    TEST (std::has_nothrow_default_constructor, throwing_ctor_t, false);
}

static void test_has_nothrow_copy ()
{
    TEST (std::has_nothrow_copy_constructor, long, true);
    TEST (std::has_nothrow_copy_constructor, long&, true);
    TEST (std::has_nothrow_copy_constructor, long[2], true);

    TEST (std::has_nothrow_copy_constructor, struct_t, true);
    TEST (std::has_nothrow_copy_constructor, throwing_copy_t, false);
}

static void test_has_virtual_destructor ()
{
    TEST (std::has_virtual_destructor, abstract_t, true);
    TEST (std::has_virtual_destructor, public_derived_t<abstract_t>, true);

    TEST (std::has_virtual_destructor, polymorphic_t, true);
    TEST (std::has_virtual_destructor, public_derived_t<polymorphic_t>, true);

    TEST (std::has_virtual_destructor, non_polymorphic_t, false);
    TEST (std::has_virtual_destructor, public_derived_t<non_polymorphic_t>, false);
}

static void test_is_signed ()
{
    TEST (std::is_signed, char, char(-1) < char(0));

    TEST (std::is_signed, signed char, true);
    TEST (std::is_signed, signed short, true);
    TEST (std::is_signed, signed int, true);
    TEST (std::is_signed, signed long, true);
    TEST (std::is_signed, signed long long, true);

    TEST (std::is_signed, unsigned char, false);
    TEST (std::is_signed, unsigned short, false);
    TEST (std::is_signed, unsigned int, false);
    TEST (std::is_signed, unsigned long, false);
    TEST (std::is_signed, unsigned long long, false);

    TEST (std::is_signed, float, true);
    TEST (std::is_signed, double, true);
    TEST (std::is_signed, long double, true);

    TEST (std::is_signed, enum_t, false);
    TEST (std::is_signed, struct_t, false);
    TEST (std::is_signed, class_t, false);
    TEST (std::is_signed, union_t, false);

    // add tests for references, pointers, arrays and functions
}

static void test_is_unsigned ()
{
    TEST (std::is_unsigned, char, char(0) < char(-1));

    TEST (std::is_unsigned, signed char, false);
    TEST (std::is_unsigned, signed short, false);
    TEST (std::is_unsigned, signed int, false);
    TEST (std::is_unsigned, signed long, false);
    TEST (std::is_unsigned, signed long long, false);

    TEST (std::is_unsigned, unsigned char, true);
    TEST (std::is_unsigned, unsigned short, true);
    TEST (std::is_unsigned, unsigned int, true);
    TEST (std::is_unsigned, unsigned long, true);
    TEST (std::is_unsigned, unsigned long long, true);

    TEST (std::is_unsigned, float, false);
    TEST (std::is_unsigned, double, false);
    TEST (std::is_unsigned, long double, false);

    TEST (std::is_unsigned, enum_t, false);
    TEST (std::is_unsigned, struct_t, false);
    TEST (std::is_unsigned, class_t, false);
    TEST (std::is_unsigned, union_t, false);
}

static void test_alignment_of ()
{
}

static void test_rank ()
{
#undef TEST
#define TEST(Trait, Type, Expect)                                   \
    test_trait (__LINE__, #Trait, #Type, Trait<Type>::value, Expect)

    TEST (std::rank, int, 0ul);
    TEST (std::rank, int [2], 1ul);
    TEST (std::rank, int [][4], 2ul);
    TEST (std::rank, int [4][4], 2ul);

    TEST (std::rank, void, 0ul);
    TEST (std::rank, void*, 0ul);

#undef TEST
}

static void test_extent ()
{
#undef TEST
#define TEST(Trait, Type, Depth, Expect)                             \
    test_trait (__LINE__, Depth, Trait<Type,Depth>::value, Expect,         \
                #Trait, #Type)

    TEST (std::extent, int, 0, 0ul);
    TEST (std::extent, int [2], 0, 2ul);
    TEST (std::extent, int [2][4], 0, 2ul);
    TEST (std::extent, int [][4], 0, 0ul);

    TEST (std::extent, int, 1, 0ul);
    TEST (std::extent, int [2], 1, 0ul);

    TEST (std::extent, int [2][4], 0, 2ul);
    TEST (std::extent, int [2][4], 1, 4ul);
    TEST (std::extent, int [2][4], 2, 0ul);
    
    TEST (std::extent, int [][4], 0, 0ul);
    TEST (std::extent, int [][4], 1, 4ul);
    TEST (std::extent, int [][4], 2, 0ul);

#undef TEST
}

static int run_test (int, char*[])
{
    test_is_const ();
    test_is_volatile ();
    test_is_trivial ();
    test_is_standard_layout ();
    test_is_pod ();
    test_is_empty ();
    test_is_polymorphic ();
    test_is_abstract ();
    test_has_trivial_constructor ();
    test_has_trivial_copy ();
    test_has_trivial_assign ();
    test_has_trivial_destructor ();
    test_has_nothrow_constructor ();
    test_has_nothrow_copy ();
    test_has_nothrow_assign ();
    test_has_virtual_destructor ();
    test_is_signed ();
    test_is_unsigned ();
    test_alignment_of ();
    test_rank ();
    test_extent ();

    return 0;
}

/**************************************************************************/

#else // _RWSTD_NO_EXT_CXX_0X

/**************************************************************************/

static int run_test (int, char*[])
{
    rw_warn (0, 0, __LINE__,
             "test disabled because _RWSTD_NO_EXT_CXX_0X is defined");
    return 0;
}

#endif // !_RWSTD_NO_EXT_CXX_0X

/**************************************************************************/

int main (int argc, char*argv[])
{
    return rw_test (argc, argv, __FILE__,
                    "meta.unary.prop",
                    0 /* no comment*/,
                    run_test,
                    0);
}
