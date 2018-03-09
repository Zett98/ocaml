/**************************************************************************/
/*                                                                     */
/*                                OCaml                                */
/*                                                                     */
/*            Xavier Leroy, projet Cristal, INRIA Rocquencourt         */
/*                                                                     */
/*  Copyright 1996 Institut National de Recherche en Informatique et   */
/*     en Automatique.                                                    */
/*                                                                     */
/*   All rights reserved.  This file is distributed under the terms of    */
/*   the GNU Lesser General Public License version 2.1, with the          */
/*   special exception on linking described in the file LICENSE.          */
/*                                                                        */
/**************************************************************************/

#include <caml/mlvalues.h>
#include <caml/fail.h>
#include <caml/alloc.h>
#include <caml/memory.h>
#include "unixsupport.h"
#include <stdio.h>
#include <grp.h>

static value alloc_group_entry(struct group *entry)
{
  value res;
  value name = Val_unit, pass = Val_unit, mem = Val_unit;

  Begin_roots3 (name, pass, mem);
    name = copy_string(entry->gr_name);
    pass = copy_string(entry->gr_passwd);
    mem = copy_string_array((const char**)entry->gr_mem);
    res = caml_alloc_4(0, name, pass,
                       Val_int(entry->gr_gid), mem);
  End_roots();
  return res;
}

CAMLprim value unix_getgrnam(value name)
{
  struct group * entry;
  if (! caml_string_is_c_safe(name)) raise_not_found();
  entry = getgrnam(String_val(name));
  if (entry == NULL) raise_not_found();
  return alloc_group_entry(entry);
}

CAMLprim value unix_getgrgid(value gid)
{
  struct group * entry;
  entry = getgrgid(Int_val(gid));
  if (entry == NULL) raise_not_found();
  return alloc_group_entry(entry);
}
