#!/bin/sh

MKDIR=$(dirname $0)

dirdeps() {
	DIR=$1
	MKFILE="${MKDIR}/${DIR}.mk"

	printf '.POSIX:\n\n' "$DIR" > $MKFILE

	for i in $(find $DIR -name '*.c'); do
		OBJ="$(echo $i | sed 's/\.c$/\.o/')"
		clang @make/CFLAGS -MM -MT "$OBJ" "$i"
		printf '\n'
	done >> $MKFILE

	printf '%s_OBJECTS=' "$(echo $DIR | tr [[:lower:]] [[:upper:]])" >> "$MKFILE"

	for i in $(find $DIR -name '*.s'); do
		printf '\\\n\t%s' "$(echo $i | sed 's/\.s$/\.o/')"
	done >> "$MKFILE"

	for i in $(find "$DIR" -name '*.c'); do
		printf '\\\n\t%s' "$(echo $i | sed 's/\.c$/\.o/')"
	done >> "$MKFILE"

	printf '\n' >> "$MKFILE"
}

dirdeps kernel
dirdeps lib
dirdeps user
