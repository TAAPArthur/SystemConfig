all: mystatusbar

mystatusbar: mystatusbar.c
	${CC} -o $@ $^

install: mystatusbar
	install -D -m 0755 -t "$(DESTDIR)/usr/bin/" $^
