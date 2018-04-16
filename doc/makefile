# Documents
JOURNAL := journal.tex
DOCUMENTATION := spectrum-analyzer.tex

# LaTeX compiler settings
BUILD_DIR := build
TEX := xelatex \
		-interaction=nonstopmode \
		-halt-on-error \
		-output-directory=$(BUILD_DIR)
		
# Automated
JOURNAL_OUT := $(patsubst %.tex,%.pdf,$(JOURNAL))
DOCUMENTATION_OUT := $(patsubst %.tex,%.pdf,$(DOCUMENTATION))

# Recipes
.PHONY: dir doc journal
all: doc journal
	cp $(BUILD_DIR)/$(JOURNAL_OUT) .
	cp $(BUILD_DIR)/$(DOCUMENTATION_OUT) .

doc: dir
	$(TEX) $(DOCUMENTATION)

journal: dir
	$(TEX) $(JOURNAL)

dir:
	mkdir -p $(BUILD_DIR)/tex
