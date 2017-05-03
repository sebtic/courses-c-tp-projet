all: clean prof 

clean: 
	@rm -rf dist build-prof build-student build-student-print

prof:
	@echo "Building TDsTPs professor version..."
	@mkdir -p build-prof
	@cp -r src/* build-prof
	@echo "\\documentclass[french,correction]{marvelous}\n" > build-prof/TDsTPs-prof.tex
	@cat build-prof/TDsTPs.tex | grep -v documentclass | grep -v newbool | grep -v setbool >> build-prof/TDsTPs-prof.tex
	@cd build-prof && latexmk -pdf TDsTPs-prof.tex
	@mkdir -p dist
	@cp build-prof/TDsTPs-prof.pdf dist/
	@rm -rf build-prof
	
student-print:
	@echo "Building TDsTPs student black&white version..."
	@mkdir -p build-student-print
	@cp -r src/* build-student-print
	@echo "\\documentclass[french,blackandwhite,openany]{marvelous}\n" > build-student-print/TDsTPs-student-print.tex
	@cat build-student-print/TDsTPs.tex | grep -v documentclass | grep -v newbool | grep -v setbool >> build-student-print/TDsTPs-student-print.tex
	@cd build-student-print && (pdflatex TDsTPs-student-print ; pdflatex TDsTPs-student-print ; pdflatex TDsTPs-student-print ) > /dev/null
	@(cd build-student-print && pdfnup --no-tidy TDsTPs-student-print.pdf 2>/dev/null >/dev/null) || (cp build-student-print/TDsTPs-student-print.pdf build-student-print/TDsTPs-student-print-nup.pdf)
	@mkdir -p dist
	@cp build-student-print/TDsTPs-student-print-nup.pdf dist/
	@rm -rf build-student-print
	
student:
	@echo "Building TDsTPs student color version..."
	@mkdir -p build-student
	@cp -r src/* build-student
	@echo "\\documentclass[french]{marvelous}\n" > build-student/TDsTPs-student.tex
	@cat build-student/TDsTPs.tex | grep -v documentclass | grep -v newbool | grep -v setbool >> build-student/TDsTPs-student.tex
	@cd build-student && (pdflatex TDsTPs-student ; pdflatex TDsTPs-student ; pdflatex TDsTPs-student) > /dev/null
	@mkdir -p dist
	@cp build-student/TDsTPs-student.pdf dist/
	@rm -rf build-student
	
