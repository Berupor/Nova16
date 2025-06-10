package codes

type Opcode byte

const (
	MOV     Opcode = 0x01
	MOVI    Opcode = 0x02
	PUSH    Opcode = 0x10
	POP     Opcode = 0x11
	ADD     Opcode = 0x20
	SUB     Opcode = 0x21
	INC     Opcode = 0x22
	DEC     Opcode = 0x23
	CMP     Opcode = 0x30
	JMP     Opcode = 0x40
	JZ      Opcode = 0x41
	CALL    Opcode = 0x50
	RET     Opcode = 0x51
	SYSCALL Opcode = 0x70
	LOAD    Opcode = 0x60
	STORE   Opcode = 0x61
	NOP     Opcode = 0xF0
	HLT     Opcode = 0xFF
)

var OpcodeMap = map[string]Opcode{
	"MOV":     MOV,
	"MOVI":    MOVI,
	"PUSH":    PUSH,
	"POP":     POP,
	"ADD":     ADD,
	"SUB":     SUB,
	"INC":     INC,
	"DEC":     DEC,
	"CMP":     CMP,
	"JMP":     JMP,
	"JZ":      JZ,
	"CALL":    CALL,
	"RET":     RET,
	"SYSCALL": SYSCALL,
	"LOAD":    LOAD,
	"STORE":   STORE,
	"NOP":     NOP,
	"HLT":     HLT,
}

// Size returns opcode size (opcode + operands)
func (o Opcode) Size() int {
	switch o {
	case RET, NOP, HLT, SYSCALL:
		return 1
	case PUSH, POP, INC, DEC:
		return 2
	case MOV, ADD, SUB, CMP, MOVI, JMP, JZ, CALL:
		return 3
	case LOAD, STORE:
		return 4
	default:
		return 0
	}
}
