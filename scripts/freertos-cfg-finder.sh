#!/bin/bash
INPUT_FILE=$1
GREP_DIR=$2

PATTERN_LIST=$(awk '/#define\ +[^()]+[ \t]+.+/{print $2}' $INPUT_FILE | awk '!a[$0]++')

for pattern in $PATTERN_LIST; do
	RESULT=$(grep -ron --include="*.[chs]" ${pattern} $GREP_DIR | cut -d ':' --output-delimiter='-' -f 1,2)
	COUNT=$(wc -w <<< $RESULT)
	echo $pattern" "$COUNT
	if [ "$RESULT" != "" ]; then
		for i in $RESULT; do
			echo $i
		done
	fi
	echo
done
