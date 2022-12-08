package main

//export onStart
func onStart() uint {
	return 0;
}

//export onStop
func onStop() uint {
	return 0;
}

//export onRequest
func onRequest(req_pos uint, req_sz uint) (errno uint, res_pos uint, res_sz uint) {
	return 0, 10, 23
}

func main() {
	// do nothing
}
