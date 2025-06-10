package emitter

import (
	"assembler/codes"
	"assembler/parser"
	"assembler/resolver"
	"fmt"
	"strconv"
)

func Emit(res resolver.Resolved) ([]byte, error) {
	var code []byte

	for _, node := range res.Nodes {
		if node.Type != parser.NodeInstruction {
			continue
		}
		inst := node.Inst
		op, ok := codes.OpcodeMap[inst.Opcode]
		if !ok {
			return nil, fmt.Errorf("unknown opcode %q at line %d", inst.Opcode, inst.Line)
		}

		expectedSize := op.Size() // opcode + args
		actualSize := 1

		for _, arg := range inst.Args {
			switch arg.Type {
			case resolver.ArgRegister, resolver.ArgImmediate:
				actualSize += 1
			case resolver.ArgAddress:
				actualSize += 2
			}
		}

		if actualSize != expectedSize {
			return nil, fmt.Errorf("instruction %s at line %d: expected %d bytes, got %d",
				inst.Opcode, inst.Line, expectedSize, actualSize)
		}

		code = append(code, byte(op))

		for _, arg := range inst.Args {
			switch arg.Type {
			case resolver.ArgRegister:
				reg := parseRegister(arg.Value)
				if reg < 0 {
					return nil, fmt.Errorf("invalid register %q", arg.Value)
				}
				code = append(code, byte(reg))

			case resolver.ArgImmediate:
				val, err := parseNumber(arg.Value)
				if err != nil {
					return nil, fmt.Errorf("invalid immediate %q", arg.Value)
				}
				code = append(code, byte(val))

			case resolver.ArgAddress:
				val, err := parseNumber(arg.Value)
				if err != nil {
					return nil, fmt.Errorf("invalid address %q", arg.Value)
				}
				// 16-bit little-endian
				code = append(code, byte(val&0xFF), byte((val>>8)&0xFF))
			}
		}
	}

	return code, nil
}

func parseRegister(s string) int {
	if len(s) != 2 || s[0] != 'R' || s[1] < '0' || s[1] > '3' {
		return -1
	}
	return int(s[1] - '0')
}

func parseNumber(s string) (int, error) {
	val, err := strconv.ParseInt(s, 0, 16)
	if err != nil {
		return 0, err
	}
	return int(val), nil
}
