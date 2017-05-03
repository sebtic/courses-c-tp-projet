dist: clean provided-clean dist-clean all
	bash make/dist.sh
dist-clean:
	rm -rf bindist debug release provided provideddebug providedrelease doc