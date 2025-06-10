package regs

type Register byte

const (
	R0 Register = 0x00
	R1 Register = 0x01
	R2 Register = 0x02
	R3 Register = 0x03
)

var NameToRegister = map[string]Register{
	"R0": R0,
	"R1": R1,
	"R2": R2,
	"R3": R3,
}
