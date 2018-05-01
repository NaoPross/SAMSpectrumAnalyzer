# build options
RELEASE := 0

# git
GIT := git

# qmake
ifeq ($(shell lsb_release -si),Debian)
QMAKE := QT_SELECT=qt5 qmake
else
QMAKE := qmake-qt5 
endif
QMAKE_ARGS := -makefile -o Makefile -Wall 

ifeq ($(RELEASE),1)
	QMAKE_ARGS += "CONFIG+=release"
else
	QMAKE_ARGS += "CONFIG+=debug" "CONFIG+=qml_debug"
endif


# targets
all: desktop

.PHONY: run
run: desktop
	./build-desktop/SpectrumAnalyzer
	
.PHONY: debug
debug: desktop
	gdb ./build-desktop/SpectrumAnalyzer


.PHONY: desktop
desktop: src-desktop
	mkdir -p build-desktop
	$(QMAKE) $(QMAKE_ARGS) -o build-desktop/ src-desktop
	$(MAKE) -C build-desktop

.PHONY: clean
clean:
	rm -rf build-desktop
	rm -rf lib/build-serial

.PHONY: build-deps
build-deps: serial qcustomplot

# libraries
.PHONY: serial
serial: lib/serial/src
	mkdir -p lib/build-serial
	$(QMAKE) $(QMAKE_ARGS) -o lib/build-serial/ lib/serial
	$(MAKE) -C lib/build-serial/

.PHONY: qcustomplot
qcustomplot: lib/qcustomplot/src
	sed -i -e 's/qmake474/$(QMAKE)/' lib/qcustomplot/release.py
	cd lib/qcustomplot && ./release.py

lib/serial/src: gitmodules
lib/qcustomplot/src: gitmodules

.PHONY: gitmodules
gitmodules:
	$(GIT) submodule init
	$(GIT) submodule update
