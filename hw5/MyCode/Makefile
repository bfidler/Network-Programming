.c.o:
	gcc -c -Wall -DRPC_SVC_FG $<


all: NameServer FileServer


NameServer: NameServer.o NameServer_svc.o NameServer_xdr.o
	gcc -o NameServer NameServer.o NameServer_svc.o NameServer_xdr.o -lrpcsvc -lnsl

FileServer: FileServer.o FileServer_svc.o FileServer_xdr.o NameServer.o NameServer_clnt.o NameServer_xdr.o
	gcc -o FileServer FileServer.o FileServer_svc.o FileServer_xdr.o NameServer.o NameServer_clnt.o NameServer_xdr.o -lrpcsvc -lnsl
