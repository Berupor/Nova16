package linker

import (
	"fmt"
	"nova_asm/lexer"
	"nova_asm/parser"
	"os"
	"path/filepath"
	"strings"
)

type LinkedAST struct {
	Nodes      []parser.ASTNode
	EntryLabel string // "main"
}

func Link(ast parser.AST, includeLoader func(path string) ([]string, error)) (LinkedAST, error) {
	var linked LinkedAST
	var result []parser.ASTNode

	for _, node := range ast.Nodes {
		if node.Type == parser.NodeDirective && node.Directive != nil {
			switch strings.ToUpper(node.Directive.Name) {
			case "INCLUDE":
				lines, err := includeLoader(node.Directive.Target)
				if err != nil {
					return LinkedAST{}, fmt.Errorf("failed to include %q: %w", node.Directive.Target, err)
				}

				// lexer + parser
				tokens, err := lexer.Lex(lines)
				if err != nil {
					return LinkedAST{}, fmt.Errorf("lex error in %s: %w", node.Directive.Target, err)
				}
				subAST, err := parser.Parse(tokens)
				if err != nil {
					return LinkedAST{}, fmt.Errorf("parse error in %s: %w", node.Directive.Target, err)
				}

				result = append(result, subAST.Nodes...)
			case "ENTRY":
				linked.EntryLabel = node.Directive.Target
			default:
				// unkown directive
			}
		} else {
			result = append(result, node)
		}
	}

	linked.Nodes = result
	return linked, nil
}

func NewIncludeLoader(baseDir string) func(string) ([]string, error) {
	return func(path string) ([]string, error) {
		fullPath := filepath.Join(baseDir, path)
		raw, err := os.ReadFile(fullPath)
		if err != nil {
			return nil, err
		}
		return strings.Split(string(raw), "\n"), nil
	}
}
