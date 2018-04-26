.c.o:
	gcc -c -Wall -DRPC_SVC_FG $<


all: NameServer


NameServer: NameServer.o NameServer_svc.o NameServer_xdr.o
	gcc -o NameServer NameServer.o NameServer_svc.o NameServer_xdr.o -lrpcsvc -lnsl
