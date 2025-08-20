import os
import re
import sys




def main():
    main_path: str = os.path.join(os.getcwd(), sys.argv[1])
    output_path: str = os.path.join(os.getcwd(), 'main.cpp')

    with open(output_path, 'w+') as output_file:
        imported_paths = set()

        def resolve_imports(path: str):
            if path in imported_paths:
                return
            if not os.path.isfile(path):
                print(path, 'not found')
                return
            imported_paths.add(path)
            with open(path, 'r') as input_file:
                for line in input_file.readlines():
                    if line == '#pragma once\n':
                        continue
                    elif match := re.match(r'#include "(.+)"', line):
                        include_path = os.path.join(os.getcwd(), 'src', match.group(1))
                        resolve_imports(include_path)
                    else:
                        output_file.write(line)

        resolve_imports(main_path)


if __name__ == "__main__":
    main()