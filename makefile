all: obj taskmonitor

# From StackOverflow: https://stackoverflow.com/questions/41568508/makefile-compile-multiple-c-file-at-once

obj:
clean:

LIBS= -fopenmp -lpthread -lgtk-4 -lpangocairo-1.0 -lpango-1.0 -lharfbuzz -lgdk_pixbuf-2.0 -lcairo-gobject -lcairo -lgraphene-1.0 -lgio-2.0 -lgobject-2.0 -lglib-2.0  -mfpmath=sse -msse -msse2
INCLUDE=-I/usr/include/gtk-4.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/glib-2.0/include -I/usr/include/sysprof-4 -I/usr/include/harfbuzz -I/usr/include/freetype2 -I/usr/include/libpng16 -I/usr/include/libmount -I/usr/include/blkid -I/usr/include/fribidi -I/usr/include/cairo -I/usr/include/lzo -I/usr/include/pixman-1 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/graphene-1.0 -I/usr/lib/graphene-1.0/include -I/usr/include/gio-unix-2.0
src_root := src
src_subdirs := 
build_root := build

o_suffix := .o

# Build list of sources. Iterate every subfolder from $(src_subdirs) list 
# and fetch all existing files with suffixes matching the list.
source_suffixes := .c
sources := $(foreach d,$(addprefix $(src_root)/,$(src_subdirs)),$(wildcard $(addprefix $d/*,$(source_suffixes))))

# If src_subdirs make variable is unset, use 'find' command to build list of sources.
# Note that we use the same list of suffixes but tweak them for use with 'find'
ifeq ($(src_subdirs),)
  sources := $(shell find $(src_root) -type f $(foreach s,$(source_suffixes),$(if $(findstring $s,$(firstword $(source_suffixes))),,-o) -name '*$s'))
endif

$(info sources=$(sources))

# Build source -> object file mapping.
# We want map $(src_root) -> $(build_root) and copy directory structure 
# of source tree but populated with object files.
objects := $(addsuffix $(o_suffix),$(basename $(patsubst $(src_root)%,$(build_root)%,$(sources))))
$(info objects=$(objects))

# Generate rules for every .o file to depend exactly on corresponding source file.
$(foreach s,$(sources),$(foreach o,$(filter %$(basename $(notdir $s)).o,$(objects)),$(info New rule: $o: $s)$(eval $o: $s)))

# This is how we compile sources:
# First check if directory for the target file exists. 
# If it doesn't run 'mkdir' command.
$(objects): ; $(if $(wildcard $(@D)),,mkdir -p $(@D) &&) gcc -g -c $< $(LIBS) $(INCLUDE) -o $@

obj: $(objects)
clean: ; rm -rf $(build_root)

taskmonitor:
	gcc -g -o build/taskmonitor $(objects) -lm $(LIBS) $(INCLUDE)