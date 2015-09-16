#!/usr/bin/python
# -*- python -*-
# -*- coding: utf-8 -*-
# vim:filetype=python:
# Create CMakeLists.txt template for leaf directories
# (c) Roberto Di Remigio  <roberto.d.remigio@uit.no>
# licensed under the GNU Lesser General Public License

import os
import sys
import glob

sys.path.append('cmake/lib/docopt')
import docopt

options = """
Usage:
  ./setup.py [options] [<builddir>]
  ./setup.py (-h | --help)

Options:
  --libname=<LIBNAME>                    Name of the library to be created [default: ''].
  --lang=<LANGUAGE>                      Source file language <CXX/C/F> [default: CXX].
  -h --help                              Show this screen.
"""

def glob_sources_cxx(dir_name):
    """Create a list of C++ headers and sources to be used in a CMakeLists.txt file."""
    headers = 'list(APPEND headers_list '
    headers += ' '.join('%s' % ''.join(map(str, os.path.basename(x))) for x in sorted(glob.glob(dir_name + '/*.hpp')))
    headers += ')\n\n'

    sources = 'list(APPEND sources_list '
    sources += ' '.join('%s' % ''.join(map(str, os.path.basename(x))) for x in sorted(glob.glob(dir_name + '/*.cpp')))
    sources += ')\n\n'

    message = '# List of headers\n' + headers \
            + '# List of sources\n' + sources
    return message

def glob_sources_c(dir_name):
    """Create a list of C headers and sources to be used in a CMakeLists.txt file."""
    headers = 'list(APPEND headers_list '
    headers += ' '.join('%s' % ''.join(map(str, os.path.basename(x))) for x in sorted(glob.glob(dir_name + '/*.h')))
    headers += ')\n\n'

    sources = 'list(APPEND sources_list '
    sources += ' '.join('%s' % ''.join(map(str, os.path.basename(x))) for x in sorted(glob.glob(dir_name + '/*.c')))
    sources += ')\n\n'

    message = '# List of headers\n' + headers \
            + '# List of sources\n' + sources
    return message

def glob_sources_fortran(dir_name):
    """Create a list of Fortran sources to be used in a CMakeLists.txt file."""
    types = ('*.f', '*.F', '*.f77', '*.F77', '*.f90', '*.F90')
    list_of_sources = []
    for ftype in types:
        list_of_sources.extend(sorted(glob.glob(dir_name + '/' + ftype)))
    sources = 'list(APPEND sources_list '
    sources += ' '.join('%s' % ''.join(map(str, os.path.basename(x))) for x in list_of_sources)
    sources += ')\n\n'

    message = '# List of sources\n' + sources
    return message

try:
    arguments = docopt.docopt(options, argv=None)
except docopt.DocoptExit:
    sys.stderr.write('ERROR: bad input to %s\n' % sys.argv[0])
    sys.stderr.write(options)
    sys.exit(-1)

# Grab command-line arguments
libname = arguments['--libname']
lang    = arguments['--lang']

root_directory = os.getcwd()
dname = os.path.join(root_directory, libname)
fname = os.path.join(dname, 'CMakeLists.txt.try')
f = open(fname, 'w')
if (lang == 'CXX'):
    f.write(glob_sources_cxx(dname))
elif (lang == 'C'):
    f.write(glob_sources_c(dname))
else:
    f.write(glob_sources_fortran(dname))

f.write('# Write list of files to be passed to cloc for counting lines of code.\n')
f.write('# Only files that are actually compiled are counted.\n')
f.write('set(to_count "${sources_list}" "${headers_list}")\n')
f.write('write_to_cloc_list("${to_count}")\n\n')

f.write('# Build static library\n')
f.write('add_library('+ libname + ' STATIC ${sources_list})\n')
f.write('# Specify dependencies for the library (if any)\n')
f.write('#add_dependencies('+ libname + ' )\n')
f.write('set_property(GLOBAL APPEND PROPERTY PCMSolver_LIBRARIES {0})\n'.format(libname))
if (not (lang == 'C' or lang == 'F')):
    f.write('if(BUILD_CUSTOM_BOOST)\n')
    f.write('   add_dependencies('+ libname + ' custom_boost)\n')
    f.write('endif()\n')
f.write('install(TARGETS ' + libname + ' ARCHIVE DESTINATION lib)\n\n')

if (not lang == 'F'):
    f.write('set_property(GLOBAL APPEND PROPERTY PCMSolver_HEADER_DIRS ${{CMAKE_CURRENT_LIST_DIR}})\n')
    f.write('# Sets install directory for all the headers in the list\n')
    f.write('foreach(_header ${headers_list})\n')
    f.write('   install(FILES ${_header} DESTINATION include/' + libname + ')\n')
    f.write('endforeach()\n')

print('Template for {} created'.format(libname))
print('Don\'t forget to fix excluded files and dependencies!!!')

# vim:et:ts=4:sw=4
