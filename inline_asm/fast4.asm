mov eax,[ebx+ 0 ]
test ax,0FFFFH
jz im32_z0
mov [ecx+ 0 ],ax
mov [ecx+ 2048 ],ax
im32_z0:
test eax,0FFFF0000H
jz im32_z1
mov [ecx+ 2 ],ax
mov [ecx+ 2050 ],ax
im32_z1:
mov eax,[ebx+ 4 ]
test ax,0FFFFH
jz im32_z2
mov [ecx+ 4 ],ax
mov [ecx+ 2052 ],ax
im32_z2:
test eax,0FFFF0000H
jz im32_z3
mov [ecx+ 6 ],ax
mov [ecx+ 2054 ],ax
im32_z3:
mov eax,[ebx+ 8 ]
test ax,0FFFFH
jz im32_z4
mov [ecx+ 8 ],ax
mov [ecx+ 2056 ],ax
im32_z4:
test eax,0FFFF0000H
jz im32_z5
mov [ecx+ 10 ],ax
mov [ecx+ 2058 ],ax
im32_z5:
mov eax,[ebx+ 12 ]
test ax,0FFFFH
jz im32_z6
mov [ecx+ 12 ],ax
mov [ecx+ 2060 ],ax
im32_z6:
test eax,0FFFF0000H
jz im32_z7
mov [ecx+ 14 ],ax
mov [ecx+ 2062 ],ax
im32_z7:
mov eax,[ebx+ 16 ]
test ax,0FFFFH
jz im32_z8
mov [ecx+ 16 ],ax
mov [ecx+ 2064 ],ax
im32_z8:
test eax,0FFFF0000H
jz im32_z9
mov [ecx+ 18 ],ax
mov [ecx+ 2066 ],ax
im32_z9:
mov eax,[ebx+ 20 ]
test ax,0FFFFH
jz im32_z10
mov [ecx+ 20 ],ax
mov [ecx+ 2068 ],ax
im32_z10:
test eax,0FFFF0000H
jz im32_z11
mov [ecx+ 22 ],ax
mov [ecx+ 2070 ],ax
im32_z11:
mov eax,[ebx+ 24 ]
test ax,0FFFFH
jz im32_z12
mov [ecx+ 24 ],ax
mov [ecx+ 2072 ],ax
im32_z12:
test eax,0FFFF0000H
jz im32_z13
mov [ecx+ 26 ],ax
mov [ecx+ 2074 ],ax
im32_z13:
mov eax,[ebx+ 28 ]
test ax,0FFFFH
jz im32_z14
mov [ecx+ 28 ],ax
mov [ecx+ 2076 ],ax
im32_z14:
test eax,0FFFF0000H
jz im32_z15
mov [ecx+ 30 ],ax
mov [ecx+ 2078 ],ax
im32_z15:
mov eax,[ebx+ 32 ]
test ax,0FFFFH
jz im32_z16
mov [ecx+ 32 ],ax
mov [ecx+ 2080 ],ax
im32_z16:
test eax,0FFFF0000H
jz im32_z17
mov [ecx+ 34 ],ax
mov [ecx+ 2082 ],ax
im32_z17:
mov eax,[ebx+ 36 ]
test ax,0FFFFH
jz im32_z18
mov [ecx+ 36 ],ax
mov [ecx+ 2084 ],ax
im32_z18:
test eax,0FFFF0000H
jz im32_z19
mov [ecx+ 38 ],ax
mov [ecx+ 2086 ],ax
im32_z19:
mov eax,[ebx+ 40 ]
test ax,0FFFFH
jz im32_z20
mov [ecx+ 40 ],ax
mov [ecx+ 2088 ],ax
im32_z20:
test eax,0FFFF0000H
jz im32_z21
mov [ecx+ 42 ],ax
mov [ecx+ 2090 ],ax
im32_z21:
mov eax,[ebx+ 44 ]
test ax,0FFFFH
jz im32_z22
mov [ecx+ 44 ],ax
mov [ecx+ 2092 ],ax
im32_z22:
test eax,0FFFF0000H
jz im32_z23
mov [ecx+ 46 ],ax
mov [ecx+ 2094 ],ax
im32_z23:
mov eax,[ebx+ 48 ]
test ax,0FFFFH
jz im32_z24
mov [ecx+ 48 ],ax
mov [ecx+ 2096 ],ax
im32_z24:
test eax,0FFFF0000H
jz im32_z25
mov [ecx+ 50 ],ax
mov [ecx+ 2098 ],ax
im32_z25:
mov eax,[ebx+ 52 ]
test ax,0FFFFH
jz im32_z26
mov [ecx+ 52 ],ax
mov [ecx+ 2100 ],ax
im32_z26:
test eax,0FFFF0000H
jz im32_z27
mov [ecx+ 54 ],ax
mov [ecx+ 2102 ],ax
im32_z27:
mov eax,[ebx+ 56 ]
test ax,0FFFFH
jz im32_z28
mov [ecx+ 56 ],ax
mov [ecx+ 2104 ],ax
im32_z28:
test eax,0FFFF0000H
jz im32_z29
mov [ecx+ 58 ],ax
mov [ecx+ 2106 ],ax
im32_z29:
mov eax,[ebx+ 60 ]
test ax,0FFFFH
jz im32_z30
mov [ecx+ 60 ],ax
mov [ecx+ 2108 ],ax
im32_z30:
test eax,0FFFF0000H
jz im32_z31
mov [ecx+ 62 ],ax
mov [ecx+ 2110 ],ax
im32_z31:
mov eax,[ebx+ 128 ]
test ax,0FFFFH
jz im32_z32
mov [ecx+ 4096 ],ax
mov [ecx+ 6144 ],ax
im32_z32:
test eax,0FFFF0000H
jz im32_z33
mov [ecx+ 4098 ],ax
mov [ecx+ 6146 ],ax
im32_z33:
mov eax,[ebx+ 132 ]
test ax,0FFFFH
jz im32_z34
mov [ecx+ 4100 ],ax
mov [ecx+ 6148 ],ax
im32_z34:
test eax,0FFFF0000H
jz im32_z35
mov [ecx+ 4102 ],ax
mov [ecx+ 6150 ],ax
im32_z35:
mov eax,[ebx+ 136 ]
test ax,0FFFFH
jz im32_z36
mov [ecx+ 4104 ],ax
mov [ecx+ 6152 ],ax
im32_z36:
test eax,0FFFF0000H
jz im32_z37
mov [ecx+ 4106 ],ax
mov [ecx+ 6154 ],ax
im32_z37:
mov eax,[ebx+ 140 ]
test ax,0FFFFH
jz im32_z38
mov [ecx+ 4108 ],ax
mov [ecx+ 6156 ],ax
im32_z38:
test eax,0FFFF0000H
jz im32_z39
mov [ecx+ 4110 ],ax
mov [ecx+ 6158 ],ax
im32_z39:
mov eax,[ebx+ 144 ]
test ax,0FFFFH
jz im32_z40
mov [ecx+ 4112 ],ax
mov [ecx+ 6160 ],ax
im32_z40:
test eax,0FFFF0000H
jz im32_z41
mov [ecx+ 4114 ],ax
mov [ecx+ 6162 ],ax
im32_z41:
mov eax,[ebx+ 148 ]
test ax,0FFFFH
jz im32_z42
mov [ecx+ 4116 ],ax
mov [ecx+ 6164 ],ax
im32_z42:
test eax,0FFFF0000H
jz im32_z43
mov [ecx+ 4118 ],ax
mov [ecx+ 6166 ],ax
im32_z43:
mov eax,[ebx+ 152 ]
test ax,0FFFFH
jz im32_z44
mov [ecx+ 4120 ],ax
mov [ecx+ 6168 ],ax
im32_z44:
test eax,0FFFF0000H
jz im32_z45
mov [ecx+ 4122 ],ax
mov [ecx+ 6170 ],ax
im32_z45:
mov eax,[ebx+ 156 ]
test ax,0FFFFH
jz im32_z46
mov [ecx+ 4124 ],ax
mov [ecx+ 6172 ],ax
im32_z46:
test eax,0FFFF0000H
jz im32_z47
mov [ecx+ 4126 ],ax
mov [ecx+ 6174 ],ax
im32_z47:
mov eax,[ebx+ 160 ]
test ax,0FFFFH
jz im32_z48
mov [ecx+ 4128 ],ax
mov [ecx+ 6176 ],ax
im32_z48:
test eax,0FFFF0000H
jz im32_z49
mov [ecx+ 4130 ],ax
mov [ecx+ 6178 ],ax
im32_z49:
mov eax,[ebx+ 164 ]
test ax,0FFFFH
jz im32_z50
mov [ecx+ 4132 ],ax
mov [ecx+ 6180 ],ax
im32_z50:
test eax,0FFFF0000H
jz im32_z51
mov [ecx+ 4134 ],ax
mov [ecx+ 6182 ],ax
im32_z51:
mov eax,[ebx+ 168 ]
test ax,0FFFFH
jz im32_z52
mov [ecx+ 4136 ],ax
mov [ecx+ 6184 ],ax
im32_z52:
test eax,0FFFF0000H
jz im32_z53
mov [ecx+ 4138 ],ax
mov [ecx+ 6186 ],ax
im32_z53:
mov eax,[ebx+ 172 ]
test ax,0FFFFH
jz im32_z54
mov [ecx+ 4140 ],ax
mov [ecx+ 6188 ],ax
im32_z54:
test eax,0FFFF0000H
jz im32_z55
mov [ecx+ 4142 ],ax
mov [ecx+ 6190 ],ax
im32_z55:
mov eax,[ebx+ 176 ]
test ax,0FFFFH
jz im32_z56
mov [ecx+ 4144 ],ax
mov [ecx+ 6192 ],ax
im32_z56:
test eax,0FFFF0000H
jz im32_z57
mov [ecx+ 4146 ],ax
mov [ecx+ 6194 ],ax
im32_z57:
mov eax,[ebx+ 180 ]
test ax,0FFFFH
jz im32_z58
mov [ecx+ 4148 ],ax
mov [ecx+ 6196 ],ax
im32_z58:
test eax,0FFFF0000H
jz im32_z59
mov [ecx+ 4150 ],ax
mov [ecx+ 6198 ],ax
im32_z59:
mov eax,[ebx+ 184 ]
test ax,0FFFFH
jz im32_z60
mov [ecx+ 4152 ],ax
mov [ecx+ 6200 ],ax
im32_z60:
test eax,0FFFF0000H
jz im32_z61
mov [ecx+ 4154 ],ax
mov [ecx+ 6202 ],ax
im32_z61:
mov eax,[ebx+ 188 ]
test ax,0FFFFH
jz im32_z62
mov [ecx+ 4156 ],ax
mov [ecx+ 6204 ],ax
im32_z62:
test eax,0FFFF0000H
jz im32_z63
mov [ecx+ 4158 ],ax
mov [ecx+ 6206 ],ax
im32_z63:
mov eax,[ebx+ 256 ]
test ax,0FFFFH
jz im32_z64
mov [ecx+ 8192 ],ax
mov [ecx+ 10240 ],ax
im32_z64:
test eax,0FFFF0000H
jz im32_z65
mov [ecx+ 8194 ],ax
mov [ecx+ 10242 ],ax
im32_z65:
mov eax,[ebx+ 260 ]
test ax,0FFFFH
jz im32_z66
mov [ecx+ 8196 ],ax
mov [ecx+ 10244 ],ax
im32_z66:
test eax,0FFFF0000H
jz im32_z67
mov [ecx+ 8198 ],ax
mov [ecx+ 10246 ],ax
im32_z67:
mov eax,[ebx+ 264 ]
test ax,0FFFFH
jz im32_z68
mov [ecx+ 8200 ],ax
mov [ecx+ 10248 ],ax
im32_z68:
test eax,0FFFF0000H
jz im32_z69
mov [ecx+ 8202 ],ax
mov [ecx+ 10250 ],ax
im32_z69:
mov eax,[ebx+ 268 ]
test ax,0FFFFH
jz im32_z70
mov [ecx+ 8204 ],ax
mov [ecx+ 10252 ],ax
im32_z70:
test eax,0FFFF0000H
jz im32_z71
mov [ecx+ 8206 ],ax
mov [ecx+ 10254 ],ax
im32_z71:
mov eax,[ebx+ 272 ]
test ax,0FFFFH
jz im32_z72
mov [ecx+ 8208 ],ax
mov [ecx+ 10256 ],ax
im32_z72:
test eax,0FFFF0000H
jz im32_z73
mov [ecx+ 8210 ],ax
mov [ecx+ 10258 ],ax
im32_z73:
mov eax,[ebx+ 276 ]
test ax,0FFFFH
jz im32_z74
mov [ecx+ 8212 ],ax
mov [ecx+ 10260 ],ax
im32_z74:
test eax,0FFFF0000H
jz im32_z75
mov [ecx+ 8214 ],ax
mov [ecx+ 10262 ],ax
im32_z75:
mov eax,[ebx+ 280 ]
test ax,0FFFFH
jz im32_z76
mov [ecx+ 8216 ],ax
mov [ecx+ 10264 ],ax
im32_z76:
test eax,0FFFF0000H
jz im32_z77
mov [ecx+ 8218 ],ax
mov [ecx+ 10266 ],ax
im32_z77:
mov eax,[ebx+ 284 ]
test ax,0FFFFH
jz im32_z78
mov [ecx+ 8220 ],ax
mov [ecx+ 10268 ],ax
im32_z78:
test eax,0FFFF0000H
jz im32_z79
mov [ecx+ 8222 ],ax
mov [ecx+ 10270 ],ax
im32_z79:
mov eax,[ebx+ 288 ]
test ax,0FFFFH
jz im32_z80
mov [ecx+ 8224 ],ax
mov [ecx+ 10272 ],ax
im32_z80:
test eax,0FFFF0000H
jz im32_z81
mov [ecx+ 8226 ],ax
mov [ecx+ 10274 ],ax
im32_z81:
mov eax,[ebx+ 292 ]
test ax,0FFFFH
jz im32_z82
mov [ecx+ 8228 ],ax
mov [ecx+ 10276 ],ax
im32_z82:
test eax,0FFFF0000H
jz im32_z83
mov [ecx+ 8230 ],ax
mov [ecx+ 10278 ],ax
im32_z83:
mov eax,[ebx+ 296 ]
test ax,0FFFFH
jz im32_z84
mov [ecx+ 8232 ],ax
mov [ecx+ 10280 ],ax
im32_z84:
test eax,0FFFF0000H
jz im32_z85
mov [ecx+ 8234 ],ax
mov [ecx+ 10282 ],ax
im32_z85:
mov eax,[ebx+ 300 ]
test ax,0FFFFH
jz im32_z86
mov [ecx+ 8236 ],ax
mov [ecx+ 10284 ],ax
im32_z86:
test eax,0FFFF0000H
jz im32_z87
mov [ecx+ 8238 ],ax
mov [ecx+ 10286 ],ax
im32_z87:
mov eax,[ebx+ 304 ]
test ax,0FFFFH
jz im32_z88
mov [ecx+ 8240 ],ax
mov [ecx+ 10288 ],ax
im32_z88:
test eax,0FFFF0000H
jz im32_z89
mov [ecx+ 8242 ],ax
mov [ecx+ 10290 ],ax
im32_z89:
mov eax,[ebx+ 308 ]
test ax,0FFFFH
jz im32_z90
mov [ecx+ 8244 ],ax
mov [ecx+ 10292 ],ax
im32_z90:
test eax,0FFFF0000H
jz im32_z91
mov [ecx+ 8246 ],ax
mov [ecx+ 10294 ],ax
im32_z91:
mov eax,[ebx+ 312 ]
test ax,0FFFFH
jz im32_z92
mov [ecx+ 8248 ],ax
mov [ecx+ 10296 ],ax
im32_z92:
test eax,0FFFF0000H
jz im32_z93
mov [ecx+ 8250 ],ax
mov [ecx+ 10298 ],ax
im32_z93:
mov eax,[ebx+ 316 ]
test ax,0FFFFH
jz im32_z94
mov [ecx+ 8252 ],ax
mov [ecx+ 10300 ],ax
im32_z94:
test eax,0FFFF0000H
jz im32_z95
mov [ecx+ 8254 ],ax
mov [ecx+ 10302 ],ax
im32_z95:
mov eax,[ebx+ 384 ]
test ax,0FFFFH
jz im32_z96
mov [ecx+ 12288 ],ax
mov [ecx+ 14336 ],ax
im32_z96:
test eax,0FFFF0000H
jz im32_z97
mov [ecx+ 12290 ],ax
mov [ecx+ 14338 ],ax
im32_z97:
mov eax,[ebx+ 388 ]
test ax,0FFFFH
jz im32_z98
mov [ecx+ 12292 ],ax
mov [ecx+ 14340 ],ax
im32_z98:
test eax,0FFFF0000H
jz im32_z99
mov [ecx+ 12294 ],ax
mov [ecx+ 14342 ],ax
im32_z99:
mov eax,[ebx+ 392 ]
test ax,0FFFFH
jz im32_z100
mov [ecx+ 12296 ],ax
mov [ecx+ 14344 ],ax
im32_z100:
test eax,0FFFF0000H
jz im32_z101
mov [ecx+ 12298 ],ax
mov [ecx+ 14346 ],ax
im32_z101:
mov eax,[ebx+ 396 ]
test ax,0FFFFH
jz im32_z102
mov [ecx+ 12300 ],ax
mov [ecx+ 14348 ],ax
im32_z102:
test eax,0FFFF0000H
jz im32_z103
mov [ecx+ 12302 ],ax
mov [ecx+ 14350 ],ax
im32_z103:
mov eax,[ebx+ 400 ]
test ax,0FFFFH
jz im32_z104
mov [ecx+ 12304 ],ax
mov [ecx+ 14352 ],ax
im32_z104:
test eax,0FFFF0000H
jz im32_z105
mov [ecx+ 12306 ],ax
mov [ecx+ 14354 ],ax
im32_z105:
mov eax,[ebx+ 404 ]
test ax,0FFFFH
jz im32_z106
mov [ecx+ 12308 ],ax
mov [ecx+ 14356 ],ax
im32_z106:
test eax,0FFFF0000H
jz im32_z107
mov [ecx+ 12310 ],ax
mov [ecx+ 14358 ],ax
im32_z107:
mov eax,[ebx+ 408 ]
test ax,0FFFFH
jz im32_z108
mov [ecx+ 12312 ],ax
mov [ecx+ 14360 ],ax
im32_z108:
test eax,0FFFF0000H
jz im32_z109
mov [ecx+ 12314 ],ax
mov [ecx+ 14362 ],ax
im32_z109:
mov eax,[ebx+ 412 ]
test ax,0FFFFH
jz im32_z110
mov [ecx+ 12316 ],ax
mov [ecx+ 14364 ],ax
im32_z110:
test eax,0FFFF0000H
jz im32_z111
mov [ecx+ 12318 ],ax
mov [ecx+ 14366 ],ax
im32_z111:
mov eax,[ebx+ 416 ]
test ax,0FFFFH
jz im32_z112
mov [ecx+ 12320 ],ax
mov [ecx+ 14368 ],ax
im32_z112:
test eax,0FFFF0000H
jz im32_z113
mov [ecx+ 12322 ],ax
mov [ecx+ 14370 ],ax
im32_z113:
mov eax,[ebx+ 420 ]
test ax,0FFFFH
jz im32_z114
mov [ecx+ 12324 ],ax
mov [ecx+ 14372 ],ax
im32_z114:
test eax,0FFFF0000H
jz im32_z115
mov [ecx+ 12326 ],ax
mov [ecx+ 14374 ],ax
im32_z115:
mov eax,[ebx+ 424 ]
test ax,0FFFFH
jz im32_z116
mov [ecx+ 12328 ],ax
mov [ecx+ 14376 ],ax
im32_z116:
test eax,0FFFF0000H
jz im32_z117
mov [ecx+ 12330 ],ax
mov [ecx+ 14378 ],ax
im32_z117:
mov eax,[ebx+ 428 ]
test ax,0FFFFH
jz im32_z118
mov [ecx+ 12332 ],ax
mov [ecx+ 14380 ],ax
im32_z118:
test eax,0FFFF0000H
jz im32_z119
mov [ecx+ 12334 ],ax
mov [ecx+ 14382 ],ax
im32_z119:
mov eax,[ebx+ 432 ]
test ax,0FFFFH
jz im32_z120
mov [ecx+ 12336 ],ax
mov [ecx+ 14384 ],ax
im32_z120:
test eax,0FFFF0000H
jz im32_z121
mov [ecx+ 12338 ],ax
mov [ecx+ 14386 ],ax
im32_z121:
mov eax,[ebx+ 436 ]
test ax,0FFFFH
jz im32_z122
mov [ecx+ 12340 ],ax
mov [ecx+ 14388 ],ax
im32_z122:
test eax,0FFFF0000H
jz im32_z123
mov [ecx+ 12342 ],ax
mov [ecx+ 14390 ],ax
im32_z123:
mov eax,[ebx+ 440 ]
test ax,0FFFFH
jz im32_z124
mov [ecx+ 12344 ],ax
mov [ecx+ 14392 ],ax
im32_z124:
test eax,0FFFF0000H
jz im32_z125
mov [ecx+ 12346 ],ax
mov [ecx+ 14394 ],ax
im32_z125:
mov eax,[ebx+ 444 ]
test ax,0FFFFH
jz im32_z126
mov [ecx+ 12348 ],ax
mov [ecx+ 14396 ],ax
im32_z126:
test eax,0FFFF0000H
jz im32_z127
mov [ecx+ 12350 ],ax
mov [ecx+ 14398 ],ax
im32_z127:
mov eax,[ebx+ 512 ]
test ax,0FFFFH
jz im32_z128
mov [ecx+ 16384 ],ax
mov [ecx+ 18432 ],ax
im32_z128:
test eax,0FFFF0000H
jz im32_z129
mov [ecx+ 16386 ],ax
mov [ecx+ 18434 ],ax
im32_z129:
mov eax,[ebx+ 516 ]
test ax,0FFFFH
jz im32_z130
mov [ecx+ 16388 ],ax
mov [ecx+ 18436 ],ax
im32_z130:
test eax,0FFFF0000H
jz im32_z131
mov [ecx+ 16390 ],ax
mov [ecx+ 18438 ],ax
im32_z131:
mov eax,[ebx+ 520 ]
test ax,0FFFFH
jz im32_z132
mov [ecx+ 16392 ],ax
mov [ecx+ 18440 ],ax
im32_z132:
test eax,0FFFF0000H
jz im32_z133
mov [ecx+ 16394 ],ax
mov [ecx+ 18442 ],ax
im32_z133:
mov eax,[ebx+ 524 ]
test ax,0FFFFH
jz im32_z134
mov [ecx+ 16396 ],ax
mov [ecx+ 18444 ],ax
im32_z134:
test eax,0FFFF0000H
jz im32_z135
mov [ecx+ 16398 ],ax
mov [ecx+ 18446 ],ax
im32_z135:
mov eax,[ebx+ 528 ]
test ax,0FFFFH
jz im32_z136
mov [ecx+ 16400 ],ax
mov [ecx+ 18448 ],ax
im32_z136:
test eax,0FFFF0000H
jz im32_z137
mov [ecx+ 16402 ],ax
mov [ecx+ 18450 ],ax
im32_z137:
mov eax,[ebx+ 532 ]
test ax,0FFFFH
jz im32_z138
mov [ecx+ 16404 ],ax
mov [ecx+ 18452 ],ax
im32_z138:
test eax,0FFFF0000H
jz im32_z139
mov [ecx+ 16406 ],ax
mov [ecx+ 18454 ],ax
im32_z139:
mov eax,[ebx+ 536 ]
test ax,0FFFFH
jz im32_z140
mov [ecx+ 16408 ],ax
mov [ecx+ 18456 ],ax
im32_z140:
test eax,0FFFF0000H
jz im32_z141
mov [ecx+ 16410 ],ax
mov [ecx+ 18458 ],ax
im32_z141:
mov eax,[ebx+ 540 ]
test ax,0FFFFH
jz im32_z142
mov [ecx+ 16412 ],ax
mov [ecx+ 18460 ],ax
im32_z142:
test eax,0FFFF0000H
jz im32_z143
mov [ecx+ 16414 ],ax
mov [ecx+ 18462 ],ax
im32_z143:
mov eax,[ebx+ 544 ]
test ax,0FFFFH
jz im32_z144
mov [ecx+ 16416 ],ax
mov [ecx+ 18464 ],ax
im32_z144:
test eax,0FFFF0000H
jz im32_z145
mov [ecx+ 16418 ],ax
mov [ecx+ 18466 ],ax
im32_z145:
mov eax,[ebx+ 548 ]
test ax,0FFFFH
jz im32_z146
mov [ecx+ 16420 ],ax
mov [ecx+ 18468 ],ax
im32_z146:
test eax,0FFFF0000H
jz im32_z147
mov [ecx+ 16422 ],ax
mov [ecx+ 18470 ],ax
im32_z147:
mov eax,[ebx+ 552 ]
test ax,0FFFFH
jz im32_z148
mov [ecx+ 16424 ],ax
mov [ecx+ 18472 ],ax
im32_z148:
test eax,0FFFF0000H
jz im32_z149
mov [ecx+ 16426 ],ax
mov [ecx+ 18474 ],ax
im32_z149:
mov eax,[ebx+ 556 ]
test ax,0FFFFH
jz im32_z150
mov [ecx+ 16428 ],ax
mov [ecx+ 18476 ],ax
im32_z150:
test eax,0FFFF0000H
jz im32_z151
mov [ecx+ 16430 ],ax
mov [ecx+ 18478 ],ax
im32_z151:
mov eax,[ebx+ 560 ]
test ax,0FFFFH
jz im32_z152
mov [ecx+ 16432 ],ax
mov [ecx+ 18480 ],ax
im32_z152:
test eax,0FFFF0000H
jz im32_z153
mov [ecx+ 16434 ],ax
mov [ecx+ 18482 ],ax
im32_z153:
mov eax,[ebx+ 564 ]
test ax,0FFFFH
jz im32_z154
mov [ecx+ 16436 ],ax
mov [ecx+ 18484 ],ax
im32_z154:
test eax,0FFFF0000H
jz im32_z155
mov [ecx+ 16438 ],ax
mov [ecx+ 18486 ],ax
im32_z155:
mov eax,[ebx+ 568 ]
test ax,0FFFFH
jz im32_z156
mov [ecx+ 16440 ],ax
mov [ecx+ 18488 ],ax
im32_z156:
test eax,0FFFF0000H
jz im32_z157
mov [ecx+ 16442 ],ax
mov [ecx+ 18490 ],ax
im32_z157:
mov eax,[ebx+ 572 ]
test ax,0FFFFH
jz im32_z158
mov [ecx+ 16444 ],ax
mov [ecx+ 18492 ],ax
im32_z158:
test eax,0FFFF0000H
jz im32_z159
mov [ecx+ 16446 ],ax
mov [ecx+ 18494 ],ax
im32_z159:
mov eax,[ebx+ 640 ]
test ax,0FFFFH
jz im32_z160
mov [ecx+ 20480 ],ax
mov [ecx+ 22528 ],ax
im32_z160:
test eax,0FFFF0000H
jz im32_z161
mov [ecx+ 20482 ],ax
mov [ecx+ 22530 ],ax
im32_z161:
mov eax,[ebx+ 644 ]
test ax,0FFFFH
jz im32_z162
mov [ecx+ 20484 ],ax
mov [ecx+ 22532 ],ax
im32_z162:
test eax,0FFFF0000H
jz im32_z163
mov [ecx+ 20486 ],ax
mov [ecx+ 22534 ],ax
im32_z163:
mov eax,[ebx+ 648 ]
test ax,0FFFFH
jz im32_z164
mov [ecx+ 20488 ],ax
mov [ecx+ 22536 ],ax
im32_z164:
test eax,0FFFF0000H
jz im32_z165
mov [ecx+ 20490 ],ax
mov [ecx+ 22538 ],ax
im32_z165:
mov eax,[ebx+ 652 ]
test ax,0FFFFH
jz im32_z166
mov [ecx+ 20492 ],ax
mov [ecx+ 22540 ],ax
im32_z166:
test eax,0FFFF0000H
jz im32_z167
mov [ecx+ 20494 ],ax
mov [ecx+ 22542 ],ax
im32_z167:
mov eax,[ebx+ 656 ]
test ax,0FFFFH
jz im32_z168
mov [ecx+ 20496 ],ax
mov [ecx+ 22544 ],ax
im32_z168:
test eax,0FFFF0000H
jz im32_z169
mov [ecx+ 20498 ],ax
mov [ecx+ 22546 ],ax
im32_z169:
mov eax,[ebx+ 660 ]
test ax,0FFFFH
jz im32_z170
mov [ecx+ 20500 ],ax
mov [ecx+ 22548 ],ax
im32_z170:
test eax,0FFFF0000H
jz im32_z171
mov [ecx+ 20502 ],ax
mov [ecx+ 22550 ],ax
im32_z171:
mov eax,[ebx+ 664 ]
test ax,0FFFFH
jz im32_z172
mov [ecx+ 20504 ],ax
mov [ecx+ 22552 ],ax
im32_z172:
test eax,0FFFF0000H
jz im32_z173
mov [ecx+ 20506 ],ax
mov [ecx+ 22554 ],ax
im32_z173:
mov eax,[ebx+ 668 ]
test ax,0FFFFH
jz im32_z174
mov [ecx+ 20508 ],ax
mov [ecx+ 22556 ],ax
im32_z174:
test eax,0FFFF0000H
jz im32_z175
mov [ecx+ 20510 ],ax
mov [ecx+ 22558 ],ax
im32_z175:
mov eax,[ebx+ 672 ]
test ax,0FFFFH
jz im32_z176
mov [ecx+ 20512 ],ax
mov [ecx+ 22560 ],ax
im32_z176:
test eax,0FFFF0000H
jz im32_z177
mov [ecx+ 20514 ],ax
mov [ecx+ 22562 ],ax
im32_z177:
mov eax,[ebx+ 676 ]
test ax,0FFFFH
jz im32_z178
mov [ecx+ 20516 ],ax
mov [ecx+ 22564 ],ax
im32_z178:
test eax,0FFFF0000H
jz im32_z179
mov [ecx+ 20518 ],ax
mov [ecx+ 22566 ],ax
im32_z179:
mov eax,[ebx+ 680 ]
test ax,0FFFFH
jz im32_z180
mov [ecx+ 20520 ],ax
mov [ecx+ 22568 ],ax
im32_z180:
test eax,0FFFF0000H
jz im32_z181
mov [ecx+ 20522 ],ax
mov [ecx+ 22570 ],ax
im32_z181:
mov eax,[ebx+ 684 ]
test ax,0FFFFH
jz im32_z182
mov [ecx+ 20524 ],ax
mov [ecx+ 22572 ],ax
im32_z182:
test eax,0FFFF0000H
jz im32_z183
mov [ecx+ 20526 ],ax
mov [ecx+ 22574 ],ax
im32_z183:
mov eax,[ebx+ 688 ]
test ax,0FFFFH
jz im32_z184
mov [ecx+ 20528 ],ax
mov [ecx+ 22576 ],ax
im32_z184:
test eax,0FFFF0000H
jz im32_z185
mov [ecx+ 20530 ],ax
mov [ecx+ 22578 ],ax
im32_z185:
mov eax,[ebx+ 692 ]
test ax,0FFFFH
jz im32_z186
mov [ecx+ 20532 ],ax
mov [ecx+ 22580 ],ax
im32_z186:
test eax,0FFFF0000H
jz im32_z187
mov [ecx+ 20534 ],ax
mov [ecx+ 22582 ],ax
im32_z187:
mov eax,[ebx+ 696 ]
test ax,0FFFFH
jz im32_z188
mov [ecx+ 20536 ],ax
mov [ecx+ 22584 ],ax
im32_z188:
test eax,0FFFF0000H
jz im32_z189
mov [ecx+ 20538 ],ax
mov [ecx+ 22586 ],ax
im32_z189:
mov eax,[ebx+ 700 ]
test ax,0FFFFH
jz im32_z190
mov [ecx+ 20540 ],ax
mov [ecx+ 22588 ],ax
im32_z190:
test eax,0FFFF0000H
jz im32_z191
mov [ecx+ 20542 ],ax
mov [ecx+ 22590 ],ax
im32_z191:
mov eax,[ebx+ 768 ]
test ax,0FFFFH
jz im32_z192
mov [ecx+ 24576 ],ax
mov [ecx+ 26624 ],ax
im32_z192:
test eax,0FFFF0000H
jz im32_z193
mov [ecx+ 24578 ],ax
mov [ecx+ 26626 ],ax
im32_z193:
mov eax,[ebx+ 772 ]
test ax,0FFFFH
jz im32_z194
mov [ecx+ 24580 ],ax
mov [ecx+ 26628 ],ax
im32_z194:
test eax,0FFFF0000H
jz im32_z195
mov [ecx+ 24582 ],ax
mov [ecx+ 26630 ],ax
im32_z195:
mov eax,[ebx+ 776 ]
test ax,0FFFFH
jz im32_z196
mov [ecx+ 24584 ],ax
mov [ecx+ 26632 ],ax
im32_z196:
test eax,0FFFF0000H
jz im32_z197
mov [ecx+ 24586 ],ax
mov [ecx+ 26634 ],ax
im32_z197:
mov eax,[ebx+ 780 ]
test ax,0FFFFH
jz im32_z198
mov [ecx+ 24588 ],ax
mov [ecx+ 26636 ],ax
im32_z198:
test eax,0FFFF0000H
jz im32_z199
mov [ecx+ 24590 ],ax
mov [ecx+ 26638 ],ax
im32_z199:
mov eax,[ebx+ 784 ]
test ax,0FFFFH
jz im32_z200
mov [ecx+ 24592 ],ax
mov [ecx+ 26640 ],ax
im32_z200:
test eax,0FFFF0000H
jz im32_z201
mov [ecx+ 24594 ],ax
mov [ecx+ 26642 ],ax
im32_z201:
mov eax,[ebx+ 788 ]
test ax,0FFFFH
jz im32_z202
mov [ecx+ 24596 ],ax
mov [ecx+ 26644 ],ax
im32_z202:
test eax,0FFFF0000H
jz im32_z203
mov [ecx+ 24598 ],ax
mov [ecx+ 26646 ],ax
im32_z203:
mov eax,[ebx+ 792 ]
test ax,0FFFFH
jz im32_z204
mov [ecx+ 24600 ],ax
mov [ecx+ 26648 ],ax
im32_z204:
test eax,0FFFF0000H
jz im32_z205
mov [ecx+ 24602 ],ax
mov [ecx+ 26650 ],ax
im32_z205:
mov eax,[ebx+ 796 ]
test ax,0FFFFH
jz im32_z206
mov [ecx+ 24604 ],ax
mov [ecx+ 26652 ],ax
im32_z206:
test eax,0FFFF0000H
jz im32_z207
mov [ecx+ 24606 ],ax
mov [ecx+ 26654 ],ax
im32_z207:
mov eax,[ebx+ 800 ]
test ax,0FFFFH
jz im32_z208
mov [ecx+ 24608 ],ax
mov [ecx+ 26656 ],ax
im32_z208:
test eax,0FFFF0000H
jz im32_z209
mov [ecx+ 24610 ],ax
mov [ecx+ 26658 ],ax
im32_z209:
mov eax,[ebx+ 804 ]
test ax,0FFFFH
jz im32_z210
mov [ecx+ 24612 ],ax
mov [ecx+ 26660 ],ax
im32_z210:
test eax,0FFFF0000H
jz im32_z211
mov [ecx+ 24614 ],ax
mov [ecx+ 26662 ],ax
im32_z211:
mov eax,[ebx+ 808 ]
test ax,0FFFFH
jz im32_z212
mov [ecx+ 24616 ],ax
mov [ecx+ 26664 ],ax
im32_z212:
test eax,0FFFF0000H
jz im32_z213
mov [ecx+ 24618 ],ax
mov [ecx+ 26666 ],ax
im32_z213:
mov eax,[ebx+ 812 ]
test ax,0FFFFH
jz im32_z214
mov [ecx+ 24620 ],ax
mov [ecx+ 26668 ],ax
im32_z214:
test eax,0FFFF0000H
jz im32_z215
mov [ecx+ 24622 ],ax
mov [ecx+ 26670 ],ax
im32_z215:
mov eax,[ebx+ 816 ]
test ax,0FFFFH
jz im32_z216
mov [ecx+ 24624 ],ax
mov [ecx+ 26672 ],ax
im32_z216:
test eax,0FFFF0000H
jz im32_z217
mov [ecx+ 24626 ],ax
mov [ecx+ 26674 ],ax
im32_z217:
mov eax,[ebx+ 820 ]
test ax,0FFFFH
jz im32_z218
mov [ecx+ 24628 ],ax
mov [ecx+ 26676 ],ax
im32_z218:
test eax,0FFFF0000H
jz im32_z219
mov [ecx+ 24630 ],ax
mov [ecx+ 26678 ],ax
im32_z219:
mov eax,[ebx+ 824 ]
test ax,0FFFFH
jz im32_z220
mov [ecx+ 24632 ],ax
mov [ecx+ 26680 ],ax
im32_z220:
test eax,0FFFF0000H
jz im32_z221
mov [ecx+ 24634 ],ax
mov [ecx+ 26682 ],ax
im32_z221:
mov eax,[ebx+ 828 ]
test ax,0FFFFH
jz im32_z222
mov [ecx+ 24636 ],ax
mov [ecx+ 26684 ],ax
im32_z222:
test eax,0FFFF0000H
jz im32_z223
mov [ecx+ 24638 ],ax
mov [ecx+ 26686 ],ax
im32_z223:
mov eax,[ebx+ 896 ]
test ax,0FFFFH
jz im32_z224
mov [ecx+ 28672 ],ax
mov [ecx+ 30720 ],ax
im32_z224:
test eax,0FFFF0000H
jz im32_z225
mov [ecx+ 28674 ],ax
mov [ecx+ 30722 ],ax
im32_z225:
mov eax,[ebx+ 900 ]
test ax,0FFFFH
jz im32_z226
mov [ecx+ 28676 ],ax
mov [ecx+ 30724 ],ax
im32_z226:
test eax,0FFFF0000H
jz im32_z227
mov [ecx+ 28678 ],ax
mov [ecx+ 30726 ],ax
im32_z227:
mov eax,[ebx+ 904 ]
test ax,0FFFFH
jz im32_z228
mov [ecx+ 28680 ],ax
mov [ecx+ 30728 ],ax
im32_z228:
test eax,0FFFF0000H
jz im32_z229
mov [ecx+ 28682 ],ax
mov [ecx+ 30730 ],ax
im32_z229:
mov eax,[ebx+ 908 ]
test ax,0FFFFH
jz im32_z230
mov [ecx+ 28684 ],ax
mov [ecx+ 30732 ],ax
im32_z230:
test eax,0FFFF0000H
jz im32_z231
mov [ecx+ 28686 ],ax
mov [ecx+ 30734 ],ax
im32_z231:
mov eax,[ebx+ 912 ]
test ax,0FFFFH
jz im32_z232
mov [ecx+ 28688 ],ax
mov [ecx+ 30736 ],ax
im32_z232:
test eax,0FFFF0000H
jz im32_z233
mov [ecx+ 28690 ],ax
mov [ecx+ 30738 ],ax
im32_z233:
mov eax,[ebx+ 916 ]
test ax,0FFFFH
jz im32_z234
mov [ecx+ 28692 ],ax
mov [ecx+ 30740 ],ax
im32_z234:
test eax,0FFFF0000H
jz im32_z235
mov [ecx+ 28694 ],ax
mov [ecx+ 30742 ],ax
im32_z235:
mov eax,[ebx+ 920 ]
test ax,0FFFFH
jz im32_z236
mov [ecx+ 28696 ],ax
mov [ecx+ 30744 ],ax
im32_z236:
test eax,0FFFF0000H
jz im32_z237
mov [ecx+ 28698 ],ax
mov [ecx+ 30746 ],ax
im32_z237:
mov eax,[ebx+ 924 ]
test ax,0FFFFH
jz im32_z238
mov [ecx+ 28700 ],ax
mov [ecx+ 30748 ],ax
im32_z238:
test eax,0FFFF0000H
jz im32_z239
mov [ecx+ 28702 ],ax
mov [ecx+ 30750 ],ax
im32_z239:
mov eax,[ebx+ 928 ]
test ax,0FFFFH
jz im32_z240
mov [ecx+ 28704 ],ax
mov [ecx+ 30752 ],ax
im32_z240:
test eax,0FFFF0000H
jz im32_z241
mov [ecx+ 28706 ],ax
mov [ecx+ 30754 ],ax
im32_z241:
mov eax,[ebx+ 932 ]
test ax,0FFFFH
jz im32_z242
mov [ecx+ 28708 ],ax
mov [ecx+ 30756 ],ax
im32_z242:
test eax,0FFFF0000H
jz im32_z243
mov [ecx+ 28710 ],ax
mov [ecx+ 30758 ],ax
im32_z243:
mov eax,[ebx+ 936 ]
test ax,0FFFFH
jz im32_z244
mov [ecx+ 28712 ],ax
mov [ecx+ 30760 ],ax
im32_z244:
test eax,0FFFF0000H
jz im32_z245
mov [ecx+ 28714 ],ax
mov [ecx+ 30762 ],ax
im32_z245:
mov eax,[ebx+ 940 ]
test ax,0FFFFH
jz im32_z246
mov [ecx+ 28716 ],ax
mov [ecx+ 30764 ],ax
im32_z246:
test eax,0FFFF0000H
jz im32_z247
mov [ecx+ 28718 ],ax
mov [ecx+ 30766 ],ax
im32_z247:
mov eax,[ebx+ 944 ]
test ax,0FFFFH
jz im32_z248
mov [ecx+ 28720 ],ax
mov [ecx+ 30768 ],ax
im32_z248:
test eax,0FFFF0000H
jz im32_z249
mov [ecx+ 28722 ],ax
mov [ecx+ 30770 ],ax
im32_z249:
mov eax,[ebx+ 948 ]
test ax,0FFFFH
jz im32_z250
mov [ecx+ 28724 ],ax
mov [ecx+ 30772 ],ax
im32_z250:
test eax,0FFFF0000H
jz im32_z251
mov [ecx+ 28726 ],ax
mov [ecx+ 30774 ],ax
im32_z251:
mov eax,[ebx+ 952 ]
test ax,0FFFFH
jz im32_z252
mov [ecx+ 28728 ],ax
mov [ecx+ 30776 ],ax
im32_z252:
test eax,0FFFF0000H
jz im32_z253
mov [ecx+ 28730 ],ax
mov [ecx+ 30778 ],ax
im32_z253:
mov eax,[ebx+ 956 ]
test ax,0FFFFH
jz im32_z254
mov [ecx+ 28732 ],ax
mov [ecx+ 30780 ],ax
im32_z254:
test eax,0FFFF0000H
jz im32_z255
mov [ecx+ 28734 ],ax
mov [ecx+ 30782 ],ax
im32_z255:
mov eax,[ebx+ 1024 ]
test ax,0FFFFH
jz im32_z256
mov [ecx+ 32768 ],ax
mov [ecx+ 34816 ],ax
im32_z256:
test eax,0FFFF0000H
jz im32_z257
mov [ecx+ 32770 ],ax
mov [ecx+ 34818 ],ax
im32_z257:
mov eax,[ebx+ 1028 ]
test ax,0FFFFH
jz im32_z258
mov [ecx+ 32772 ],ax
mov [ecx+ 34820 ],ax
im32_z258:
test eax,0FFFF0000H
jz im32_z259
mov [ecx+ 32774 ],ax
mov [ecx+ 34822 ],ax
im32_z259:
mov eax,[ebx+ 1032 ]
test ax,0FFFFH
jz im32_z260
mov [ecx+ 32776 ],ax
mov [ecx+ 34824 ],ax
im32_z260:
test eax,0FFFF0000H
jz im32_z261
mov [ecx+ 32778 ],ax
mov [ecx+ 34826 ],ax
im32_z261:
mov eax,[ebx+ 1036 ]
test ax,0FFFFH
jz im32_z262
mov [ecx+ 32780 ],ax
mov [ecx+ 34828 ],ax
im32_z262:
test eax,0FFFF0000H
jz im32_z263
mov [ecx+ 32782 ],ax
mov [ecx+ 34830 ],ax
im32_z263:
mov eax,[ebx+ 1040 ]
test ax,0FFFFH
jz im32_z264
mov [ecx+ 32784 ],ax
mov [ecx+ 34832 ],ax
im32_z264:
test eax,0FFFF0000H
jz im32_z265
mov [ecx+ 32786 ],ax
mov [ecx+ 34834 ],ax
im32_z265:
mov eax,[ebx+ 1044 ]
test ax,0FFFFH
jz im32_z266
mov [ecx+ 32788 ],ax
mov [ecx+ 34836 ],ax
im32_z266:
test eax,0FFFF0000H
jz im32_z267
mov [ecx+ 32790 ],ax
mov [ecx+ 34838 ],ax
im32_z267:
mov eax,[ebx+ 1048 ]
test ax,0FFFFH
jz im32_z268
mov [ecx+ 32792 ],ax
mov [ecx+ 34840 ],ax
im32_z268:
test eax,0FFFF0000H
jz im32_z269
mov [ecx+ 32794 ],ax
mov [ecx+ 34842 ],ax
im32_z269:
mov eax,[ebx+ 1052 ]
test ax,0FFFFH
jz im32_z270
mov [ecx+ 32796 ],ax
mov [ecx+ 34844 ],ax
im32_z270:
test eax,0FFFF0000H
jz im32_z271
mov [ecx+ 32798 ],ax
mov [ecx+ 34846 ],ax
im32_z271:
mov eax,[ebx+ 1056 ]
test ax,0FFFFH
jz im32_z272
mov [ecx+ 32800 ],ax
mov [ecx+ 34848 ],ax
im32_z272:
test eax,0FFFF0000H
jz im32_z273
mov [ecx+ 32802 ],ax
mov [ecx+ 34850 ],ax
im32_z273:
mov eax,[ebx+ 1060 ]
test ax,0FFFFH
jz im32_z274
mov [ecx+ 32804 ],ax
mov [ecx+ 34852 ],ax
im32_z274:
test eax,0FFFF0000H
jz im32_z275
mov [ecx+ 32806 ],ax
mov [ecx+ 34854 ],ax
im32_z275:
mov eax,[ebx+ 1064 ]
test ax,0FFFFH
jz im32_z276
mov [ecx+ 32808 ],ax
mov [ecx+ 34856 ],ax
im32_z276:
test eax,0FFFF0000H
jz im32_z277
mov [ecx+ 32810 ],ax
mov [ecx+ 34858 ],ax
im32_z277:
mov eax,[ebx+ 1068 ]
test ax,0FFFFH
jz im32_z278
mov [ecx+ 32812 ],ax
mov [ecx+ 34860 ],ax
im32_z278:
test eax,0FFFF0000H
jz im32_z279
mov [ecx+ 32814 ],ax
mov [ecx+ 34862 ],ax
im32_z279:
mov eax,[ebx+ 1072 ]
test ax,0FFFFH
jz im32_z280
mov [ecx+ 32816 ],ax
mov [ecx+ 34864 ],ax
im32_z280:
test eax,0FFFF0000H
jz im32_z281
mov [ecx+ 32818 ],ax
mov [ecx+ 34866 ],ax
im32_z281:
mov eax,[ebx+ 1076 ]
test ax,0FFFFH
jz im32_z282
mov [ecx+ 32820 ],ax
mov [ecx+ 34868 ],ax
im32_z282:
test eax,0FFFF0000H
jz im32_z283
mov [ecx+ 32822 ],ax
mov [ecx+ 34870 ],ax
im32_z283:
mov eax,[ebx+ 1080 ]
test ax,0FFFFH
jz im32_z284
mov [ecx+ 32824 ],ax
mov [ecx+ 34872 ],ax
im32_z284:
test eax,0FFFF0000H
jz im32_z285
mov [ecx+ 32826 ],ax
mov [ecx+ 34874 ],ax
im32_z285:
mov eax,[ebx+ 1084 ]
test ax,0FFFFH
jz im32_z286
mov [ecx+ 32828 ],ax
mov [ecx+ 34876 ],ax
im32_z286:
test eax,0FFFF0000H
jz im32_z287
mov [ecx+ 32830 ],ax
mov [ecx+ 34878 ],ax
im32_z287:
mov eax,[ebx+ 1152 ]
test ax,0FFFFH
jz im32_z288
mov [ecx+ 36864 ],ax
mov [ecx+ 38912 ],ax
im32_z288:
test eax,0FFFF0000H
jz im32_z289
mov [ecx+ 36866 ],ax
mov [ecx+ 38914 ],ax
im32_z289:
mov eax,[ebx+ 1156 ]
test ax,0FFFFH
jz im32_z290
mov [ecx+ 36868 ],ax
mov [ecx+ 38916 ],ax
im32_z290:
test eax,0FFFF0000H
jz im32_z291
mov [ecx+ 36870 ],ax
mov [ecx+ 38918 ],ax
im32_z291:
mov eax,[ebx+ 1160 ]
test ax,0FFFFH
jz im32_z292
mov [ecx+ 36872 ],ax
mov [ecx+ 38920 ],ax
im32_z292:
test eax,0FFFF0000H
jz im32_z293
mov [ecx+ 36874 ],ax
mov [ecx+ 38922 ],ax
im32_z293:
mov eax,[ebx+ 1164 ]
test ax,0FFFFH
jz im32_z294
mov [ecx+ 36876 ],ax
mov [ecx+ 38924 ],ax
im32_z294:
test eax,0FFFF0000H
jz im32_z295
mov [ecx+ 36878 ],ax
mov [ecx+ 38926 ],ax
im32_z295:
mov eax,[ebx+ 1168 ]
test ax,0FFFFH
jz im32_z296
mov [ecx+ 36880 ],ax
mov [ecx+ 38928 ],ax
im32_z296:
test eax,0FFFF0000H
jz im32_z297
mov [ecx+ 36882 ],ax
mov [ecx+ 38930 ],ax
im32_z297:
mov eax,[ebx+ 1172 ]
test ax,0FFFFH
jz im32_z298
mov [ecx+ 36884 ],ax
mov [ecx+ 38932 ],ax
im32_z298:
test eax,0FFFF0000H
jz im32_z299
mov [ecx+ 36886 ],ax
mov [ecx+ 38934 ],ax
im32_z299:
mov eax,[ebx+ 1176 ]
test ax,0FFFFH
jz im32_z300
mov [ecx+ 36888 ],ax
mov [ecx+ 38936 ],ax
im32_z300:
test eax,0FFFF0000H
jz im32_z301
mov [ecx+ 36890 ],ax
mov [ecx+ 38938 ],ax
im32_z301:
mov eax,[ebx+ 1180 ]
test ax,0FFFFH
jz im32_z302
mov [ecx+ 36892 ],ax
mov [ecx+ 38940 ],ax
im32_z302:
test eax,0FFFF0000H
jz im32_z303
mov [ecx+ 36894 ],ax
mov [ecx+ 38942 ],ax
im32_z303:
mov eax,[ebx+ 1184 ]
test ax,0FFFFH
jz im32_z304
mov [ecx+ 36896 ],ax
mov [ecx+ 38944 ],ax
im32_z304:
test eax,0FFFF0000H
jz im32_z305
mov [ecx+ 36898 ],ax
mov [ecx+ 38946 ],ax
im32_z305:
mov eax,[ebx+ 1188 ]
test ax,0FFFFH
jz im32_z306
mov [ecx+ 36900 ],ax
mov [ecx+ 38948 ],ax
im32_z306:
test eax,0FFFF0000H
jz im32_z307
mov [ecx+ 36902 ],ax
mov [ecx+ 38950 ],ax
im32_z307:
mov eax,[ebx+ 1192 ]
test ax,0FFFFH
jz im32_z308
mov [ecx+ 36904 ],ax
mov [ecx+ 38952 ],ax
im32_z308:
test eax,0FFFF0000H
jz im32_z309
mov [ecx+ 36906 ],ax
mov [ecx+ 38954 ],ax
im32_z309:
mov eax,[ebx+ 1196 ]
test ax,0FFFFH
jz im32_z310
mov [ecx+ 36908 ],ax
mov [ecx+ 38956 ],ax
im32_z310:
test eax,0FFFF0000H
jz im32_z311
mov [ecx+ 36910 ],ax
mov [ecx+ 38958 ],ax
im32_z311:
mov eax,[ebx+ 1200 ]
test ax,0FFFFH
jz im32_z312
mov [ecx+ 36912 ],ax
mov [ecx+ 38960 ],ax
im32_z312:
test eax,0FFFF0000H
jz im32_z313
mov [ecx+ 36914 ],ax
mov [ecx+ 38962 ],ax
im32_z313:
mov eax,[ebx+ 1204 ]
test ax,0FFFFH
jz im32_z314
mov [ecx+ 36916 ],ax
mov [ecx+ 38964 ],ax
im32_z314:
test eax,0FFFF0000H
jz im32_z315
mov [ecx+ 36918 ],ax
mov [ecx+ 38966 ],ax
im32_z315:
mov eax,[ebx+ 1208 ]
test ax,0FFFFH
jz im32_z316
mov [ecx+ 36920 ],ax
mov [ecx+ 38968 ],ax
im32_z316:
test eax,0FFFF0000H
jz im32_z317
mov [ecx+ 36922 ],ax
mov [ecx+ 38970 ],ax
im32_z317:
mov eax,[ebx+ 1212 ]
test ax,0FFFFH
jz im32_z318
mov [ecx+ 36924 ],ax
mov [ecx+ 38972 ],ax
im32_z318:
test eax,0FFFF0000H
jz im32_z319
mov [ecx+ 36926 ],ax
mov [ecx+ 38974 ],ax
im32_z319:
mov eax,[ebx+ 1280 ]
test ax,0FFFFH
jz im32_z320
mov [ecx+ 40960 ],ax
mov [ecx+ 43008 ],ax
im32_z320:
test eax,0FFFF0000H
jz im32_z321
mov [ecx+ 40962 ],ax
mov [ecx+ 43010 ],ax
im32_z321:
mov eax,[ebx+ 1284 ]
test ax,0FFFFH
jz im32_z322
mov [ecx+ 40964 ],ax
mov [ecx+ 43012 ],ax
im32_z322:
test eax,0FFFF0000H
jz im32_z323
mov [ecx+ 40966 ],ax
mov [ecx+ 43014 ],ax
im32_z323:
mov eax,[ebx+ 1288 ]
test ax,0FFFFH
jz im32_z324
mov [ecx+ 40968 ],ax
mov [ecx+ 43016 ],ax
im32_z324:
test eax,0FFFF0000H
jz im32_z325
mov [ecx+ 40970 ],ax
mov [ecx+ 43018 ],ax
im32_z325:
mov eax,[ebx+ 1292 ]
test ax,0FFFFH
jz im32_z326
mov [ecx+ 40972 ],ax
mov [ecx+ 43020 ],ax
im32_z326:
test eax,0FFFF0000H
jz im32_z327
mov [ecx+ 40974 ],ax
mov [ecx+ 43022 ],ax
im32_z327:
mov eax,[ebx+ 1296 ]
test ax,0FFFFH
jz im32_z328
mov [ecx+ 40976 ],ax
mov [ecx+ 43024 ],ax
im32_z328:
test eax,0FFFF0000H
jz im32_z329
mov [ecx+ 40978 ],ax
mov [ecx+ 43026 ],ax
im32_z329:
mov eax,[ebx+ 1300 ]
test ax,0FFFFH
jz im32_z330
mov [ecx+ 40980 ],ax
mov [ecx+ 43028 ],ax
im32_z330:
test eax,0FFFF0000H
jz im32_z331
mov [ecx+ 40982 ],ax
mov [ecx+ 43030 ],ax
im32_z331:
mov eax,[ebx+ 1304 ]
test ax,0FFFFH
jz im32_z332
mov [ecx+ 40984 ],ax
mov [ecx+ 43032 ],ax
im32_z332:
test eax,0FFFF0000H
jz im32_z333
mov [ecx+ 40986 ],ax
mov [ecx+ 43034 ],ax
im32_z333:
mov eax,[ebx+ 1308 ]
test ax,0FFFFH
jz im32_z334
mov [ecx+ 40988 ],ax
mov [ecx+ 43036 ],ax
im32_z334:
test eax,0FFFF0000H
jz im32_z335
mov [ecx+ 40990 ],ax
mov [ecx+ 43038 ],ax
im32_z335:
mov eax,[ebx+ 1312 ]
test ax,0FFFFH
jz im32_z336
mov [ecx+ 40992 ],ax
mov [ecx+ 43040 ],ax
im32_z336:
test eax,0FFFF0000H
jz im32_z337
mov [ecx+ 40994 ],ax
mov [ecx+ 43042 ],ax
im32_z337:
mov eax,[ebx+ 1316 ]
test ax,0FFFFH
jz im32_z338
mov [ecx+ 40996 ],ax
mov [ecx+ 43044 ],ax
im32_z338:
test eax,0FFFF0000H
jz im32_z339
mov [ecx+ 40998 ],ax
mov [ecx+ 43046 ],ax
im32_z339:
mov eax,[ebx+ 1320 ]
test ax,0FFFFH
jz im32_z340
mov [ecx+ 41000 ],ax
mov [ecx+ 43048 ],ax
im32_z340:
test eax,0FFFF0000H
jz im32_z341
mov [ecx+ 41002 ],ax
mov [ecx+ 43050 ],ax
im32_z341:
mov eax,[ebx+ 1324 ]
test ax,0FFFFH
jz im32_z342
mov [ecx+ 41004 ],ax
mov [ecx+ 43052 ],ax
im32_z342:
test eax,0FFFF0000H
jz im32_z343
mov [ecx+ 41006 ],ax
mov [ecx+ 43054 ],ax
im32_z343:
mov eax,[ebx+ 1328 ]
test ax,0FFFFH
jz im32_z344
mov [ecx+ 41008 ],ax
mov [ecx+ 43056 ],ax
im32_z344:
test eax,0FFFF0000H
jz im32_z345
mov [ecx+ 41010 ],ax
mov [ecx+ 43058 ],ax
im32_z345:
mov eax,[ebx+ 1332 ]
test ax,0FFFFH
jz im32_z346
mov [ecx+ 41012 ],ax
mov [ecx+ 43060 ],ax
im32_z346:
test eax,0FFFF0000H
jz im32_z347
mov [ecx+ 41014 ],ax
mov [ecx+ 43062 ],ax
im32_z347:
mov eax,[ebx+ 1336 ]
test ax,0FFFFH
jz im32_z348
mov [ecx+ 41016 ],ax
mov [ecx+ 43064 ],ax
im32_z348:
test eax,0FFFF0000H
jz im32_z349
mov [ecx+ 41018 ],ax
mov [ecx+ 43066 ],ax
im32_z349:
mov eax,[ebx+ 1340 ]
test ax,0FFFFH
jz im32_z350
mov [ecx+ 41020 ],ax
mov [ecx+ 43068 ],ax
im32_z350:
test eax,0FFFF0000H
jz im32_z351
mov [ecx+ 41022 ],ax
mov [ecx+ 43070 ],ax
im32_z351:
mov eax,[ebx+ 1408 ]
test ax,0FFFFH
jz im32_z352
mov [ecx+ 45056 ],ax
mov [ecx+ 47104 ],ax
im32_z352:
test eax,0FFFF0000H
jz im32_z353
mov [ecx+ 45058 ],ax
mov [ecx+ 47106 ],ax
im32_z353:
mov eax,[ebx+ 1412 ]
test ax,0FFFFH
jz im32_z354
mov [ecx+ 45060 ],ax
mov [ecx+ 47108 ],ax
im32_z354:
test eax,0FFFF0000H
jz im32_z355
mov [ecx+ 45062 ],ax
mov [ecx+ 47110 ],ax
im32_z355:
mov eax,[ebx+ 1416 ]
test ax,0FFFFH
jz im32_z356
mov [ecx+ 45064 ],ax
mov [ecx+ 47112 ],ax
im32_z356:
test eax,0FFFF0000H
jz im32_z357
mov [ecx+ 45066 ],ax
mov [ecx+ 47114 ],ax
im32_z357:
mov eax,[ebx+ 1420 ]
test ax,0FFFFH
jz im32_z358
mov [ecx+ 45068 ],ax
mov [ecx+ 47116 ],ax
im32_z358:
test eax,0FFFF0000H
jz im32_z359
mov [ecx+ 45070 ],ax
mov [ecx+ 47118 ],ax
im32_z359:
mov eax,[ebx+ 1424 ]
test ax,0FFFFH
jz im32_z360
mov [ecx+ 45072 ],ax
mov [ecx+ 47120 ],ax
im32_z360:
test eax,0FFFF0000H
jz im32_z361
mov [ecx+ 45074 ],ax
mov [ecx+ 47122 ],ax
im32_z361:
mov eax,[ebx+ 1428 ]
test ax,0FFFFH
jz im32_z362
mov [ecx+ 45076 ],ax
mov [ecx+ 47124 ],ax
im32_z362:
test eax,0FFFF0000H
jz im32_z363
mov [ecx+ 45078 ],ax
mov [ecx+ 47126 ],ax
im32_z363:
mov eax,[ebx+ 1432 ]
test ax,0FFFFH
jz im32_z364
mov [ecx+ 45080 ],ax
mov [ecx+ 47128 ],ax
im32_z364:
test eax,0FFFF0000H
jz im32_z365
mov [ecx+ 45082 ],ax
mov [ecx+ 47130 ],ax
im32_z365:
mov eax,[ebx+ 1436 ]
test ax,0FFFFH
jz im32_z366
mov [ecx+ 45084 ],ax
mov [ecx+ 47132 ],ax
im32_z366:
test eax,0FFFF0000H
jz im32_z367
mov [ecx+ 45086 ],ax
mov [ecx+ 47134 ],ax
im32_z367:
mov eax,[ebx+ 1440 ]
test ax,0FFFFH
jz im32_z368
mov [ecx+ 45088 ],ax
mov [ecx+ 47136 ],ax
im32_z368:
test eax,0FFFF0000H
jz im32_z369
mov [ecx+ 45090 ],ax
mov [ecx+ 47138 ],ax
im32_z369:
mov eax,[ebx+ 1444 ]
test ax,0FFFFH
jz im32_z370
mov [ecx+ 45092 ],ax
mov [ecx+ 47140 ],ax
im32_z370:
test eax,0FFFF0000H
jz im32_z371
mov [ecx+ 45094 ],ax
mov [ecx+ 47142 ],ax
im32_z371:
mov eax,[ebx+ 1448 ]
test ax,0FFFFH
jz im32_z372
mov [ecx+ 45096 ],ax
mov [ecx+ 47144 ],ax
im32_z372:
test eax,0FFFF0000H
jz im32_z373
mov [ecx+ 45098 ],ax
mov [ecx+ 47146 ],ax
im32_z373:
mov eax,[ebx+ 1452 ]
test ax,0FFFFH
jz im32_z374
mov [ecx+ 45100 ],ax
mov [ecx+ 47148 ],ax
im32_z374:
test eax,0FFFF0000H
jz im32_z375
mov [ecx+ 45102 ],ax
mov [ecx+ 47150 ],ax
im32_z375:
mov eax,[ebx+ 1456 ]
test ax,0FFFFH
jz im32_z376
mov [ecx+ 45104 ],ax
mov [ecx+ 47152 ],ax
im32_z376:
test eax,0FFFF0000H
jz im32_z377
mov [ecx+ 45106 ],ax
mov [ecx+ 47154 ],ax
im32_z377:
mov eax,[ebx+ 1460 ]
test ax,0FFFFH
jz im32_z378
mov [ecx+ 45108 ],ax
mov [ecx+ 47156 ],ax
im32_z378:
test eax,0FFFF0000H
jz im32_z379
mov [ecx+ 45110 ],ax
mov [ecx+ 47158 ],ax
im32_z379:
mov eax,[ebx+ 1464 ]
test ax,0FFFFH
jz im32_z380
mov [ecx+ 45112 ],ax
mov [ecx+ 47160 ],ax
im32_z380:
test eax,0FFFF0000H
jz im32_z381
mov [ecx+ 45114 ],ax
mov [ecx+ 47162 ],ax
im32_z381:
mov eax,[ebx+ 1468 ]
test ax,0FFFFH
jz im32_z382
mov [ecx+ 45116 ],ax
mov [ecx+ 47164 ],ax
im32_z382:
test eax,0FFFF0000H
jz im32_z383
mov [ecx+ 45118 ],ax
mov [ecx+ 47166 ],ax
im32_z383:
mov eax,[ebx+ 1536 ]
test ax,0FFFFH
jz im32_z384
mov [ecx+ 49152 ],ax
mov [ecx+ 51200 ],ax
im32_z384:
test eax,0FFFF0000H
jz im32_z385
mov [ecx+ 49154 ],ax
mov [ecx+ 51202 ],ax
im32_z385:
mov eax,[ebx+ 1540 ]
test ax,0FFFFH
jz im32_z386
mov [ecx+ 49156 ],ax
mov [ecx+ 51204 ],ax
im32_z386:
test eax,0FFFF0000H
jz im32_z387
mov [ecx+ 49158 ],ax
mov [ecx+ 51206 ],ax
im32_z387:
mov eax,[ebx+ 1544 ]
test ax,0FFFFH
jz im32_z388
mov [ecx+ 49160 ],ax
mov [ecx+ 51208 ],ax
im32_z388:
test eax,0FFFF0000H
jz im32_z389
mov [ecx+ 49162 ],ax
mov [ecx+ 51210 ],ax
im32_z389:
mov eax,[ebx+ 1548 ]
test ax,0FFFFH
jz im32_z390
mov [ecx+ 49164 ],ax
mov [ecx+ 51212 ],ax
im32_z390:
test eax,0FFFF0000H
jz im32_z391
mov [ecx+ 49166 ],ax
mov [ecx+ 51214 ],ax
im32_z391:
mov eax,[ebx+ 1552 ]
test ax,0FFFFH
jz im32_z392
mov [ecx+ 49168 ],ax
mov [ecx+ 51216 ],ax
im32_z392:
test eax,0FFFF0000H
jz im32_z393
mov [ecx+ 49170 ],ax
mov [ecx+ 51218 ],ax
im32_z393:
mov eax,[ebx+ 1556 ]
test ax,0FFFFH
jz im32_z394
mov [ecx+ 49172 ],ax
mov [ecx+ 51220 ],ax
im32_z394:
test eax,0FFFF0000H
jz im32_z395
mov [ecx+ 49174 ],ax
mov [ecx+ 51222 ],ax
im32_z395:
mov eax,[ebx+ 1560 ]
test ax,0FFFFH
jz im32_z396
mov [ecx+ 49176 ],ax
mov [ecx+ 51224 ],ax
im32_z396:
test eax,0FFFF0000H
jz im32_z397
mov [ecx+ 49178 ],ax
mov [ecx+ 51226 ],ax
im32_z397:
mov eax,[ebx+ 1564 ]
test ax,0FFFFH
jz im32_z398
mov [ecx+ 49180 ],ax
mov [ecx+ 51228 ],ax
im32_z398:
test eax,0FFFF0000H
jz im32_z399
mov [ecx+ 49182 ],ax
mov [ecx+ 51230 ],ax
im32_z399:
mov eax,[ebx+ 1568 ]
test ax,0FFFFH
jz im32_z400
mov [ecx+ 49184 ],ax
mov [ecx+ 51232 ],ax
im32_z400:
test eax,0FFFF0000H
jz im32_z401
mov [ecx+ 49186 ],ax
mov [ecx+ 51234 ],ax
im32_z401:
mov eax,[ebx+ 1572 ]
test ax,0FFFFH
jz im32_z402
mov [ecx+ 49188 ],ax
mov [ecx+ 51236 ],ax
im32_z402:
test eax,0FFFF0000H
jz im32_z403
mov [ecx+ 49190 ],ax
mov [ecx+ 51238 ],ax
im32_z403:
mov eax,[ebx+ 1576 ]
test ax,0FFFFH
jz im32_z404
mov [ecx+ 49192 ],ax
mov [ecx+ 51240 ],ax
im32_z404:
test eax,0FFFF0000H
jz im32_z405
mov [ecx+ 49194 ],ax
mov [ecx+ 51242 ],ax
im32_z405:
mov eax,[ebx+ 1580 ]
test ax,0FFFFH
jz im32_z406
mov [ecx+ 49196 ],ax
mov [ecx+ 51244 ],ax
im32_z406:
test eax,0FFFF0000H
jz im32_z407
mov [ecx+ 49198 ],ax
mov [ecx+ 51246 ],ax
im32_z407:
mov eax,[ebx+ 1584 ]
test ax,0FFFFH
jz im32_z408
mov [ecx+ 49200 ],ax
mov [ecx+ 51248 ],ax
im32_z408:
test eax,0FFFF0000H
jz im32_z409
mov [ecx+ 49202 ],ax
mov [ecx+ 51250 ],ax
im32_z409:
mov eax,[ebx+ 1588 ]
test ax,0FFFFH
jz im32_z410
mov [ecx+ 49204 ],ax
mov [ecx+ 51252 ],ax
im32_z410:
test eax,0FFFF0000H
jz im32_z411
mov [ecx+ 49206 ],ax
mov [ecx+ 51254 ],ax
im32_z411:
mov eax,[ebx+ 1592 ]
test ax,0FFFFH
jz im32_z412
mov [ecx+ 49208 ],ax
mov [ecx+ 51256 ],ax
im32_z412:
test eax,0FFFF0000H
jz im32_z413
mov [ecx+ 49210 ],ax
mov [ecx+ 51258 ],ax
im32_z413:
mov eax,[ebx+ 1596 ]
test ax,0FFFFH
jz im32_z414
mov [ecx+ 49212 ],ax
mov [ecx+ 51260 ],ax
im32_z414:
test eax,0FFFF0000H
jz im32_z415
mov [ecx+ 49214 ],ax
mov [ecx+ 51262 ],ax
im32_z415:
mov eax,[ebx+ 1664 ]
test ax,0FFFFH
jz im32_z416
mov [ecx+ 53248 ],ax
mov [ecx+ 55296 ],ax
im32_z416:
test eax,0FFFF0000H
jz im32_z417
mov [ecx+ 53250 ],ax
mov [ecx+ 55298 ],ax
im32_z417:
mov eax,[ebx+ 1668 ]
test ax,0FFFFH
jz im32_z418
mov [ecx+ 53252 ],ax
mov [ecx+ 55300 ],ax
im32_z418:
test eax,0FFFF0000H
jz im32_z419
mov [ecx+ 53254 ],ax
mov [ecx+ 55302 ],ax
im32_z419:
mov eax,[ebx+ 1672 ]
test ax,0FFFFH
jz im32_z420
mov [ecx+ 53256 ],ax
mov [ecx+ 55304 ],ax
im32_z420:
test eax,0FFFF0000H
jz im32_z421
mov [ecx+ 53258 ],ax
mov [ecx+ 55306 ],ax
im32_z421:
mov eax,[ebx+ 1676 ]
test ax,0FFFFH
jz im32_z422
mov [ecx+ 53260 ],ax
mov [ecx+ 55308 ],ax
im32_z422:
test eax,0FFFF0000H
jz im32_z423
mov [ecx+ 53262 ],ax
mov [ecx+ 55310 ],ax
im32_z423:
mov eax,[ebx+ 1680 ]
test ax,0FFFFH
jz im32_z424
mov [ecx+ 53264 ],ax
mov [ecx+ 55312 ],ax
im32_z424:
test eax,0FFFF0000H
jz im32_z425
mov [ecx+ 53266 ],ax
mov [ecx+ 55314 ],ax
im32_z425:
mov eax,[ebx+ 1684 ]
test ax,0FFFFH
jz im32_z426
mov [ecx+ 53268 ],ax
mov [ecx+ 55316 ],ax
im32_z426:
test eax,0FFFF0000H
jz im32_z427
mov [ecx+ 53270 ],ax
mov [ecx+ 55318 ],ax
im32_z427:
mov eax,[ebx+ 1688 ]
test ax,0FFFFH
jz im32_z428
mov [ecx+ 53272 ],ax
mov [ecx+ 55320 ],ax
im32_z428:
test eax,0FFFF0000H
jz im32_z429
mov [ecx+ 53274 ],ax
mov [ecx+ 55322 ],ax
im32_z429:
mov eax,[ebx+ 1692 ]
test ax,0FFFFH
jz im32_z430
mov [ecx+ 53276 ],ax
mov [ecx+ 55324 ],ax
im32_z430:
test eax,0FFFF0000H
jz im32_z431
mov [ecx+ 53278 ],ax
mov [ecx+ 55326 ],ax
im32_z431:
mov eax,[ebx+ 1696 ]
test ax,0FFFFH
jz im32_z432
mov [ecx+ 53280 ],ax
mov [ecx+ 55328 ],ax
im32_z432:
test eax,0FFFF0000H
jz im32_z433
mov [ecx+ 53282 ],ax
mov [ecx+ 55330 ],ax
im32_z433:
mov eax,[ebx+ 1700 ]
test ax,0FFFFH
jz im32_z434
mov [ecx+ 53284 ],ax
mov [ecx+ 55332 ],ax
im32_z434:
test eax,0FFFF0000H
jz im32_z435
mov [ecx+ 53286 ],ax
mov [ecx+ 55334 ],ax
im32_z435:
mov eax,[ebx+ 1704 ]
test ax,0FFFFH
jz im32_z436
mov [ecx+ 53288 ],ax
mov [ecx+ 55336 ],ax
im32_z436:
test eax,0FFFF0000H
jz im32_z437
mov [ecx+ 53290 ],ax
mov [ecx+ 55338 ],ax
im32_z437:
mov eax,[ebx+ 1708 ]
test ax,0FFFFH
jz im32_z438
mov [ecx+ 53292 ],ax
mov [ecx+ 55340 ],ax
im32_z438:
test eax,0FFFF0000H
jz im32_z439
mov [ecx+ 53294 ],ax
mov [ecx+ 55342 ],ax
im32_z439:
mov eax,[ebx+ 1712 ]
test ax,0FFFFH
jz im32_z440
mov [ecx+ 53296 ],ax
mov [ecx+ 55344 ],ax
im32_z440:
test eax,0FFFF0000H
jz im32_z441
mov [ecx+ 53298 ],ax
mov [ecx+ 55346 ],ax
im32_z441:
mov eax,[ebx+ 1716 ]
test ax,0FFFFH
jz im32_z442
mov [ecx+ 53300 ],ax
mov [ecx+ 55348 ],ax
im32_z442:
test eax,0FFFF0000H
jz im32_z443
mov [ecx+ 53302 ],ax
mov [ecx+ 55350 ],ax
im32_z443:
mov eax,[ebx+ 1720 ]
test ax,0FFFFH
jz im32_z444
mov [ecx+ 53304 ],ax
mov [ecx+ 55352 ],ax
im32_z444:
test eax,0FFFF0000H
jz im32_z445
mov [ecx+ 53306 ],ax
mov [ecx+ 55354 ],ax
im32_z445:
mov eax,[ebx+ 1724 ]
test ax,0FFFFH
jz im32_z446
mov [ecx+ 53308 ],ax
mov [ecx+ 55356 ],ax
im32_z446:
test eax,0FFFF0000H
jz im32_z447
mov [ecx+ 53310 ],ax
mov [ecx+ 55358 ],ax
im32_z447:
mov eax,[ebx+ 1792 ]
test ax,0FFFFH
jz im32_z448
mov [ecx+ 57344 ],ax
mov [ecx+ 59392 ],ax
im32_z448:
test eax,0FFFF0000H
jz im32_z449
mov [ecx+ 57346 ],ax
mov [ecx+ 59394 ],ax
im32_z449:
mov eax,[ebx+ 1796 ]
test ax,0FFFFH
jz im32_z450
mov [ecx+ 57348 ],ax
mov [ecx+ 59396 ],ax
im32_z450:
test eax,0FFFF0000H
jz im32_z451
mov [ecx+ 57350 ],ax
mov [ecx+ 59398 ],ax
im32_z451:
mov eax,[ebx+ 1800 ]
test ax,0FFFFH
jz im32_z452
mov [ecx+ 57352 ],ax
mov [ecx+ 59400 ],ax
im32_z452:
test eax,0FFFF0000H
jz im32_z453
mov [ecx+ 57354 ],ax
mov [ecx+ 59402 ],ax
im32_z453:
mov eax,[ebx+ 1804 ]
test ax,0FFFFH
jz im32_z454
mov [ecx+ 57356 ],ax
mov [ecx+ 59404 ],ax
im32_z454:
test eax,0FFFF0000H
jz im32_z455
mov [ecx+ 57358 ],ax
mov [ecx+ 59406 ],ax
im32_z455:
mov eax,[ebx+ 1808 ]
test ax,0FFFFH
jz im32_z456
mov [ecx+ 57360 ],ax
mov [ecx+ 59408 ],ax
im32_z456:
test eax,0FFFF0000H
jz im32_z457
mov [ecx+ 57362 ],ax
mov [ecx+ 59410 ],ax
im32_z457:
mov eax,[ebx+ 1812 ]
test ax,0FFFFH
jz im32_z458
mov [ecx+ 57364 ],ax
mov [ecx+ 59412 ],ax
im32_z458:
test eax,0FFFF0000H
jz im32_z459
mov [ecx+ 57366 ],ax
mov [ecx+ 59414 ],ax
im32_z459:
mov eax,[ebx+ 1816 ]
test ax,0FFFFH
jz im32_z460
mov [ecx+ 57368 ],ax
mov [ecx+ 59416 ],ax
im32_z460:
test eax,0FFFF0000H
jz im32_z461
mov [ecx+ 57370 ],ax
mov [ecx+ 59418 ],ax
im32_z461:
mov eax,[ebx+ 1820 ]
test ax,0FFFFH
jz im32_z462
mov [ecx+ 57372 ],ax
mov [ecx+ 59420 ],ax
im32_z462:
test eax,0FFFF0000H
jz im32_z463
mov [ecx+ 57374 ],ax
mov [ecx+ 59422 ],ax
im32_z463:
mov eax,[ebx+ 1824 ]
test ax,0FFFFH
jz im32_z464
mov [ecx+ 57376 ],ax
mov [ecx+ 59424 ],ax
im32_z464:
test eax,0FFFF0000H
jz im32_z465
mov [ecx+ 57378 ],ax
mov [ecx+ 59426 ],ax
im32_z465:
mov eax,[ebx+ 1828 ]
test ax,0FFFFH
jz im32_z466
mov [ecx+ 57380 ],ax
mov [ecx+ 59428 ],ax
im32_z466:
test eax,0FFFF0000H
jz im32_z467
mov [ecx+ 57382 ],ax
mov [ecx+ 59430 ],ax
im32_z467:
mov eax,[ebx+ 1832 ]
test ax,0FFFFH
jz im32_z468
mov [ecx+ 57384 ],ax
mov [ecx+ 59432 ],ax
im32_z468:
test eax,0FFFF0000H
jz im32_z469
mov [ecx+ 57386 ],ax
mov [ecx+ 59434 ],ax
im32_z469:
mov eax,[ebx+ 1836 ]
test ax,0FFFFH
jz im32_z470
mov [ecx+ 57388 ],ax
mov [ecx+ 59436 ],ax
im32_z470:
test eax,0FFFF0000H
jz im32_z471
mov [ecx+ 57390 ],ax
mov [ecx+ 59438 ],ax
im32_z471:
mov eax,[ebx+ 1840 ]
test ax,0FFFFH
jz im32_z472
mov [ecx+ 57392 ],ax
mov [ecx+ 59440 ],ax
im32_z472:
test eax,0FFFF0000H
jz im32_z473
mov [ecx+ 57394 ],ax
mov [ecx+ 59442 ],ax
im32_z473:
mov eax,[ebx+ 1844 ]
test ax,0FFFFH
jz im32_z474
mov [ecx+ 57396 ],ax
mov [ecx+ 59444 ],ax
im32_z474:
test eax,0FFFF0000H
jz im32_z475
mov [ecx+ 57398 ],ax
mov [ecx+ 59446 ],ax
im32_z475:
mov eax,[ebx+ 1848 ]
test ax,0FFFFH
jz im32_z476
mov [ecx+ 57400 ],ax
mov [ecx+ 59448 ],ax
im32_z476:
test eax,0FFFF0000H
jz im32_z477
mov [ecx+ 57402 ],ax
mov [ecx+ 59450 ],ax
im32_z477:
mov eax,[ebx+ 1852 ]
test ax,0FFFFH
jz im32_z478
mov [ecx+ 57404 ],ax
mov [ecx+ 59452 ],ax
im32_z478:
test eax,0FFFF0000H
jz im32_z479
mov [ecx+ 57406 ],ax
mov [ecx+ 59454 ],ax
im32_z479:
mov eax,[ebx+ 1920 ]
test ax,0FFFFH
jz im32_z480
mov [ecx+ 61440 ],ax
mov [ecx+ 63488 ],ax
im32_z480:
test eax,0FFFF0000H
jz im32_z481
mov [ecx+ 61442 ],ax
mov [ecx+ 63490 ],ax
im32_z481:
mov eax,[ebx+ 1924 ]
test ax,0FFFFH
jz im32_z482
mov [ecx+ 61444 ],ax
mov [ecx+ 63492 ],ax
im32_z482:
test eax,0FFFF0000H
jz im32_z483
mov [ecx+ 61446 ],ax
mov [ecx+ 63494 ],ax
im32_z483:
mov eax,[ebx+ 1928 ]
test ax,0FFFFH
jz im32_z484
mov [ecx+ 61448 ],ax
mov [ecx+ 63496 ],ax
im32_z484:
test eax,0FFFF0000H
jz im32_z485
mov [ecx+ 61450 ],ax
mov [ecx+ 63498 ],ax
im32_z485:
mov eax,[ebx+ 1932 ]
test ax,0FFFFH
jz im32_z486
mov [ecx+ 61452 ],ax
mov [ecx+ 63500 ],ax
im32_z486:
test eax,0FFFF0000H
jz im32_z487
mov [ecx+ 61454 ],ax
mov [ecx+ 63502 ],ax
im32_z487:
mov eax,[ebx+ 1936 ]
test ax,0FFFFH
jz im32_z488
mov [ecx+ 61456 ],ax
mov [ecx+ 63504 ],ax
im32_z488:
test eax,0FFFF0000H
jz im32_z489
mov [ecx+ 61458 ],ax
mov [ecx+ 63506 ],ax
im32_z489:
mov eax,[ebx+ 1940 ]
test ax,0FFFFH
jz im32_z490
mov [ecx+ 61460 ],ax
mov [ecx+ 63508 ],ax
im32_z490:
test eax,0FFFF0000H
jz im32_z491
mov [ecx+ 61462 ],ax
mov [ecx+ 63510 ],ax
im32_z491:
mov eax,[ebx+ 1944 ]
test ax,0FFFFH
jz im32_z492
mov [ecx+ 61464 ],ax
mov [ecx+ 63512 ],ax
im32_z492:
test eax,0FFFF0000H
jz im32_z493
mov [ecx+ 61466 ],ax
mov [ecx+ 63514 ],ax
im32_z493:
mov eax,[ebx+ 1948 ]
test ax,0FFFFH
jz im32_z494
mov [ecx+ 61468 ],ax
mov [ecx+ 63516 ],ax
im32_z494:
test eax,0FFFF0000H
jz im32_z495
mov [ecx+ 61470 ],ax
mov [ecx+ 63518 ],ax
im32_z495:
mov eax,[ebx+ 1952 ]
test ax,0FFFFH
jz im32_z496
mov [ecx+ 61472 ],ax
mov [ecx+ 63520 ],ax
im32_z496:
test eax,0FFFF0000H
jz im32_z497
mov [ecx+ 61474 ],ax
mov [ecx+ 63522 ],ax
im32_z497:
mov eax,[ebx+ 1956 ]
test ax,0FFFFH
jz im32_z498
mov [ecx+ 61476 ],ax
mov [ecx+ 63524 ],ax
im32_z498:
test eax,0FFFF0000H
jz im32_z499
mov [ecx+ 61478 ],ax
mov [ecx+ 63526 ],ax
im32_z499:
mov eax,[ebx+ 1960 ]
test ax,0FFFFH
jz im32_z500
mov [ecx+ 61480 ],ax
mov [ecx+ 63528 ],ax
im32_z500:
test eax,0FFFF0000H
jz im32_z501
mov [ecx+ 61482 ],ax
mov [ecx+ 63530 ],ax
im32_z501:
mov eax,[ebx+ 1964 ]
test ax,0FFFFH
jz im32_z502
mov [ecx+ 61484 ],ax
mov [ecx+ 63532 ],ax
im32_z502:
test eax,0FFFF0000H
jz im32_z503
mov [ecx+ 61486 ],ax
mov [ecx+ 63534 ],ax
im32_z503:
mov eax,[ebx+ 1968 ]
test ax,0FFFFH
jz im32_z504
mov [ecx+ 61488 ],ax
mov [ecx+ 63536 ],ax
im32_z504:
test eax,0FFFF0000H
jz im32_z505
mov [ecx+ 61490 ],ax
mov [ecx+ 63538 ],ax
im32_z505:
mov eax,[ebx+ 1972 ]
test ax,0FFFFH
jz im32_z506
mov [ecx+ 61492 ],ax
mov [ecx+ 63540 ],ax
im32_z506:
test eax,0FFFF0000H
jz im32_z507
mov [ecx+ 61494 ],ax
mov [ecx+ 63542 ],ax
im32_z507:
mov eax,[ebx+ 1976 ]
test ax,0FFFFH
jz im32_z508
mov [ecx+ 61496 ],ax
mov [ecx+ 63544 ],ax
im32_z508:
test eax,0FFFF0000H
jz im32_z509
mov [ecx+ 61498 ],ax
mov [ecx+ 63546 ],ax
im32_z509:
mov eax,[ebx+ 1980 ]
test ax,0FFFFH
jz im32_z510
mov [ecx+ 61500 ],ax
mov [ecx+ 63548 ],ax
im32_z510:
test eax,0FFFF0000H
jz im32_z511
mov [ecx+ 61502 ],ax
mov [ecx+ 63550 ],ax
im32_z511:
