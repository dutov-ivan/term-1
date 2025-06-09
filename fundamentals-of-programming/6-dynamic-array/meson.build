project('op-lab-6', 'c')

c_args = ['-Isrc', '-g' ]
include_dirs = include_directories('src')

main_sources = [
  'src/main.c',
  'src/algorithm.c',
  'src/io/int.c',
  'src/io/double.c',
  'src/io/validators.c',
  'src/io/utils.c',
  'src/io/equations.c',
]

executable('main.out', main_sources,
  c_args: c_args,
  include_directories: include_dirs,
  link_args: ['-lm']
)

