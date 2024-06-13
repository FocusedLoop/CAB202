; Configures TCA0 to drive buzzer (see Ex 3.6)
ldi r16, 0x13
sts 0x0A01, r16      ; SINGLESLOPE mode, WO0 enabled
ldi r16, 0x40
sts 0x0A27, r16      ; ~200 Hz   
ldi r16, 0x20
sts 0x0A29, r16      ; 50% duty   
ldi r16, 0x01
sts 0x0A00, r16      ; Enable
; End TCA0 configuration

