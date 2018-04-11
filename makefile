# tools
ifeq ($(shell lsb_release -si),Debian)
QMAKE := QT_SELECT=qt5 qmake
else
QMAKE := qmake-qt5 
endif

GIT := git
QMAKE_ARGS := -makefile -o Makefile -Wall "CONFIG+=debug" "CONFIG+=qml_debug"

# targets
all: desktop

run: desktop
	LD_LIBRARY_PATH=build-desktop ./build-desktop/SpectrumAnalyzer

.PHONY: desktop
desktop: src-desktop serial qcustomplot
	mkdir -p build-desktop
	$(QMAKE) $(QMAKE_ARGS) -o build-desktop/ src-desktop
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
