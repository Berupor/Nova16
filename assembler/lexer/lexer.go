package lexer

import (
	"strconv"
	"strings"
)

type TokenKind int

const (
	TKOpcode TokenKind = iota
	TKRegister
	TKNumber
	TKLabel
	TKDirective // INCLUDE, ENTRY
)

type Token struct {
	Kind  TokenKind
	Value string
	Line  int
}

func isNumber(s string) bool {
	// strconv.ParseInt understands: "123", "0xFF", "0755" etc
	_, err := strconv.ParseInt(s, 0, 64)
	return err == nil
}

func Lex(lines []string) ([]Token, error) {
	var tokens []Token

	for lineIdx, line := range lines {
		line = strings.TrimSpace(line)
		if line == "" || strings.HasPrefix(line, ";") {
			continue // skip comment
		}

		parts := strings.Fields(line)
		for _, part := range parts {
			part = strings.Trim(part, ",")
			switch {
			case strings.HasSuffix(part, ":"):
				tokens = append(tokens, Token{Kind: TKLabel, Value: strings.TrimSuffix(part, ":"), Line: lineIdx + 1})
			case strings.ToUpper(part) == "INCLUDE" || strings.ToUpper(part) == "ENTRY":
				tokens = append(tokens, Token{Kind: TKDirective, Value: part, Line: lineIdx + 1})
			case strings.HasPrefix(part, "R") && len(part) == 2:
				tokens = append(tokens, Token{Kind: TKRegister, Value: part, Line: lineIdx + 1})
			case isNumber(part):
				tokens = append(tokens, Token{Kind: TKNumber, Value: part, Line: lineIdx + 1})
			default:
				tokens = append(tokens, Token{Kind: TKOpcode, Value: part, Line: lineIdx + 1})
			}
		}
	}
	return tokens, nil

}
