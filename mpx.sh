#!/bin/sh
set -e

if !(make -q); then
	printf 'Your kernel.bin is out-of-date. Remember to run `make`'
	tput bold
	printf ' and fix any errors.\n'
	tput sgr0
	exit 1
fi

termsettings=$(stty -g)
qemu-system-i386 -nographic -kernel kernel.bin -s "$@"
stty "${termsettings}"
tput -S <<-EOF
	sgr0
	smam
EOF
