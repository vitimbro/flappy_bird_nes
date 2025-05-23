; This file is for the FamiTone2 library and was generated by FamiStudio


.export _music_game = music_data_mega_man_2


music_data_mega_man_2:
	.byte 1
	.word @instruments
	.word @samples-3
; 00 : Dr. Wily's Castle
	.word @song0ch0
	.word @song0ch1
	.word @song0ch2
	.word @song0ch3
	.word @song0ch4
	.word 307,256

.export music_data_mega_man_2
.global FAMISTUDIO_DPCM_PTR

@instruments:
	.byte $30 ; 00 : WilyBass
	.word @env4, @env6, @env6
	.byte $00
	.byte $30 ; 01 : WilyBassPow
	.word @env4, @env10, @env6
	.byte $00
	.byte $30 ; 02 : WilyLeadDuty0
	.word @env7, @env6, @env6
	.byte $00
	.byte $30 ; 03 : WilyLeadDuty0Tremolo
	.word @env5, @env6, @env1
	.byte $00
	.byte $70 ; 04 : WilyLeadDuty1
	.word @env3, @env6, @env6
	.byte $00
	.byte $70 ; 05 : WilyLeadDuty1Alt
	.word @env5, @env6, @env6
	.byte $00
	.byte $70 ; 06 : WilyLeadDuty1Tremolo
	.word @env5, @env6, @env1
	.byte $00
	.byte $b0 ; 07 : WilyLeadDuty2
	.word @env2, @env6, @env6
	.byte $00
	.byte $f0 ; 08 : WilyLeadDuty3Tremolo
	.word @env9, @env6, @env1
	.byte $00
	.byte $30 ; 09 : WilyNoiseDrumHi
	.word @env8, @env6, @env6
	.byte $00
	.byte $30 ; 0a : WilyNoiseDrumLo
	.word @env0, @env6, @env6
	.byte $00

@env0:
	.byte $c8,$c8,$c5,$02,$c3,$02,$c1,$c0,$00,$07
@env1:
	.byte $c1,$c0,$bf,$c1,$00,$01
@env2:
	.byte $c9,$0e,$c7,$0e,$c6,$0e,$c5,$00,$06
@env3:
	.byte $c8,$02,$c7,$03,$c6,$03,$c5,$03,$c4,$03,$c3,$03,$c2,$00,$0c
@env4:
	.byte $cf,$7f,$00,$00
@env5:
	.byte $c8,$13,$c6,$13,$c4,$00,$04
@env6:
	.byte $c0,$7f,$00,$01
@env7:
	.byte $c7,$13,$c6,$13,$c5,$00,$04
@env8:
	.byte $c7,$c3,$c2,$c0,$00,$03
@env9:
	.byte $c9,$13,$c7,$13,$c5,$00,$04
@env10:
	.byte $c0,$c0,$be,$be,$bc,$bc,$bb,$00,$06

@samples:

@song0ch0:
	.byte $fb, $01
@song0ch0loop:
	.byte $88, $34
@song0ref6:
	.byte $8f, $00
@song0ref8:
	.byte $3a, $85, $00, $3a, $85, $00, $3a, $8f, $00, $3a, $85, $00, $3a, $85, $00, $3a, $8f, $00, $34, $8f, $00, $93, $34, $85
	.byte $00, $34, $85, $00, $3a, $8f, $00, $3a, $85, $00, $3a, $85, $00, $3a, $8f, $00, $34, $8f, $00, $93, $42, $8f, $00, $3e
	.byte $8f, $00, $42, $8f, $00, $93, $3a, $85, $00, $3a, $85, $00, $3a, $8f, $00, $3a, $85, $00, $3a, $85, $00, $3a, $8f, $00
	.byte $34, $8f, $00, $93, $42, $8f, $00, $93, $3e, $8f, $00, $93, $3a, $8f, $00, $93, $3e, $8f, $00, $a7, $93, $3e, $85, $00
	.byte $3e, $85, $00, $3e, $8f, $00, $3e, $85, $00, $3e, $85, $00, $3e, $8f, $00, $38, $8f, $00, $93, $42, $8f, $00, $93, $3e
	.byte $8f, $00, $93, $3a, $8f, $00, $93, $38, $8f, $00, $93
@song0ref139:
	.byte $34, $8f, $00, $93, $34, $8f, $00, $42, $8f, $00, $48, $8f, $00, $46, $b7, $00, $34, $8f, $00, $93, $34, $8f, $00, $42
	.byte $8f, $00, $48, $8f, $00, $46, $8f, $00, $93, $50, $8f, $00, $52, $8f, $00, $93
	.byte $ff, $a4
	.word @song0ref8
