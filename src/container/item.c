/*!The Treasure Box Library
 * 
 * TBox is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 * 
 * TBox is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with TBox; 
 * If not, see <a href="http://www.gnu.org/licenses/"> http://www.gnu.org/licenses/</a>
 * 
 * Copyright (C) 2009 - 2012, ruki All rights reserved.
 *
 * @author		ruki
 * @file		item.c
 * @ingroup 	container
 *
 */
/* ///////////////////////////////////////////////////////////////////////
 * includes
 */
#include "item.h"
#include "../libc/libc.h"
#include "../utils/utils.h"
#include "../memory/memory.h"
#include "../platform/platform.h"

/* ///////////////////////////////////////////////////////////////////////
 * details
 */

// the integer for tb_long_t
static tb_long_t tb_item_func_long_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return ((tb_long_t)ldata > (tb_long_t)rdata? 1 : ((tb_long_t)ldata < (tb_long_t)rdata? -1 : 0));
	//return (ldata - rdata); //!< maybe overflow for int32
}
static tb_pointer_t tb_item_func_long_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);

	return (tb_pointer_t)*((tb_long_t*)item);
}
static tb_char_t const* tb_item_func_long_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%ld", (tb_long_t)data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_long_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);
	*((tb_long_t*)item) = (tb_long_t)data;
}
static tb_void_t tb_item_func_long_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);

	if (func->size == 4) tb_memset_u32(item, data, size);
	else while (size--) ((tb_long_t*)item)[size] = (tb_long_t)data;
}

