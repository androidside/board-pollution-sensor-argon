/* * Data converted from C:/cygwin64/home/Sauron/git/board_argon/projects/esp32/esp32-starter-master/main/files/sample.txt
 * (null byte appended)
 */
.data
.section .rodata.embedded

.global sample_txt
sample_txt:

.global _binary_sample_txt_start
_binary_sample_txt_start: /* for objcopy compatibility */
.byte 0x61, 0x20, 0x73, 0x61, 0x6d, 0x70, 0x6c, 0x65, 0x20, 0x74, 0x65, 0x78, 0x74, 0x00

.global _binary_sample_txt_end
_binary_sample_txt_end: /* for objcopy compatibility */


.global sample_txt_length
sample_txt_length: /* not including null byte */
.word 13