@song0ref182:
	.byte $48, $8f, $00, $8e, $4c, $91, $4c, $a5, $4c, $cb, $00, $48, $8f, $00, $52, $a3, $00, $4c, $a3, $00, $48, $a3, $00, $4c
	.byte $a3, $00, $93, $48, $cb, $00, $48, $8f, $00, $4c, $b7, $00, $42, $8f, $00, $44, $8f, $00, $42, $8f, $00, $3a, $8f, $00
	.byte $93, $3a, $8f, $00, $42, $8f, $00, $48, $8f, $00, $4c, $f3, $00, $48, $8f, $00, $52, $a3, $00, $4c, $a3, $00, $48, $a3
	.byte $00, $4c, $a3, $00, $48, $91, $48, $cb, $00, $48, $8f, $00, $42, $8f, $00, $48, $8f, $00, $4a, $8f, $00, $93, $4a, $8f
	.byte $00, $4a, $8f, $00, $50, $8f, $00, $5a, $cb, $00, $86, $2a, $f3, $00, $26, $8f, $00, $30, $a3, $00, $2c, $a3, $00, $2a
	.byte $a3, $00, $2c, $a3, $00, $2a, $91, $2a, $cd, $2a, $a3, $00, $26, $8f, $00, $30, $a3, $00, $2c, $a3, $00, $2a, $a3, $00
	.byte $2c, $a3, $00, $22, $91, $22, $cb, $00, $22, $8f, $00, $26, $8f, $00, $2a, $8f, $00, $20, $f9, $a1, $00, $8e
	.byte $ff, $21
	.word @song0ref139
	.byte $48, $8f, $00, $90, $4c, $91, $4c, $cb, $00, $4c, $a3, $00, $48, $8f, $00, $4c, $8f, $00, $93, $52, $b7, $00, $5a, $8f
	.byte $00, $56, $8f, $00, $52, $8f, $00, $50, $8f, $00, $4c, $cb, $00, $4c, $a3, $00, $48, $8f, $00, $4c, $8f, $00, $93, $52
	.byte $b7, $00, $52, $8f, $00, $56, $8f, $00, $52, $a3, $00, $50, $cb, $00, $50, $8f, $00, $4c, $8f, $00, $48, $8f, $00, $5a
	.byte $b7, $00, $56, $a3, $00, $52, $a3, $00, $50, $a3, $00, $50, $8f, $00, $52, $8f, $00, $50, $8f, $00, $4c, $cb, $00, $34
	.byte $8f, $00, $93, $34, $8f, $00, $42, $8f, $00, $48, $8f, $00, $46, $8f, $00, $93
	.byte $ff, $67
	.word @song0ref182
	.byte $b7, $00, $8a
@song0ref469:
	.byte $3a, $8f, $00, $93, $3a, $8f, $00, $3a, $8f, $00, $38, $8f, $00, $3a, $a3, $00, $4c, $8f, $00, $48, $a3, $00, $44, $a3
	.byte $00, $42, $a3, $00, $3e, $a3, $00, $3e, $8f, $00, $93, $42, $8f, $00, $42, $8f, $00, $93, $42, $8f, $00, $a7, $3e, $8f
	.byte $00, $93, $42, $8f, $00, $42, $8f, $00, $93, $42, $8f, $00, $93, $3e, $8f, $00
	.byte $ff, $1f
	.word @song0ref469
	.byte $93, $38, $8f, $00, $3a, $8f, $00, $38, $8f, $00, $34, $91, $34, $cb, $00, $f9, $a5, $fd
	.word @song0ch0loop
@song0ch1:
@song0ch1loop:
	.byte $93, $88
@song0ref559:
	.byte $34, $85, $00, $34, $85, $00, $34, $8f, $00, $34, $85, $00, $34, $85, $00, $34, $8f, $00, $2a, $8f, $00, $93, $2a, $85
	.byte $00, $2a, $85, $00, $34, $8f, $00, $34, $85, $00, $34, $85, $00, $34, $8f, $00, $2a, $8f, $00, $93, $3a, $8f, $00, $38
	.byte $8f, $00, $3a, $8f, $00, $93, $34, $85, $00, $34, $85, $00, $34, $8f, $00, $34, $85, $00, $34, $85, $00, $34, $8f, $00
	.byte $2a, $8f, $00, $93, $3a, $8f, $00, $93, $38, $8f, $00, $93, $34, $8f, $00, $93, $38, $8f, $00, $a7, $93, $38, $85, $00
	.byte $38, $85, $00, $38, $8f, $00, $38, $85, $00, $38, $85, $00, $38, $8f, $00, $30, $8f, $00, $93, $3a, $8f, $00, $93, $38
	.byte $8f, $00, $93, $34, $8f, $00, $93, $30, $8f, $00, $a7
	.byte $ff, $20
	.word @song0ref139
	.byte $48, $8f, $00, $4c, $8f, $00, $93
	.byte $ff, $83
	.word @song0ref559
	.byte $ff, $20
	.word @song0ref139
	.byte $48, $8f, $00, $4c, $8f, $00, $89, $8e
