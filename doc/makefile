# Documents
JOURNAL := journal.tex
DOCUMENTATION := spectrum-analyzer.tex

# LaTeX compiler settings
BUILD_DIR := build
TEX := xelatex \
		-shell-escape \
		-interaction=nonstopmode \
		-halt-on-error \
		-output-directory=$(BUILD_DIR)
		
# Automated
JOURNAL_OUT := $(patsubst %.tex,%.pdf,$(JOURNAL))
DOCUMENTATION_OUT := $(patsubst %.tex,%.pdf,$(DOCUMENTATION))

# Recipes
.PHONY: dir doc journal count
all: doc journal
	cp $(BUILD_DIR)/$(JOURNAL_OUT) .
	cp $(BUILD_DIR)/$(DOCUMENTATION_OUT) .

doc: dir
	$(MAKE) -C figures/uml/
	$(TEX) $(DOCUMENTATION)

journal: dir
	$(TEX) $(JOURNAL)

dir:
	mkdir -p $(BUILD_DIR)/tex $(BUILD_DIR)/tikz

count:
	texcount -inc -total -letters spectrum-analyzer.tex
