SUBDIR = server client
#all-recursive clean-recursive depand-recursive install-recursive:
all-recursive clean-recursive install-recursive:
	@target=`echo $@ | sed s/-recursive//`;\
	list='$(SUBDIR)';\
	for subdir in $$list; do\
		echo "===> Making $$target in $$subdir...";\
		(cd $$subdir && $(MAKE) $$target)\
	done;

all: all-recursive

clean: clean-recursive

depand: depand-recursive

install: install-recursive

#.PHONY : server
#	make "SUBDIR = server"
#.PHONY : client
#	make "SUBDIR = client"
