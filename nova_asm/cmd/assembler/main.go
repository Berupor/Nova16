package main

import (
	"flag"
	"fmt"
	"path/filepath"

	"nova_asm/emitter"
	"nova_asm/lexer"
	"nova_asm/linker"
	"nova_asm/parser"
	"nova_asm/resolver"

	"os"
	"strings"
)

func main() {
	var input, output string
	flag.StringVar(&input, "input", "", "Path to .nova source")
	flag.StringVar(&output, "output", "", "Path to output .bin file")
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

	fmt.Println("Tokens:")
	fmt.Println(tokens)

	ast, err := parser.Parse(tokens)
	if err != nil {
		panic(err)
	}
	fmt.Println("AST:")
	fmt.Println(ast)

	baseDir := filepath.Dir(input)
	includeLoader := linker.NewIncludeLoader(baseDir)

	linked, err := linker.Link(ast, includeLoader)

	if err != nil {
		panic(err)
	}

	fmt.Println("Linked:")
	fmt.Println(linked)

	resolved, err := resolver.Resolve(linked)

	if err != nil {
		panic(err)
	}

	fmt.Println("Resolved:")
	fmt.Println(resolved)

	bytecode, err := emitter.Emit(resolved)
	if err != nil {
		panic(err)
	}

	fmt.Println("Bytecode:")
	fmt.Println(bytecode)

	entry := resolved.EntryPoint
	codesize := len(bytecode)

	header := []byte{
		0xDE,                                          // MAGIC_1
		0xAD,                                          // MAGIC_2
		0x02,                                          // VERSION
		byte(entry & 0xFF), byte((entry >> 8) & 0xFF), // ENTRY (lo, hi)
		byte(codesize & 0xFF), byte((codesize >> 8) & 0xFF), // CODESIZE (lo, hi)
		0x00, 0x00, 0x00, // RESERVED
	}

	out := append(header, bytecode...)

	fmt.Println("Out (hex):")
	printHexDump(out)

	fmt.Printf("Entry: 0x%04X\n", resolved.EntryPoint)
	for k, v := range resolved.Symbols {
		fmt.Printf("Label: %-10s Addr: 0x%04X\n", k, v)
	}

	if output == "" {
		output = "program.bin"
	}

	err = os.WriteFile(output, out, 0644)
	if err != nil {
		panic(err)
	}

	fmt.Printf("Wrote %d bytes to %s (entry: 0x%04X)\n", len(out), output, entry)

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
