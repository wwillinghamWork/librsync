/* -*- mode: c; c-file-style: "k&r" -*- */

/* libhsync/copyq.c -- Queue outgoing copy commands.
   
   Copyright (C) 1999-2000 by Martin Pool.
   Copyright (C) 1999-2000 by Peter Barker.
   Copyright (C) 1999 by Andrew Tridgell
   
   This program is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free 
   Software Foundation; either version 2 of the License, or (at your option)
   any later version.
   
   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
   or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
   for more details.
   
   You should have received a copy of the GNU General Public License along
   with this program; if not, write to the Free Software Foundation, Inc., 59 
   Temple Place, Suite 330, Boston, MA 02111-1307 USA 
  */


#include "includes.h"
#include "hsync.h"
#include "hsyncproto.h"
#include "private.h"



void _hs_queue_copy(_hs_copyq_t *copyq, size_t start, size_t len)
{
     if (copyq->len == 0) {
	  copyq->start = start;
	  copyq->len = len;
     } else { 
	  assert(copyq->start + copyq->len == start);
	  copyq->len += len;
     }
}


int _hs_copyq_flush(rs_write_fn_t write_fn, void *write_priv,
		     _hs_copyq_t *copyq,
		    hs_stats_t *stats)
{
     int ret;
     
     if (copyq->len == 0)
	  return 0;
     assert(copyq->len > 0);
     
     ret = _hs_emit_copy(write_fn, write_priv, copyq->start, copyq->len, stats);
     copyq->len = 0;

     return ret;
}