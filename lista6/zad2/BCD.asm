
                opt f-g-h+l+o+
                org $1000

start           equ *

                lda #%00001111         ;liczba która chcemy wypisac
                sta byte        ;zapisujemy ją w byte
                
                lda #00         ;czyscimy akumulator
                ldx #8          ;ustawiamy iterator petli na 8 (ilosc bitów podanej liczby)
                sed             ;wlaczamy tryb dziesietny

bcd             asl byte        ;Przesumięcie bitowe byte (mnożenie przez 2)
                lda word        ;Wrzucenie word do akumulatora
                adc word        ;Dodanie word do akumulatora (z carry)
                sta word		;Wrzucenie akumulatora do word
                rol word+1		;Przesuniecie bitowe (i dodanie carry) word + 1
				;lda word+1
				;adc word+1
				;sta word+1
                dex				;Zmniejszenie X
                bne bcd         ;Jesli X > 0 skocz do bcd
                cld
                
                lda word+1          ;wypisanie drugiej cyfry starszego bajtu
				jsr print

                lda word            ;wypisanie pierwszej cyfry mlodszego bajtu
                lsr @
                lsr @
                lsr @
                lsr @
                jsr print

                lda word            ;wypisanie drugiej cyfry mlodszego bajtu
                jsr print

                brk

print           and #$f
                ora #'0'
                sta char
                lda <char
                ldx >char
                jsr $ff80
                rts

				org $2000
				
char            dta b(0)
byte            dta b(0)
word            dta b(0)
				dta b(0)

                org $2E0
                dta a(start)

                end of file