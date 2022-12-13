## Steps

- Open the folder in vsocde
- Choose "Dev Containers: Reopen in container"
- In VsCode
  - run `cmake -S . -B build`
  - run `cmake --build build`
  - run `ctest -V --test-dir build`

## TODOs

> `find . -type f -not -path "./.git/*" -not -path "./external/wasm-micro-runtime/*" -not -path "./build/*" -not -path "./README.md" | xargs grep "\<TBD\>" | sort`

- [ ] use Golang to write glue
- [ ] write .wasm with Golang and control its exported items
- [ ] write .wasm with Golang and passing strings
- [x] access .wasm in EGo
- [x] use `mounts` instead of `files` in ego configration
- [x] is there a way to call mmap() (wasm_runtime_instantiate)
- [ ] is there a way to resolve _init fd table failed?_
- [ ] test WASI interfaces or WASI from glue?
- [x] ./app/server/main.go: fmt.Println("TBD: talk with wamr")
- [x] ./app/server/main.go: fmt.Println("TBD: talk with .wasm")
- [x] ./app/server/main.go:// TBD: How to access .wasm in enclave
- [ ] ./app/wasm/main.go:// TBD: resturn a string
- [ ] ./app/wasm/main.go:// TBD: as WASI reactor
- [x] ./cmake/FindEGo.cmake:message(STATUS "TBD: seach ego")
- [x] ./cmake/FindGo.cmake:message(STATUS "TBD: seach go")
- [ ] ./cmake/FindOpenEnclave.cmake:message(STATUS "TBD: seach OpenEnclave")
- [ ] ./cmake/FindSGX.cmake:message(STATUS "TBD: seach SGX")
- [x] ./cmake/FindTinyGo.cmake:message(STATUS "TBD: seach tinygo")
- [x] ./cmake/FindWABT.cmake:message(STATUS "TBD: seach WABT")
- [x] ./cmake/FindWASISDK.cmake:message(STATUS "TBD: seach WASI SDK")
- [ ] ./library/src/glue.c: // TBD: replace with go_faas.wasm
- [ ] shall I dynamically generate the value of `mounts.source` in _config.json_?