@song0ref714:
	.byte $4c, $91, $4c, $a5, $4c, $cb, $00, $48, $8f, $00, $52, $a3, $00, $4c, $a3, $00, $48, $a3, $00, $4c, $9b, $87, $00, $93
	.byte $48, $cb, $00, $48, $8f, $00, $4c, $b7, $00, $42, $8f, $00, $44, $8f, $00, $42, $8f, $00, $3a, $8f, $00, $93, $3a, $8f
	.byte $00, $42, $87, $87, $00, $48, $8f, $00, $4c, $f3, $00, $48, $8f, $00, $52, $a3, $00, $4c, $a3, $00, $48, $a3, $00, $4c
	.byte $9b, $87, $00, $48, $91, $48, $cb, $00, $48, $8f, $00, $42, $8f, $00, $48, $8f, $00, $4a, $8f, $00, $93, $4a, $8f, $00
	.byte $4a, $8f, $00, $50, $8f, $00, $5a, $a5, $5a, $87, $84, $5a
	.byte $ff, $84
	.word @song0ref6
	.byte $a7, $8e
	.byte $ff, $20
	.word @song0ref139
	.byte $56, $8f, $00, $5a, $8f, $00, $93, $84
	.byte $ff, $83
	.word @song0ref8
	.byte $8e, $34, $8f, $00, $93, $34, $8f, $00, $42, $8f, $00, $48, $8f, $00, $46, $cb, $00, $34, $8f, $00, $93, $34, $8f, $00
	.byte $42, $8f, $00, $48, $8f, $00, $46, $8f, $00, $56, $8f, $00, $5a, $8f, $00, $89
	.byte $ff, $66
	.word @song0ref714
	.byte $9d, $8c
@song0ref886:
	.byte $34, $8f, $00, $93, $34, $8f, $00, $34, $8f, $00, $30, $8f, $00, $34, $a3, $00, $44, $8f, $00, $3e, $a3, $00, $3e, $a3
	.byte $00, $3a, $a3, $00, $38, $a3, $00, $38, $8f, $00, $93, $3a, $8f, $00, $3a, $8f, $00, $93, $3a, $8f, $00, $a7, $38, $8f
	.byte $00, $93, $3a, $8f, $00, $3a, $8f, $00, $93, $3a, $8f, $00, $93, $38, $8f, $00
	.byte $ff, $1f
	.word @song0ref886
	.byte $93, $30, $8f, $00, $34, $8f, $00, $30, $8f, $00, $2a, $91, $2a, $cb, $00, $f9, $a5, $fd
	.word @song0ch1loop
@song0ch2:
@song0ch2loop:
	.byte $80
@song0ref975:
	.byte $4c, $8f, $00
@song0ref978:
	.byte $4c, $85, $00, $4c, $85, $00, $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00, $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00
	.byte $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00
	.byte $ff, $24
	.word @song0ref975
@song0ref1014:
	.byte $44, $8f, $00, $44, $85, $00, $44, $85, $00, $44, $8f, $00, $44, $85, $00, $44, $85, $00, $44, $8f, $00, $44, $85, $00
	.byte $44, $85, $00, $44, $8f, $00, $44, $85, $00, $44, $85, $00
	.byte $ff, $24
	.word @song0ref1014
@song0ref1053:
	.byte $48, $8f, $00, $48, $85, $00, $48, $85, $00, $48, $8f, $00, $48, $85, $00, $48, $85, $00, $48, $8f, $00, $48, $85, $00
	.byte $48, $85, $00, $48, $8f, $00, $48, $85, $00, $48, $85, $00
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref975
@song0ref1095:
	.byte $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00, $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00, $4c, $8f, $00, $4c, $85, $00
	.byte $4c, $85, $00, $48, $8f, $00, $4c, $91, $8b, $00, $85
	.byte $ff, $21
	.word @song0ref978
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $23
	.word @song0ref1095
