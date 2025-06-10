package parser

import (
	"assembler/lexer"
	"fmt"
	"strings"
)

type NodeType int

const (
	NodeInstruction NodeType = iota
	NodeLabel
	NodeDirective
)

type Instruction struct {
	Opcode string   // example: "MOV"
	Args   []string // example: ["R0", "R1"]
	Line   int
}
type Directive struct {
	Name   string // example: "ENTRY"
	Target string // example: "main"
}
type Label struct {
	Name string // example: "loop"
}

type ASTNode struct {
	Type      NodeType
	Inst      *Instruction
	Label     *Label
	Directive *Directive
	Line      int
}
type AST struct {
	Nodes []ASTNode
}

func Parse(tokens []lexer.Token) (AST, error) {
	var ast AST
	var currentLine int
	var lineTokens []lexer.Token

	flushLine := func() error {
		if len(lineTokens) == 0 {
			return nil
		}

		first := lineTokens[0]

		switch first.Kind {
		case lexer.TKLabel:
			ast.Nodes = append(ast.Nodes, ASTNode{
				Type:  NodeLabel,
				Label: &Label{Name: first.Value},
				Line:  first.Line,
			})
			return nil
		case lexer.TKDirective:
			if len(lineTokens) < 2 {
				return fmt.Errorf("directive %s on line %d missing target", first.Value, first.Line)
			}
			ast.Nodes = append(ast.Nodes, ASTNode{
				Type: NodeDirective,
				Directive: &Directive{
					Name:   first.Value,
					Target: stripQuotes(lineTokens[1].Value),
				},
				Line: first.Line,
			})
			return nil
		case lexer.TKOpcode:
			args := make([]string, 0)
			for _, tok := range lineTokens[1:] {
				args = append(args, tok.Value)
			}
			ast.Nodes = append(ast.Nodes, ASTNode{
				Type: NodeInstruction,
				Inst: &Instruction{
					Opcode: first.Value,
					Args:   args,
					Line:   first.Line,
				},
				Line: first.Line,
			})
			return nil
		default:
			return fmt.Errorf("unexpected token %v on line %d", first, first.Line)
		}
	}

	for _, token := range tokens {
		if currentLine == 0 {
			currentLine = token.Line
		}

		if token.Line != currentLine {
			if err := flushLine(); err != nil {
				return AST{}, err
			}
			lineTokens = nil
			currentLine = token.Line
		}
		lineTokens = append(lineTokens, token)
	}

	if err := flushLine(); err != nil {
		return AST{}, err
	}

	return ast, nil
}
func stripQuotes(s string) string {
	if strings.HasPrefix(s, "\"") && strings.HasSuffix(s, "\"") {
		return s[1 : len(s)-1]
	}
	return s
}
