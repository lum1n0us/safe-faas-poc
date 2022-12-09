package main

/*
#cgo CFLAGS: -g -Wall -I${SRCDIR}/../../library/include/
#cgo LDFLAGS: -L${SRCDIR}/../../build/ -L${SRCDIR}/../../build/external/wasm-micro-runtime/
#cgo LDFLAGS: -l:libwamr.a -l:libvmlib.a -lm
#include <stdlib.h>
#include "glue.h"
*/
import "C"
import (
	"fmt"
	"strings"
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
		panic("Glue return an un-expected string")
	}
}

func echoFromWAMR() {
	name := C.CString("Go App")
	defer C.free(unsafe.Pointer(name))

	out := C.malloc(C.sizeof_char * 128)
	defer C.free(unsafe.Pointer(out))

	size := C.echo_from_wamr(name, (*C.char)(out))

	b := C.GoBytes(out, size)

	fmt.Println("Hi Glue, this is Go App")
	fmt.Println(string(b))

	if !strings.HasPrefix(string(b), "Hi Go App, this is WAMR") {
		panic("WARM return an un-expected string")
	}
}

func echoFromWASM() {
	name := C.CString("Go App")
	defer C.free(unsafe.Pointer(name))

	out := C.malloc(C.sizeof_char * 128)
	defer C.free(unsafe.Pointer(out))

	size := C.echo_from_wasm(name, (*C.char)(out))
	if size == 0 {
		panic("echo_from_wasm failed")
	}

	b := C.GoBytes(out, size)

	fmt.Println("Hi Glue, this is Go App")
	fmt.Println(string(b))

	if !strings.HasPrefix(string(b), "Hi Go App, this is .wasm") {
		panic("WASM return an un-expected string")
	}
}

func main() {
	echoFromGlue()
	echoFromWAMR()
	echoFromWASM()
}
