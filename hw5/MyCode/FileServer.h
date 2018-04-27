/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _FILESERVER_H_RPCGEN
#define _FILESERVER_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

struct flName {
	char *name;
};
typedef struct flName flName;

#define FILE_SERVER 2222222
#define NAME_VERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define getFile 1
extern  char ** getfile_1(flName *, CLIENT *);
extern  char ** getfile_1_svc(flName *, struct svc_req *);
extern int file_server_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define getFile 1
extern  char ** getfile_1();
extern  char ** getfile_1_svc();
extern int file_server_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_flName (XDR *, flName*);

#else /* K&R C */
extern bool_t xdr_flName ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_FILESERVER_H_RPCGEN */