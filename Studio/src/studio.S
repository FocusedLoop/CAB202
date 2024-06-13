.section init0

entry:
;    ldi r18, 32        //1
;    sts 0x0420, r16    //2
;    sbi 0x4, 5         //1

;32 = 0x20 = 0b0010 0000 //would not have space in real code, this is a bit field
lds r17, 0x0420
//ldi r16, 0b00100000
ori r17, 0b00100000
//or r16, r17
lds r17, 0x0420
sts 0x0420, r17
andi r17, 0b11011111

loop:
    rjmp loop