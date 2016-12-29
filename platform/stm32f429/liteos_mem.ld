/*
 * memory definitions
 */

MEMORY
{
	ROM    (rx)  : ORIGIN = 0x08000000, LENGTH = 2048K
	RAM    (rwx) : ORIGIN = 0x20000400, LENGTH = 256K
	RAM1   (rwx) : ORIGIN = 0x10000000, LENGTH = 64K
	VECTOR (rwx) : ORIGIN = 0x20000000, LENGTH = 1K
}
