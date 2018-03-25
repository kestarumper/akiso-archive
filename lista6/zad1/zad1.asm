        opt f-g-h+l+o+ ; SET FLAGS
        org $1000  ; SET ORIGIN START POINT

start   ldy #1 ; Y = 5 //DONT KNOW WHY
        lda #%11110001 ; ACC = FF   // OUR VALUE
        jsr phex ; JUMP TO PHEX     STACK <- 
        ; RETUR HERE1 ; ADDRESS NEXT COMMAND - 1
        ;text = ABCD
        lda <text ;ACC <- CD
        ldx >text ;X <- AB
        jsr $ff80 ; PRINTOWANIE NA OBRAZIE (ADRES PAMIĘCI, KTÓRY ZOSTANIE WYJEBANY NA EKRAN)
        brk


phex	pha ; PUSH ACC'S VALUE TO STACK  (ACC, HERE1)
		jsr pxdig ; JUMP TO PXDIG STACK 
        ; RETURN HERE 2  (HERE2, ACC, HERE1)
		pla ; ACC <- STACK.pop() 
		lsr @ ;LOGICAL SHIFT RIGHT
		lsr @
		lsr @
		lsr @
pxdig	and #%00001111 ; OUR MASK - 4 LAST BITS 
		ora #'0' ; DODAJE 30, ŻEBY WEJŚĆ NA ASCII GRAŻYNA !
		cmp #'9'+1 ; IF (ACC > 9) c = 1 //hard version FOR ASCII IF ( ACC >= '9'+1) SET CARRY = 1 
		bcc pr ; BRANCH ON CARRY CLEAR IF ( CARRY == 0 ) JUMP PR if trueAVOID COMMAD BEFOR
		adc #'A'-'9'-2 ; #$06  A - F  CHANGE 11,12,13,14,15 -> LETTERS ALLWAYS ADDS 6 TO ACC
pr		sta text,Y ;store the value of acc at absolute memory location 'text' + Y
		dey ;decrement Y - place in string text
		rts ; return from subroutine
        

        
        org $2000

text    equ *
        dta b(0),b(0) ; tu zmienilem 

        dta b(10)
        dta b(0)

        org $2E0
        dta a(start)

        end of file