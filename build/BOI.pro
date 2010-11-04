######################################################################
# Main BOI QMake build file
######################################################################

TEMPLATE = subdirs
CONFIG  += ordered

SUBDIRS = lib \
          app \
          ../plugins/BasicShapes/build \

