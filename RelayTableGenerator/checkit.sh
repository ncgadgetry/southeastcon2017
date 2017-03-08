echo "Number of lines in the table should be 88, is" `grep \# < relayTable.h | wc -l`

echo "Counts/Positions/counts for the wire: count 88, positions should skip 1:"
grep \# < relayTable.h | cut -d\  -f8  | cut -d, -f1 | sort | uniq -c

echo "Counts/Positions for the resistor: count 88, positions should skip 2:"
grep \# < relayTable.h | cut -d\  -f10 | cut -d, -f1 | sort | uniq -c

echo "Counts/Positions for the capacitor: count 88, positions should skip 3:"
grep \# < relayTable.h | cut -d\  -f9  | cut -d, -f1 | sort | uniq -c

echo "Counts/Positions for the inductor: count 88, positions should skip 4:"
grep \# < relayTable.h | cut -d\  -f11 | cut -d, -f1 | sort | uniq -c

echo "Counts/Positions for the diode: count 88, positions should skip 5:"
grep \# < relayTable.h | cut -d\  -f12 | cut -d, -f1 | sort | uniq -c

echo "Count how many times each component is found - each should be 88"
echo W: `grep \# < relayTable.h | cut -d\  -f15 | grep -c W`
echo C: `grep \# < relayTable.h | cut -d\  -f15 | grep -c C`
echo R: `grep \# < relayTable.h | cut -d\  -f15 | grep -c R`
echo I: `grep \# < relayTable.h | cut -d\  -f15 | grep -c I`

echo "Count how many anode/cathode oriented diodes - each should be 44"
echo D: `grep \# < relayTable.h | cut -d\  -f15 | grep -c D`
echo d: `grep \# < relayTable.h | cut -d\  -f15 | grep -c d`