// the integer for tb_size_t
static tb_size_t tb_item_func_size_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && tb_ispow2(size), 0);
	return (((tb_size_t)data) ^ 0xdeadbeef) & (size - 1);
}
static tb_long_t tb_item_func_size_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return ((tb_size_t)ldata > (tb_size_t)rdata? 1 : ((tb_size_t)ldata < (tb_size_t)rdata? -1 : 0));
	//return (ldata - rdata); //!< maybe overflow for int32
}
static tb_pointer_t tb_item_func_size_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return (tb_pointer_t)*((tb_size_t*)item);
}
static tb_char_t const* tb_item_func_size_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%lu", (tb_size_t)data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_size_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	*((tb_size_t*)item) = 0;
}
static tb_void_t tb_item_func_size_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);
	*((tb_size_t*)item) = data;
}
static tb_void_t tb_item_func_size_nfree(tb_item_func_t* func, tb_pointer_t item, tb_size_t size)
{
	tb_assert_and_check_return(func && item);
	if (size) tb_memset(item, 0, size * func->size);
}
static tb_void_t tb_item_func_size_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);

	if (func->size == 4) tb_memset_u32(item, data, size);
	else while (size--) ((tb_size_t*)item)[size] = (tb_size_t)data;
}
// the integer for tb_uint8_t
static tb_size_t tb_item_func_uint8_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && tb_ispow2(size), 0);
	return (((tb_uint8_t)data) ^ 0xbe) & (size - 1);
}
static tb_long_t tb_item_func_uint8_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return ((tb_uint8_t)ldata > (tb_uint8_t)rdata? 1 : ((tb_uint8_t)ldata < (tb_uint8_t)rdata? -1 : 0));
	//return (ldata - rdata); //!< maybe overflow for int32
}
static tb_pointer_t tb_item_func_uint8_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return (tb_pointer_t)*((tb_uint8_t*)item);
}
static tb_char_t const* tb_item_func_uint8_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%u", (tb_uint8_t)data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_uint8_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint8_t*)item) = 0;
}
static tb_void_t tb_item_func_uint8_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint8_t*)item) = (tb_uint8_t)data;
}
static tb_void_t tb_item_func_uint8_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);
	tb_memset(item, data, size);
}
// the integer for tb_uint16_t
static tb_size_t tb_item_func_uint16_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && tb_ispow2(size), 0);
	return (((tb_uint16_t)data) ^ 0xbeaf) & (size - 1);
}
static tb_long_t tb_item_func_uint16_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return ((tb_uint16_t)ldata > (tb_uint16_t)rdata? 1 : ((tb_uint16_t)ldata < (tb_uint16_t)rdata? -1 : 0));
	//return (ldata - rdata); //!< maybe overflow for int32
}
static tb_pointer_t tb_item_func_uint16_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return (tb_pointer_t)*((tb_uint16_t*)item);
}
static tb_char_t const* tb_item_func_uint16_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%u", (tb_uint16_t)data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_uint16_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint16_t*)item) = 0;
}
static tb_void_t tb_item_func_uint16_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint16_t*)item) = (tb_uint16_t)data;
}
static tb_void_t tb_item_func_uint16_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);
	tb_memset_u16(item, (tb_uint16_t)data, size);
}
// the integer for tb_uint32_t
static tb_size_t tb_item_func_uint32_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && tb_ispow2(size), 0);
	return (((tb_uint32_t)data) ^ 0xdeadbeef) & (size - 1);
}
static tb_long_t tb_item_func_uint32_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return ((tb_uint32_t)ldata > (tb_uint32_t)rdata? 1 : ((tb_uint32_t)ldata < (tb_uint32_t)rdata? -1 : 0));
	//return (ldata - rdata); //!< maybe overflow for int32
}
static tb_pointer_t tb_item_func_uint32_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return (tb_pointer_t)*((tb_uint32_t*)item);
}
static tb_char_t const* tb_item_func_uint32_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%u", (tb_uint32_t)data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_uint32_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint32_t*)item) = 0;
}
static tb_void_t tb_item_func_uint32_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);
	*((tb_uint32_t*)item) = (tb_uint32_t)data;
}
static tb_void_t tb_item_func_uint32_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);
	tb_memset_u32(item, (tb_uint32_t)data, size);
}
// the cstring
static tb_size_t tb_item_func_str_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && data && tb_ispow2(size), 0);

	tb_size_t h = 2166136261;
	tb_byte_t const* p = data;
	while (*p) h = 16777619 * h ^ (tb_size_t)(*p++);

	return (h & (size - 1));
}
static tb_long_t tb_item_func_str_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	tb_assert_and_check_return_val(func, 0);
	return func->priv? tb_strcmp(ldata, rdata) : tb_stricmp(ldata, rdata);
}
static tb_char_t const* tb_item_func_str_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func, "");
	return (tb_char_t const*)data;
}
static tb_void_t tb_item_func_str_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	if (*((tb_pointer_t*)item)) 
	{
		if (func->pool) tb_spool_free(func->pool, *((tb_pointer_t*)item));
		else tb_free(*((tb_pointer_t*)item));
	}
}
static tb_void_t tb_item_func_str_dupl(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);

	if (data) *((tb_pointer_t*)item) = func->pool? tb_spool_strdup(func->pool, data) : tb_strdup(data);
	else *((tb_pointer_t*)item) = tb_null;
}
static tb_void_t tb_item_func_str_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);

	if (func->pool) *((tb_pointer_t*)item) = data? tb_spool_strdup(func->pool, data) : tb_null;
	else *((tb_pointer_t*)item) = data? tb_strdup(data) : tb_null;
}

// the pointer
static tb_size_t tb_item_func_ptr_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && tb_ispow2(size), 0);
	return (((tb_size_t)data) ^ 0xdeadbeef) & (size - 1);
}
static tb_long_t tb_item_func_ptr_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	return (ldata - rdata);
}
static tb_pointer_t tb_item_func_ptr_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);

	return *((tb_pointer_t*)item);
}
static tb_char_t const* tb_item_func_ptr_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && cstr, "");
	tb_long_t n = tb_snprintf(cstr, maxn, "%x", data);
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_ptr_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	*((tb_pointer_t*)item) = tb_null;
}
static tb_void_t tb_item_func_ptr_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);

	// copy it
	*((tb_pointer_t*)item) = data;
}
static tb_void_t tb_item_func_ptr_nfree(tb_item_func_t* func, tb_pointer_t item, tb_size_t size)
{
	// check
	tb_assert_and_check_return(func && item);

	// the free is not hooked? or null
	if (func->free == tb_item_func_ptr_free || !func->free)
	{
		// clear
		if (size) tb_memset(item, 0, size * func->size);
	}
	else
	{
		while (size--) func->free(func, (tb_byte_t*)item + size * func->size);
	}
}
static tb_void_t tb_item_func_ptr_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && item);

	// copy it
	if (func->size == 4) tb_memset_u32(item, data, size);
	else while (size--) ((tb_pointer_t*)item)[size] = data;
}

