package resolver

import (
	"assembler/codes"
	"assembler/linker"
	"assembler/parser"
	"fmt"
	"strconv"
	"strings"
)

type ArgType int

const (
	ArgRegister ArgType = iota
	ArgImmediate
	ArgAddress
)

type Argument struct {
	Type  ArgType
	Value string
}

type Instruction struct {
	Opcode string
	Args   []Argument
	Line   int
}

type ResolvedNode struct {
	Type  parser.NodeType
	Inst  *Instruction
	Label *parser.Label
	Line  int
}

type Resolved struct {
	Nodes      []ResolvedNode
	EntryPoint int
	Symbols    map[string]int
}

func Resolve(linked linker.LinkedAST) (Resolved, error) {
	var out Resolved
	out.Symbols = map[string]int{}
	var resolved []ResolvedNode

	pc := 0 // TEXT_START

	for _, node := range linked.Nodes {
		switch node.Type {
		case parser.NodeLabel:
			label := node.Label.Name
			if _, exists := out.Symbols[label]; exists {
				return Resolved{}, fmt.Errorf("duplicate label %q", label)
			}
			out.Symbols[label] = pc

			resolved = append(resolved, ResolvedNode{
				Type:  parser.NodeLabel,
				Label: node.Label,
				Line:  node.Line,
			})

		case parser.NodeInstruction:
			opcode := strings.ToUpper(node.Inst.Opcode)
			code, ok := codes.OpcodeMap[opcode]
			if !ok {
				return Resolved{}, fmt.Errorf("unknown opcode %q at line %d", opcode, node.Line)
			}
			pc += code.Size()

			var args []Argument
			for _, raw := range node.Inst.Args {
				arg, err := classifyArg(raw, out.Symbols)
				if err != nil {
					return Resolved{}, fmt.Errorf("line %d: %v", node.Line, err)
				}
				args = append(args, arg)
			}
			resolved = append(resolved, ResolvedNode{
				Type: node.Type,
				Inst: &Instruction{
					Opcode: node.Inst.Opcode,
					Args:   args,
					Line:   node.Line,
				},
				Line: node.Line,
			})
		}
	}

	entry, ok := out.Symbols[linked.EntryLabel]
	if !ok {
		return Resolved{}, fmt.Errorf("entry label %q not found", linked.EntryLabel)
	}
	out.EntryPoint = entry

	out.Nodes = resolved
	return out, nil
}

func classifyArg(s string, symbols map[string]int) (Argument, error) {
	if strings.HasPrefix(s, "R") {
		return Argument{Type: ArgRegister, Value: s}, nil
	} else if isNumber(s) {
		return Argument{Type: ArgImmediate, Value: s}, nil
	} else if addr, ok := symbols[s]; ok {
		return Argument{Type: ArgAddress, Value: fmt.Sprintf("%d", addr)}, nil
	} else {
		return Argument{}, fmt.Errorf("unknown symbol or invalid arg: %q", s)
	}
}

func isNumber(s string) bool {

	_, err := strconv.ParseInt(s, 0, 64)
	return err == nil
}
