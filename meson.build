project('pl32lib', 'c',
		version : '4.0.0',
		license : 'LGPL-2.1-or-later')

inc = include_directories('include')

subdir('include')
subdir('src')

pkg_mod = import('pkgconfig')
pkg_mod.generate(libraries : pl32lib,
				version : '4.0',
				name : 'pl32lib',
				filebase : 'pl32',
				description : 'A library containing all portable routines/algorithms I\'ve ever implemented in any program')

## Tests
testexe = executable('pl32-test.out', 'pl32-test.c',
					include_directories : inc,
					link_with : pl32lib)
test('Check Version', testexe, args : ['version'])
test('Show Memory Usage', testexe, args : ['show-memusg'])
test('Parser', testexe, args : ['parser-test'])
test('Memory Allocation', testexe, args : ['memory-test'])
test('File Reading', testexe, args : ['file-test'])