// the external fixed memory
static tb_void_t tb_item_func_efm_free(tb_item_func_t* func, tb_pointer_t item)
{
	tb_assert_and_check_return(func && item);
	if (func->pool) tb_rpool_free(func->pool, *((tb_pointer_t*)item));
	else if (*((tb_pointer_t*)item)) tb_free(*((tb_pointer_t*)item));
}
static tb_pointer_t tb_item_func_efm_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return *((tb_pointer_t*)item);
}
static tb_void_t tb_item_func_efm_dupl(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);

	if (func->pool) *((tb_pointer_t*)item) = data? tb_rpool_memdup(func->pool, data) : 0;
	else if (func->priv) *((tb_pointer_t*)item) = data? tb_memdup(data, func->priv) : tb_null;
}
static tb_void_t tb_item_func_efm_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && item);

	if (*((tb_pointer_t*)item) && func->priv) 
	{
		if (data) tb_memcpy(*((tb_pointer_t*)item), data, func->priv);
		else tb_memset(*((tb_pointer_t*)item), 0, func->priv);
	}
}
static tb_size_t tb_item_func_efm_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && func->priv && data && tb_ispow2(size), 0);
	return (tb_crc_encode(TB_CRC_MODE_32_IEEE_LE, 0, data, func->priv) & (size - 1));
}
static tb_long_t tb_item_func_efm_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	tb_assert_and_check_return_val(func && func->priv && ldata && rdata, 0);
	return tb_memcmp(ldata, rdata, func->priv);
}
static tb_char_t const* tb_item_func_efm_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && func->priv && cstr && data, "");

	// data => string
	tb_long_t n = tb_snprintf(cstr, maxn, "0x%x", tb_crc_encode(TB_CRC_MODE_32_IEEE_LE, 0, data, func->priv));
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_efm_nfree(tb_item_func_t* func, tb_pointer_t item, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);
	if (func->free)
		while (size--) func->free(func, (tb_byte_t*)item + size * func->size);
}
static tb_void_t tb_item_func_efm_ndupl(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);
	if (func->dupl)
		while (size--) func->dupl(func, (tb_byte_t*)item + size * func->size, data);
}
static tb_void_t tb_item_func_efm_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);
	if (func->copy)
		while (size--) func->copy(func, (tb_byte_t*)item + size * func->size, data);
}
// the internal fixed memory
static tb_size_t tb_item_func_ifm_hash(tb_item_func_t* func, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return_val(func && func->size && data && tb_ispow2(size), 0);
	return (tb_crc_encode(TB_CRC_MODE_32_IEEE_LE, 0, data, func->size) & (size - 1));
}
static tb_long_t tb_item_func_ifm_comp(tb_item_func_t* func, tb_cpointer_t ldata, tb_cpointer_t rdata)
{
	tb_assert_and_check_return_val(func && func->size && ldata && rdata, 0);
	return tb_memcmp(ldata, rdata, func->size);
}
static tb_pointer_t tb_item_func_ifm_data(tb_item_func_t* func, tb_cpointer_t item)
{
	tb_assert_and_check_return_val(func && item, tb_null);
	return item;
}
static tb_char_t const* tb_item_func_ifm_cstr(tb_item_func_t* func, tb_cpointer_t data, tb_char_t* cstr, tb_size_t maxn)
{
	tb_assert_and_check_return_val(func && func->size && cstr && data, "");

	// data => string
	tb_long_t n = tb_snprintf(cstr, maxn, "0x%x", tb_crc_encode(TB_CRC_MODE_32_IEEE_LE, 0, data, func->size));
	if (n > 0) cstr[n] = '\0';
	return (tb_char_t const*)cstr;
}
static tb_void_t tb_item_func_ifm_dupl(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && func->size && item);

	// copy item
	if (data) tb_memcpy(item, data, func->size);
	else tb_memset(item, 0, func->size);
}
static tb_void_t tb_item_func_ifm_copy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data)
{
	tb_assert_and_check_return(func && func->size && item);

	// free item
	if (func->free) func->free(func, item);

	// copy item
	if (data) tb_memcpy(item, data, func->size);
	else tb_memset(item, 0, func->size);
}
static tb_void_t tb_item_func_ifm_nfree(tb_item_func_t* func, tb_pointer_t item, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);
	if (func->free)
		while (size--) func->free(func, (tb_byte_t*)item + size * func->size);
}
static tb_void_t tb_item_func_ifm_ndupl(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);

	// copy item
	if (data) while (size--) tb_memcpy((tb_byte_t*)item + size * func->size, data, func->size);
	else tb_memset(item, 0, func->size * size);
}
static tb_void_t tb_item_func_ifm_ncopy(tb_item_func_t* func, tb_pointer_t item, tb_cpointer_t data, tb_size_t size)
{
	tb_assert_and_check_return(func && func->size && item);

	// free item
	if (func->nfree) func->nfree(func, item, size);

	// copy item
	if (func->ndupl) func->ndupl(func, item, data, size);
}
/* ///////////////////////////////////////////////////////////////////////
 * implementation
 */
