package main

import (
	"flag"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"assembler/emitter"
	"assembler/lexer"
	"assembler/linker"
	"assembler/parser"
	"assembler/resolver"
)

func main() {
	var input, output string
	var debug bool

	flag.StringVar(&input, "input", "", "Path to .nova source")
	flag.StringVar(&output, "output", "", "Path to output .bin file")
	flag.BoolVar(&debug, "debug", false, "Enable debug output")
	flag.Parse()

	if input == "" {
		fmt.Fprintln(os.Stderr, "Missing -input flag")
		os.Exit(1)
	}

	linesRaw, err := os.ReadFile(input)
	if err != nil {
		panic(err)
	}
	lines := strings.Split(string(linesRaw), "\n")

	tokens, err := lexer.Lex(lines)
	if err != nil {
		panic(err)
	}

	ast, err := parser.Parse(tokens)
	if err != nil {
		panic(err)
	}

	baseDir := filepath.Dir(input)
	includeLoader := linker.NewIncludeLoader(baseDir)

	linked, err := linker.Link(ast, includeLoader)
	if err != nil {
		panic(err)
	}

	resolved, err := resolver.Resolve(linked)
	if err != nil {
		panic(err)
	}

	bytecode, err := emitter.Emit(resolved)
	if err != nil {
		panic(err)
	}

	entry := resolved.EntryPoint
	codesize := len(bytecode)

	header := []byte{
		0xDE,                                          // MAGIC_1
		0xAD,                                          // MAGIC_2
		0x02,                                          // VERSION
		byte(entry & 0xFF), byte((entry >> 8) & 0xFF), // ENTRY
		byte(codesize & 0xFF), byte((codesize >> 8) & 0xFF), // CODESIZE
		0x00, 0x00, 0x00, // RESERVED
	}

	out := append(header, bytecode...)

	if output == "" {
		output = "program.bin"
	}

	err = os.WriteFile(output, out, 0644)
	if err != nil {
		panic(err)
	}

	if debug {
		fmt.Println("\n\033[1;34m=== Nova Assembler Output ===\033[0m")
		fmt.Printf("Entry Point: \033[1;33m0x%04X\033[0m\n", entry)
		fmt.Printf("Code Size:   \033[1;33m%d bytes\033[0m\n", codesize)
		fmt.Println("\n\033[1;36mBytecode (hex):\033[0m")
		printHexDump(out)
		fmt.Printf("\n\033[1;32m✓ Assembled successfully to %s\033[0m\n", output)
	}

}
func printHexDump(data []byte) {
	for i := 0; i < len(data); i += 16 {
		end := i + 16
		if end > len(data) {
			end = len(data)
		}
		fmt.Printf("%04X  ", i)
		for j := i; j < end; j++ {
			fmt.Printf("%02X ", data[j])
		}
		fmt.Println()
	}
}
