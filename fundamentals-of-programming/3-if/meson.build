project('op-lab-3', 'c')

# Compiler flags and include directories
c_args = ['-I.', '-Iunity/src', '-Isrc', '-g', '-lm']
inc_dirs = include_directories('.', 'unity/src', 'src')

# Main program sources
main_sources = [
  'src/input/input.c',
  'src/root/root.c',
  'src/main.c'
]

# Test program sources
test_sources = [
  'test/input/check_input_test.c',
  'test/input/custom_input_test.c',
  'test/root/root_test.c',
  'test/test.c',
  'unity/src/unity.c',
  'src/input/input.c',
  'src/root/root.c',
]

# Define the main executable
executable('main.out', main_sources,
  c_args: c_args,
  include_directories: inc_dirs,
  link_args: ['-lm']
)

# Define the test executable
executable('test.out', test_sources,
  c_args: c_args,
  include_directories: inc_dirs,
  link_args: ['-lm']
)

