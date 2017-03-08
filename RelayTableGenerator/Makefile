
default: RelayTable.h

combinations: combinations.c
	gcc combinations.c -o combinations

relayTable.h: combinations
	./combinations > relayTable.h

test: relayTable.h
	bash checkit.sh

install: relayTable.h
	cp relayTable.h ../ArenaControl

clean: 
	rm -f combinations
