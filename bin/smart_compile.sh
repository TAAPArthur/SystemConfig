#!/bin/sh

make -f - "$1.lint" <<EOF
-include config.mk

%.lint:
	tcc \$(CPPFLAGS) -c -o /dev/null $*
EOF
exit 0
while : ; do
	if [ -r Makefile ] || [ -r makefile ]; then
		exec make
	fi
	if [ -d .git ] || [ -d .root ] || [ "$PWD" = / ]; then
		break
	fi
	cd ..
done