@song0ref1154:
	.byte $4e, $85, $00, $4e, $85, $00, $4e, $8f, $00, $4e, $85, $00, $4e, $85, $00, $4e, $8f, $00, $5c, $85, $00, $5c, $85, $00
	.byte $56, $85, $00, $56, $85, $00, $56, $8f, $00, $93, $4e, $85, $00, $4e, $85, $00, $56, $8f, $00, $4e, $85, $00, $4e, $85
	.byte $00, $64, $8f, $00, $56, $85, $00, $56, $85, $00, $5c, $8f, $00, $4e, $85, $00, $4e, $85, $00, $4c, $8f, $00, $4c, $85
	.byte $00, $4c, $85, $00, $4c, $8f, $00, $4c, $85, $00, $4c, $85, $00, $4c, $8f, $00, $5a, $85, $00, $5a, $85, $00, $52, $85
	.byte $00, $52, $85, $00, $60, $8f, $00, $93, $4c, $85, $00, $4c, $85, $00, $52, $8f, $00, $4c, $85, $00, $4c, $85, $00, $60
	.byte $8f, $00, $52, $85, $00, $52, $85, $00, $5a, $8f, $00, $4c, $85, $00, $4c, $85, $00, $4e, $8f, $00
	.byte $ff, $43
	.word @song0ref1154
	.byte $ff, $1e
	.word @song0ref1053
	.byte $4a, $8f, $00, $93, $4a, $8f, $00, $4a, $8f, $00, $4a, $8f, $00, $4a, $9d, $00, $ad
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $23
	.word @song0ref1095
	.byte $ff, $21
	.word @song0ref978
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1014
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref1053
	.byte $ff, $24
	.word @song0ref975
	.byte $ff, $23
	.word @song0ref1095
	.byte $ff, $8c
	.word @song0ref1154
	.byte $ff, $43
	.word @song0ref1154
	.byte $ff, $1e
	.word @song0ref1053
	.byte $4a, $8f, $00, $93, $4a, $8f, $00, $4a, $8f, $00, $4a, $8f, $00, $4a, $9d, $00, $99
@song0ref1391:
	.byte $44, $91, $8b, $00, $85, $44, $85, $00, $44, $85, $00, $44, $8f, $00, $44, $85, $00, $44, $85, $00, $44, $8f, $00, $44
	.byte $85, $00, $44, $85, $00, $44, $8f, $00, $48, $9d, $00, $85, $48, $85, $00, $48, $85, $00, $48, $8f, $00, $48, $85, $00
	.byte $48, $85, $00, $48, $8f, $00, $48, $85, $00, $48, $85, $00, $48, $8f, $00, $34, $91, $8b, $00, $85, $4c, $8f, $00, $38
	.byte $8f, $00, $50, $8f, $00, $3a, $8f, $00, $52, $8f, $00, $38, $8f, $00, $34, $9d, $00, $85, $4c, $8f, $00, $38, $8f, $00
	.byte $50, $8f, $00, $3a, $8f, $00, $52, $8f, $00, $38, $8f, $00
	.byte $ff, $3c
	.word @song0ref1391
	.byte $4a, $9d, $00, $85, $4c, $9d, $00, $85, $82, $58, $85, $00, $58, $85, $00, $56, $85, $00, $56, $85, $00, $52, $85, $00
	.byte $52, $85, $00, $4e, $85, $00, $4e, $85, $00, $80, $48, $8f, $00, $4c, $8f, $00, $93, $48, $8f, $00, $4c, $9d, $00, $85
	.byte $48, $8f, $00, $4a, $8f, $00, $4c, $9d, $00, $85, $fd
	.word @song0ch2loop
@song0ch3:
@song0ch3loop:
	.byte $92, $5a, $91, $5a, $87, $5a, $87
@song0ref1571:
	.byte $94, $50, $91, $92, $5a, $87, $5a, $87, $5a, $91, $5a, $87, $5a, $87, $94, $50, $91, $92, $5a, $87, $5a, $87, $5a, $91
	.byte $5a, $87, $5a, $87, $94, $50, $91, $92, $5a, $87, $5a, $87, $5a, $91, $5a, $87, $5a, $87, $94, $50, $91, $92, $5a, $87
	.byte $5a, $87, $5a, $91, $5a, $89, $5a, $85
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $30
	.word @song0ref1571
	.byte $ff, $2a
	.word @song0ref1571
	.byte $fd
	.word @song0ch3loop
@song0ch4:
@song0ch4loop:
@song0ref1712:
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $f9, $f9, $cb, $f9, $f9, $cb
	.byte $ff, $1e
	.word @song0ref1712
	.byte $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb, $f9, $f9, $cb
	.byte $fd
	.word @song0ch4loop
