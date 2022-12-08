package main

import "fmt"

//export onStart
func onStart() uint {
	return 0
}

//export onStop
func onStop() uint {
	return 0
}

//export onRequest
func onRequest(req_pos uint, req_sz uint) (errno uint, res_pos uint, res_sz uint) {
	return 0, 10, 23
}

// TBD: resturn a string
//
//export hi
func hi() string {
	return "this is .wasm"
}

// TBD: as WASI reactor
func main() {
	// do nothing
	fmt.Println(hi())
}
