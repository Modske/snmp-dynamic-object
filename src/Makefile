BIN:=str_variable.so
CFLAGS:=-Wall -fPIC 
CPPFLAGS:=
LDLFLAGS:=-lnetsnmp -luci

all:$(BIN)

$(BIN): main.o uci_cfg.o
	$(CC) -o $@ $^ $(CFLAGS) -shared $(LDLFLAGS)

main.o: uci_cfg.o
	$(CC) -c $(CFLAGS) main.c $(LDLFLAGS) 

uci_cfg.o: uci_cfg.c
	$(CC) -c $(CFLAGS) $^ $(LDLFLAGS) 