project('op-lab-5', 'c')

c_args = ['-Isrc', '-g' ]
include_dirs = include_directories('src')

main_sources = [
  'src/main.c',
  'src/options.c',
  'src/input/int.c',
  'src/input/string.c',
  'src/input/utils.c',
  'src/string/random.c',
  'src/string/sort.c'
]

executable('main.out', main_sources,
  c_args: c_args,
  include_directories: include_dirs,
  link_args: []
)
