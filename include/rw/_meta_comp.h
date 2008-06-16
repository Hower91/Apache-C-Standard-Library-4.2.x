/***************************************************************************
 *
 * This is an internal header file used to implement the C++ Standard
 * Library. It should never be #included directly by a program.
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
 * Copyright 2008 Rogue Wave Software, Inc.
 *
 **************************************************************************/

#ifndef _RWSTD_RW_META_COMP_H_INCLUDED
#define _RWSTD_RW_META_COMP_H_INCLUDED

#include <rw/_defs.h>
#include <rw/_meta_cat.h>

_RWSTD_NAMESPACE (__rw) {

/**
 * UnaryTypeTrait indicates that _TypeT is either a lval or
 * rval reference type.
 */
template <class _TypeT>
struct __rw_is_reference
    : __rw_integral_constant<bool,    __rw_is_lvalue_reference<_TypeT>::value
                                   || __rw_is_rvalue_reference<_TypeT>::value>
{
};

/**
 * UnaryTypeTrait indicates that _TypeT is an arithmethic type.
 *
 * The arithmetic types include all integral and floating point
 * types.
 */
template <class _TypeT>
struct __rw_is_arithmetic
    : __rw_integral_constant<bool,    __rw_is_integral<_TypeT>::value
                                   || __rw_is_floating_point<_TypeT>::value>
{
};

/**
 * UnaryTypeTrait indicates that _TypeT is an fundamental type.
 *
 * The fundamental types include void and all of the standard
 * arithmetic types.
 */
template <class _TypeT>
struct __rw_is_fundamental
    : __rw_integral_constant<bool,    __rw_is_void<_TypeT>::value
                                   || __rw_is_arithmetic<_TypeT>::value>
{
};


/**
 * UnaryTypeTrait indicates that _TypeT is an object type.
 *
 * An object is a (possibly cv-qualified) type that is not a
 * function type, not a reference type, and not a void type.
 */
template <class _TypeT>
struct __rw_is_object
    : __rw_integral_constant<bool,    !__rw_is_function<_TypeT>::value
                                   && !__rw_is_reference<_TypeT>::value
                                   && !__rw_is_void<_TypeT>::value>
{
};

/**
 * UnaryTypeTrait indicates that _TypeT is a member pointer type.
 *
 * The member pointer types include pointers to member functions and
 * pointer to member data. Neither pointers to static member functions
 * or pointers to static member data are included.
 */
template <class _TypeT>
struct __rw_is_member_pointer
    : __rw_integral_constant<bool,    __rw_is_member_function_pointer<_TypeT>::value
                                   || __rw_is_member_object_pointer<_TypeT>::value>
{
};

/**
 * UnaryTypeTrait indicates that _TypeT is a scalar type.
 *
 * The scalar types include arithmetic types, enumeration types,
 * pointer types, pointer to member types, std::nullptr_t and
 * cv-qualified versions of these types.
 */
template <class _TypeT>
struct __rw_is_scalar
    : __rw_integral_constant<bool,    __rw_is_arithmetic<_TypeT>::value
                                   || __rw_is_enum<_TypeT>::value
                                   || __rw_is_pointer<_TypeT>::value
                                   || __rw_is_member_pointer<_TypeT>::value>
{
    /**
     * todo need to handle special case
     *
     * __rw_is_same<std::nullptr_t, __rw_remove_cv<_TypeT>::type>::value
     */
};

/**
 * UnaryTypeTrait indicates that _TypeT is a compound type.
 *
 * The compound types include arrays, functions, pointers, references,
 * classes, unions, enumerations, pointer to non-static class members
 */
template <class _TypeT>
struct __rw_is_compound
    : __rw_integral_constant<bool,    __rw_is_array<_TypeT>::value
                                   || __rw_is_function<_TypeT>::value
                                   || __rw_is_pointer<_TypeT>::value
                                   || __rw_is_reference<_TypeT>::value
                                   || __rw_is_class<_TypeT>::value
                                   || __rw_is_union<_TypeT>::value
                                   || __rw_is_enum<_TypeT>::value
                                   || __rw_is_member_pointer<_TypeT>::value>
{
};

} // namespace __rw


#endif   // _RWSTD_RW_META_COMP_H_INCLUDED

