# Harry - A Tool for Measuring String Similarity
# Copyright (C) 2013 Konrad Rieck (konrad@mlsec.org)

AUTOMAKE_OPTIONS = foreign
SUBDIRS = src doc examples python . tests

ACLOCAL_AMFLAGS = -I m4
EXTRA_DIST = README.md COPYING CHANGES harry.png

clean-local:
	rm -f *~
	rm -f CHANGES

CHANGES:
	$(srcdir)/git2changes.py

