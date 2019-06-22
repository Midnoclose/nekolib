find ./src -name \*.h\* -print -o -name \*.c\* -print | xargs clang-format -style='{BasedOnStyle: Google, Language: Cpp, IndentWidth: 4}' -i
find ./include -name \*.h\* -print -o -name \*.c\* -print | xargs clang-format -style='{BasedOnStyle: Google, Language: Cpp, IndentWidth: 4}' -i