tb_item_func_t tb_item_func_long()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_LONG;

	func.hash = tb_item_func_size_hash;
	func.comp = tb_item_func_long_comp;

	func.data = tb_item_func_long_data;
	func.cstr = tb_item_func_long_cstr;

	func.free = tb_item_func_size_free;
	func.dupl = tb_item_func_long_copy;
	func.copy = tb_item_func_long_copy;

	func.nfree = tb_item_func_size_nfree;
	func.ndupl = tb_item_func_long_ncopy;
	func.ncopy = tb_item_func_long_ncopy;

	func.size = sizeof(tb_long_t);

	return func;
}
tb_item_func_t tb_item_func_size()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_SIZE;

	func.hash = tb_item_func_size_hash;
	func.comp = tb_item_func_size_comp;

	func.data = tb_item_func_size_data;
	func.cstr = tb_item_func_size_cstr;

	func.free = tb_item_func_size_free;
	func.dupl = tb_item_func_size_copy;
	func.copy = tb_item_func_size_copy;

	func.nfree = tb_item_func_size_nfree;
	func.ndupl = tb_item_func_size_ncopy;
	func.ncopy = tb_item_func_size_ncopy;

	func.size = sizeof(tb_size_t);

	return func;
}
tb_item_func_t tb_item_func_uint8()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_UINT8;

	func.hash = tb_item_func_uint8_hash;
	func.comp = tb_item_func_uint8_comp;

	func.data = tb_item_func_uint8_data;
	func.cstr = tb_item_func_uint8_cstr;

	func.free = tb_item_func_uint8_free;
	func.dupl = tb_item_func_uint8_copy;
	func.copy = tb_item_func_uint8_copy;

	func.nfree = tb_item_func_size_nfree;
	func.ndupl = tb_item_func_uint8_ncopy;
	func.ncopy = tb_item_func_uint8_ncopy;

	func.size = sizeof(tb_uint8_t);

	return func;
}
tb_item_func_t tb_item_func_uint16()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_UINT16;

	func.hash = tb_item_func_uint16_hash;
	func.comp = tb_item_func_uint16_comp;

	func.data = tb_item_func_uint16_data;
	func.cstr = tb_item_func_uint16_cstr;

	func.free = tb_item_func_uint16_free;
	func.dupl = tb_item_func_uint16_copy;
	func.copy = tb_item_func_uint16_copy;

	func.nfree = tb_item_func_size_nfree;
	func.ndupl = tb_item_func_uint16_ncopy;
	func.ncopy = tb_item_func_uint16_ncopy;

	func.size = sizeof(tb_uint16_t);

	return func;
}
tb_item_func_t tb_item_func_uint32()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_UINT32;

	func.hash = tb_item_func_uint32_hash;
	func.comp = tb_item_func_uint32_comp;

	func.data = tb_item_func_uint32_data;
	func.cstr = tb_item_func_uint32_cstr;

	func.free = tb_item_func_uint32_free;
	func.dupl = tb_item_func_uint32_copy;
	func.copy = tb_item_func_uint32_copy;

	func.nfree = tb_item_func_size_nfree;
	func.ndupl = tb_item_func_uint32_ncopy;
	func.ncopy = tb_item_func_uint32_ncopy;

	func.size = sizeof(tb_uint32_t);

	return func;
}
tb_item_func_t tb_item_func_str(tb_bool_t bcase, tb_handle_t spool)
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_STR;
	func.hash = tb_item_func_str_hash;
	func.comp = tb_item_func_str_comp;

	func.data = tb_item_func_ptr_data;
	func.cstr = tb_item_func_str_cstr;

	func.free = tb_item_func_str_free;
	func.dupl = tb_item_func_str_dupl;
	func.copy = tb_item_func_str_copy;

	func.nfree = tb_item_func_efm_nfree;
	func.ndupl = tb_item_func_efm_ndupl;
	func.ncopy = tb_item_func_efm_ncopy;

	func.size = sizeof(tb_pointer_t);
	func.pool = spool;
	func.priv = (tb_pointer_t)bcase;

	return func;
}
tb_item_func_t tb_item_func_ptr()
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_PTR;

	func.hash = tb_item_func_ptr_hash;
	func.comp = tb_item_func_ptr_comp;

	func.data = tb_item_func_ptr_data;
	func.cstr = tb_item_func_ptr_cstr;

	func.free = tb_item_func_ptr_free;
	func.dupl = tb_item_func_ptr_copy;
	func.copy = tb_item_func_ptr_copy;

	func.nfree = tb_item_func_ptr_nfree;
	func.ndupl = tb_item_func_ptr_ncopy;
	func.ncopy = tb_item_func_ptr_ncopy;

	func.size = sizeof(tb_pointer_t);

	return func;
}
tb_item_func_t tb_item_func_efm(tb_size_t size, tb_handle_t rpool)
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_EFM;

	func.hash = tb_item_func_efm_hash;
	func.comp = tb_item_func_efm_comp;

	func.data = tb_item_func_efm_data;
	func.cstr = tb_item_func_efm_cstr;

	func.free = tb_item_func_efm_free;
	func.dupl = tb_item_func_efm_dupl;
	func.copy = tb_item_func_efm_copy;

	func.nfree = tb_item_func_efm_nfree;
	func.ndupl = tb_item_func_efm_ndupl;
	func.ncopy = tb_item_func_efm_ncopy;

	func.size = sizeof(tb_pointer_t);
	func.pool = rpool;
	func.priv = (tb_pointer_t)size;

	return func;
}
tb_item_func_t tb_item_func_ifm(tb_size_t size, tb_item_func_free_t free, tb_pointer_t priv)
{
	tb_item_func_t func;
	tb_memset(&func, 0, sizeof(tb_item_func_t));
	func.type = TB_ITEM_TYPE_IFM;

	func.hash = tb_item_func_ifm_hash;
	func.comp = tb_item_func_ifm_comp;

	func.data = tb_item_func_ifm_data;
	func.cstr = tb_item_func_ifm_cstr;

	func.free = free;
	func.dupl = tb_item_func_ifm_dupl;
	func.copy = tb_item_func_ifm_copy;

	func.nfree = tb_item_func_ifm_nfree;
	func.ndupl = tb_item_func_ifm_ndupl;
	func.ncopy = tb_item_func_ifm_ncopy;

	func.size = size;
	func.priv = priv;

	return func;
}
