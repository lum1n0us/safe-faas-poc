## Steps

- Open the folder in vsocde
- Choose "Dev Containers: Reopen in container"
- In VsCode
  - run `cmake -S . -B build`
  - run `cmake --build build`
  - run `ctest -V --test-dir build`

## TODOs

> `find . -type f -not -path "./.git/*" -not -path "./external/wasm-micro-runtime/*" -not -path "./build/*" -not -path "./README.md" | xargs grep "\<TBD\>"`

- [x] ./app/server/main.go: fmt.Println("TBD: talk with wamr")
- [ ] ./app/server/main.go: fmt.Println("TBD: talk with .wasm")
- [ ] ./cmake/FindOpenEnclave.cmake:message(STATUS "TBD: seach OpenEnclave")
- [ ] ./cmake/FindEGo.cmake:message(STATUS "TBD: seach ego")
- [ ] ./cmake/FindGo.cmake:message(STATUS "TBD: seach go")
- [ ] ./cmake/FindSGX.cmake:message(STATUS "TBD: seach SGX")
- [ ] ./cmake/FindTinyGo.cmake:message(STATUS "TBD: seach tinygo")
