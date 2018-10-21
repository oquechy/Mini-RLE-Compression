LIBPATH=target/release
LIB=src/lib.rs
CFLAGS=-pthread -Wl,--no-as-needed -ldl

.PHONY: test clean

test: bin bin/test
	bin/test

bin:
	mkdir -p bin

$(LIBPATH)/libminirle.a: $(LIB)
	cargo build --release

bin/test: test.c minirle.h $(LIBPATH)/libminirle.a
	$(CC) $(CFLAGS) test.c $(LIBPATH)/libminirle.a -o bin/test

clean:
	rm -rf bin target
