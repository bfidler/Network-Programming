.c.o:
	gcc -c -Wall -DRPC_SVC_FG $<


all: NameServer FileServer client


NameServer: NameServer.o NameServer_svc.o NameServer_xdr.o
	gcc -o NameServer NameServer.o NameServer_svc.o NameServer_xdr.o -lrpcsvc -lnsl

FileServer: FileServer.o FileServer_svc.o FileServer_xdr.o NameServer.o NameServer_clnt.o NameServer_xdr.o
	gcc -o FileServer FileServer.o FileServer_svc.o FileServer_xdr.o NameServer.o NameServer_clnt.o NameServer_xdr.o -lrpcsvc -lnsl

client: client.o NameServer.o NameServer_clnt.o NameServer_xdr.o FileServer.o FileServer_clnt.o FileServer_xdr.o
	gcc -o client client.o NameServer.o NameServer_clnt.o NameServer_xdr.o FileServer.o FileServer_clnt.o FileServer_xdr.o -lnsl

clean:
	rm *.o NameServer FileServer
