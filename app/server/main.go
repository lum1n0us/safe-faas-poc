package main

/*
#cgo CFLAGS: -g -Wall -I${SRCDIR}/../../library/include/
#cgo LDFLAGS: -L${SRCDIR}/../../build/
#cgo LDFLAGS: -l:libwamr.a
#include <stdlib.h>
#include "glue.h"
*/
import "C"
import (
	"fmt"
	"unsafe"
)

func echoFromGlue() {
	name := C.CString("Go App")
	defer C.free(unsafe.Pointer(name))

	out := C.malloc(C.sizeof_char * 128)
	defer C.free(unsafe.Pointer(out))

	size := C.echo_from_glue(name, (*C.char)(out))

	b := C.GoBytes(out, size)

	fmt.Println("Hi Glue, this is Go App")
	fmt.Println(string(b))

	if string(b) != "Hi Go App, this is glue" {
		panic("Glue return an expected string")
	}

}

func echoFromWAMR() {
	fmt.Println("TBD: talk with wamr")
}

func echoFromWASM() {
	fmt.Println("TBD: talk with .wasm")
}

func main() {
	echoFromGlue()
}
