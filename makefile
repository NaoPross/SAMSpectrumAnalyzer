# tools
GIT := git
QMAKE := qmake-qt5 -makefile -o Makefile -Wall "CONFIG+=debug" "CONFIG+=qml_debug"

# targets
all: desktop

run: desktop
	LD_LIBRARY_PATH=build-desktop ./build-desktop/SpectrumAnalyzer

.PHONY: desktop
desktop: src-desktop serial
	mkdir -p build-desktop
	$(QMAKE) -o build-desktop/ src-desktop
	$(MAKE) -C build-desktop
	cp lib/build-serial/libserial.so* build-desktop

.PHONY: clean
clean:
	rm -rf build-desktop
	rm -rf lib/build-serial

# libraries
.PHONY: serial
serial: lib/serial/src
	mkdir -p lib/build-serial
	$(QMAKE) -o lib/build-serial/ lib/serial
	$(MAKE) -C lib/build-serial/

lib/serial/src:
	$(GIT) submodule init
	$(GIT) submodule update
