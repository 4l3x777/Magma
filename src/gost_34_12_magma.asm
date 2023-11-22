;GOST 34.12-2018 X64 'MAGMA'
;Author - 4l3x777

.const
	pi0		db 12, 4, 6, 2, 10, 5, 11, 9, 14, 8, 13, 7, 0, 3, 15, 1
	pi1		db 6, 8, 2, 3, 9, 10, 5, 12, 1, 14, 4, 7, 11, 13, 0, 15
	pi2		db 11, 3, 5, 8, 2, 15, 10, 13, 14, 1, 7, 4, 12, 9, 6, 0
	pi3		db 12, 8, 2, 1, 13, 4, 15, 6, 7, 0, 10, 5, 3, 14, 9, 11
	pi4		db 7, 15, 5, 10, 8, 1, 6, 13, 0, 9, 3, 14, 11, 4, 2, 12
	pi5		db 5, 13, 15, 6, 9, 2, 12, 10, 11, 7, 8, 1, 4, 3, 14, 0
	pi6		db 8, 14, 2, 5, 6, 9, 1, 12, 15, 4, 11, 0, 13, 10, 3, 7
	pi7		db 1, 7, 14, 13, 0, 5, 8, 3, 4, 15, 10, 6, 9, 12, 11, 2

.code 
	;t-transmission function
	_t_proc		proc
				push rsi
				push rdx
				push rdi

				xor rdi, rdi		

				mov eax, 0000000fh
				and eax, ecx
				xor edx, edx
				lea rsi, pi0
				mov dl, byte ptr[rsi + rax]
				xor edi, edx

				mov eax, 000000f0h
				and eax, ecx
				ror al, 4
				xor edx, edx
				lea rsi, pi1
				mov dl, byte ptr[rsi + rax]
				rol dl, 4
				xor edi, edx

				mov eax, 00000f00h
				and eax, ecx
				ror ax, 8
				xor edx, edx
				lea rsi, pi2
				mov dl, byte ptr[rsi + rax]
				rol dx, 8
				xor edi, edx

				mov eax, 0000f000h
				and eax, ecx
				ror ax, 12
				xor edx, edx
				lea rsi, pi3
				mov dl, byte ptr[rsi + rax]
				rol dx, 12
				xor edi, edx

				mov eax, 000f0000h
				and eax, ecx
				ror eax, 16
				xor edx, edx
				lea rsi, pi4
				mov dl, byte ptr[rsi + rax]
				rol edx, 16
				xor edi, edx

				mov eax, 00f00000h
				and eax, ecx
				rol eax, 12
				xor edx, edx
				lea rsi, pi5
				mov dl, byte ptr[rsi + rax]
				ror edx, 12
				xor edi, edx

				mov eax, 0f000000h
				and eax, ecx
				rol eax, 8
				xor edx, edx
				lea rsi, pi6
				mov dl, byte ptr[rsi + rax]
				ror edx, 8
				xor edi, edx

				mov eax, 0f0000000h
				and eax, ecx
				rol eax, 4
				xor edx, edx
				lea rsi, pi7
				mov dl, byte ptr[rsi + rax]
				ror edx, 4
				xor edi, edx

				mov eax, edi
				
				pop rdi
				pop rdx
				pop rsi
				ret
	_t_proc		endp

	;g-shift function
	_g_proc		proc
				add ecx, edx
				call _t_proc
				rol eax, 11
				ret
	_g_proc		endp

	;G1-linear function
	_G1_proc	proc
				push rsi

				push rdx
				mov edx, [r8]
				call _g_proc				
				pop rdx

				mov rsi, [rdx]
				xor eax, esi

				mov rsi, [r8]
				mov [rdx], esi

				mov [r8], eax

				pop rsi
				ret
	_G1_proc	endp

	;G2-inversion G1 linear function
	_G2_proc	proc		
				call _G1_proc

				push rsi
				mov eax, [r8]
				sal rax, 32
				xor rsi, rsi
				mov esi, [rdx]
				xor rax, rsi
				pop rsi
				ret
	_G2_proc	endp

	;RKey-primary key expantion function
	_RKey_proc	proc
				push rbx
				
				xor rax, rax
				mov al, dl
				push rdx
				xor rdx, rdx
				xor rbx, rbx
				mov bl, 8	
				div rbx
				mov rbx, rdx
				pop rdx	

				cmp rdx, 23
				jg RKey_24_31
				mov rax, 7
				sub rax, rbx
				mov rbx, rax				
	RKey_24_31:	xor rax, rax
				mov eax, dword ptr[rcx + 4*rbx]
				pop rbx
				ret
	_RKey_proc	endp

	;GOST_encr-encryption block function
	_GOST_encr	proc
				push rbx

				xor rbx, rbx
	round_func: push rdx
				mov rdx, rbx
				call _RKey_proc
				pop rdx

				push rcx
				mov rcx, rax			
				call _G1_proc
				pop rcx

				inc rbx
				cmp rbx, 30 
				jle round_func

				push rdx
				mov rdx, rbx
				call _RKey_proc
				pop rdx

				push rcx
				mov rcx, rax			
				call _G2_proc
				pop rcx
				pop rbx
				ret
	_GOST_encr	endp		

	;GOST_decr-decryption block function
	_GOST_decr	proc
				push rbx

				mov rbx, 31
	round_func: push rdx
				mov rdx, rbx
				call _RKey_proc
				pop rdx

				push rcx
				mov rcx, rax			
				call _G1_proc
				pop rcx

				dec rbx
				test rbx, rbx
				jnz round_func

				push rdx
				xor rdx, rdx
				call _RKey_proc
				pop rdx

				push rcx
				mov rcx, rax			
				call _G2_proc
				pop rcx
				pop rbx
				ret
	_GOST_decr	endp
END