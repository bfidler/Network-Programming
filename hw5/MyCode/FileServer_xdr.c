/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "FileServer.h"
#include <string.h>

bool_t
xdr_flName (XDR *xdrs, flName *objp)
{
	register int32_t *buf;

	 if (!xdr_pointer (xdrs, (char **)&objp->name, sizeof (char), (xdrproc_t) xdr_char))
		 return FALSE;
	return TRUE;
}
