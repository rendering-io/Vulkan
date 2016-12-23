#! /usr/bin/python
from __future__ import print_function
import sys
from sh import grep, ErrorReturnCode_1

def extract_function_name(line):
  start = line.find('VKAPI_CALL ')
  if -1 == start:
    return None

  end = line.find('(', start + 11)
  return line[start+11:end]

def scan_directory_for_function(dir, f):
  try:
    grep("-nr", f, dir)
  except ErrorReturnCode_1:
    print(f)

def main():
  with open('/usr/include/vulkan/vulkan.h') as f:
    lines = f.readlines()

  functions = map(extract_function_name, lines)
  functions = sorted(filter(lambda x: x != None, functions))

  directory = sys.argv[1]
  [scan_directory_for_function(directory, f) for f in functions]

if __name__ == "__main__":
    main()