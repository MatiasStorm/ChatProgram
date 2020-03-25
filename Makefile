all:
	cd src && $(MAKE) $@

test:
	cd tests && $(MAKE) $@

clean:
	cd tests && $(MAKE) $@
	# cd src && $(MAKE) $@