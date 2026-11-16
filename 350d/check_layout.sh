#!/bin/sh
# Checks that the add-on and Canon's main heap don't overlap. Three
# separate places have to agree on ADDON_BASE: the address the code is
# linked to run at, the destination COPY() copies it to, and the heap end
# that my_taskcreate_Startup passes to CreateMainHeap. A build where they
# disagree would run code the heap can hand out.
#
# Usage: check_layout.sh <elf> <ADDON_BASE>
elf=$1
base=$(printf '%d' "$2")
objdump=${CROSS:-arm-none-eabi-}objdump
fail=0

# Print, in decimal, each constant a function loads: MOV immediates and
# the words its LDRs read from the literal pool.
consts() {
	$objdump -d "$elf" | awk -v f="<$1>:" '
		$2 == f { on = 1; next }
		on && /^$/ { exit }
		on && /\tmov\tr[0-9]+, #/ {
			sub(/.*#/, ""); sub(/[ \t].*/, ""); print "imm " $0
		}
		on && /\tldr\tr[0-9]+, \[pc, #-?[0-9]+\]/ {
			sub(/.*@ /, ""); sub(/ .*/, ""); print "lit " $0
		}' |
	while read -r kind v; do
		if [ "$kind" = imm ]; then
			printf '%d\n' "$v"
		else
			# The word at address v, from a hex dump of 4 bytes,
			# assembled little-endian.
			$objdump -s --start-address=0x"$v" \
				--stop-address=$((0x$v + 4)) "$elf" |
			awk 'NR > 4 && NF >= 2 { print $2; exit }' |
			sed 's/\(..\)\(..\)\(..\)\(..\)/\4\3\2\1/' |
			while read -r w; do printf '%d\n' 0x"$w"; done
		fi
	done
}

check() {
	if consts "$1" | grep -qx "$base"; then
		echo "ok: $2"
	else
		echo "FAIL: $2 is not $(printf '0x%X' "$base")!" >&2
		fail=1
	fi
}

start=$($objdump -t "$elf" | awk '$NF == "_start" { print $1 }')
if [ "$((0x$start))" = "$base" ]; then
	echo "ok: _start is linked at the add-on base"
else
	echo "FAIL: _start is at 0x$start, not $(printf '0x%X' "$base")!" >&2
	fail=1
fi

check COPY "COPY() destination"
check my_taskcreate_Startup "heap end passed to CreateMainHeap"

exit $fail
