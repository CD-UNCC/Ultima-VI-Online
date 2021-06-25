mov eax,[ebx+ 0 ]
test ax,0FFFFH
jz sf32_z0
mov [ecx+ 0 ],ax
mov [ecx+ 2048 ],ax
sf32_z0:
test eax,0FFFF0000H
jz sf32_z1
mov [ecx+ 2 ],ax
mov [ecx+ 2050 ],ax
sf32_z1:
mov eax,[ebx+ 4 ]
test ax,0FFFFH
jz sf32_z2
mov [ecx+ 4 ],ax
mov [ecx+ 2052 ],ax
sf32_z2:
test eax,0FFFF0000H
jz sf32_z3
mov [ecx+ 6 ],ax
mov [ecx+ 2054 ],ax
sf32_z3:
mov eax,[ebx+ 8 ]
test ax,0FFFFH
jz sf32_z4
mov [ecx+ 8 ],ax
mov [ecx+ 2056 ],ax
sf32_z4:
test eax,0FFFF0000H
jz sf32_z5
mov [ecx+ 10 ],ax
mov [ecx+ 2058 ],ax
sf32_z5:
mov eax,[ebx+ 12 ]
test ax,0FFFFH
jz sf32_z6
mov [ecx+ 12 ],ax
mov [ecx+ 2060 ],ax
sf32_z6:
test eax,0FFFF0000H
jz sf32_z7
mov [ecx+ 14 ],ax
mov [ecx+ 2062 ],ax
sf32_z7:
mov eax,[ebx+ 16 ]
test ax,0FFFFH
jz sf32_z8
mov [ecx+ 16 ],ax
mov [ecx+ 2064 ],ax
sf32_z8:
test eax,0FFFF0000H
jz sf32_z9
mov [ecx+ 18 ],ax
mov [ecx+ 2066 ],ax
sf32_z9:
mov eax,[ebx+ 20 ]
test ax,0FFFFH
jz sf32_z10
mov [ecx+ 20 ],ax
mov [ecx+ 2068 ],ax
sf32_z10:
test eax,0FFFF0000H
jz sf32_z11
mov [ecx+ 22 ],ax
mov [ecx+ 2070 ],ax
sf32_z11:
mov eax,[ebx+ 24 ]
test ax,0FFFFH
jz sf32_z12
mov [ecx+ 24 ],ax
mov [ecx+ 2072 ],ax
sf32_z12:
test eax,0FFFF0000H
jz sf32_z13
mov [ecx+ 26 ],ax
mov [ecx+ 2074 ],ax
sf32_z13:
mov eax,[ebx+ 28 ]
test ax,0FFFFH
jz sf32_z14
mov [ecx+ 28 ],ax
mov [ecx+ 2076 ],ax
sf32_z14:
test eax,0FFFF0000H
jz sf32_z15
mov [ecx+ 30 ],ax
mov [ecx+ 2078 ],ax
sf32_z15:
mov eax,[ebx+ 32 ]
test ax,0FFFFH
jz sf32_z16
mov [ecx+ 32 ],ax
mov [ecx+ 2080 ],ax
sf32_z16:
test eax,0FFFF0000H
jz sf32_z17
mov [ecx+ 34 ],ax
mov [ecx+ 2082 ],ax
sf32_z17:
mov eax,[ebx+ 36 ]
test ax,0FFFFH
jz sf32_z18
mov [ecx+ 36 ],ax
mov [ecx+ 2084 ],ax
sf32_z18:
test eax,0FFFF0000H
jz sf32_z19
mov [ecx+ 38 ],ax
mov [ecx+ 2086 ],ax
sf32_z19:
mov eax,[ebx+ 40 ]
test ax,0FFFFH
jz sf32_z20
mov [ecx+ 40 ],ax
mov [ecx+ 2088 ],ax
sf32_z20:
test eax,0FFFF0000H
jz sf32_z21
mov [ecx+ 42 ],ax
mov [ecx+ 2090 ],ax
sf32_z21:
mov eax,[ebx+ 44 ]
test ax,0FFFFH
jz sf32_z22
mov [ecx+ 44 ],ax
mov [ecx+ 2092 ],ax
sf32_z22:
test eax,0FFFF0000H
jz sf32_z23
mov [ecx+ 46 ],ax
mov [ecx+ 2094 ],ax
sf32_z23:
mov eax,[ebx+ 48 ]
test ax,0FFFFH
jz sf32_z24
mov [ecx+ 48 ],ax
mov [ecx+ 2096 ],ax
sf32_z24:
test eax,0FFFF0000H
jz sf32_z25
mov [ecx+ 50 ],ax
mov [ecx+ 2098 ],ax
sf32_z25:
mov eax,[ebx+ 52 ]
test ax,0FFFFH
jz sf32_z26
mov [ecx+ 52 ],ax
mov [ecx+ 2100 ],ax
sf32_z26:
test eax,0FFFF0000H
jz sf32_z27
mov [ecx+ 54 ],ax
mov [ecx+ 2102 ],ax
sf32_z27:
mov eax,[ebx+ 56 ]
test ax,0FFFFH
jz sf32_z28
mov [ecx+ 56 ],ax
mov [ecx+ 2104 ],ax
sf32_z28:
test eax,0FFFF0000H
jz sf32_z29
mov [ecx+ 58 ],ax
mov [ecx+ 2106 ],ax
sf32_z29:
mov eax,[ebx+ 60 ]
test ax,0FFFFH
jz sf32_z30
mov [ecx+ 60 ],ax
mov [ecx+ 2108 ],ax
sf32_z30:
test eax,0FFFF0000H
jz sf32_z31
mov [ecx+ 62 ],ax
mov [ecx+ 2110 ],ax
sf32_z31:
mov eax,[ebx+ 4096 ]
test ax,0FFFFH
jz sf32_z32
mov [ecx+ 4096 ],ax
mov [ecx+ 6144 ],ax
sf32_z32:
test eax,0FFFF0000H
jz sf32_z33
mov [ecx+ 4098 ],ax
mov [ecx+ 6146 ],ax
sf32_z33:
mov eax,[ebx+ 4100 ]
test ax,0FFFFH
jz sf32_z34
mov [ecx+ 4100 ],ax
mov [ecx+ 6148 ],ax
sf32_z34:
test eax,0FFFF0000H
jz sf32_z35
mov [ecx+ 4102 ],ax
mov [ecx+ 6150 ],ax
sf32_z35:
mov eax,[ebx+ 4104 ]
test ax,0FFFFH
jz sf32_z36
mov [ecx+ 4104 ],ax
mov [ecx+ 6152 ],ax
sf32_z36:
test eax,0FFFF0000H
jz sf32_z37
mov [ecx+ 4106 ],ax
mov [ecx+ 6154 ],ax
sf32_z37:
mov eax,[ebx+ 4108 ]
test ax,0FFFFH
jz sf32_z38
mov [ecx+ 4108 ],ax
mov [ecx+ 6156 ],ax
sf32_z38:
test eax,0FFFF0000H
jz sf32_z39
mov [ecx+ 4110 ],ax
mov [ecx+ 6158 ],ax
sf32_z39:
mov eax,[ebx+ 4112 ]
test ax,0FFFFH
jz sf32_z40
mov [ecx+ 4112 ],ax
mov [ecx+ 6160 ],ax
sf32_z40:
test eax,0FFFF0000H
jz sf32_z41
mov [ecx+ 4114 ],ax
mov [ecx+ 6162 ],ax
sf32_z41:
mov eax,[ebx+ 4116 ]
test ax,0FFFFH
jz sf32_z42
mov [ecx+ 4116 ],ax
mov [ecx+ 6164 ],ax
sf32_z42:
test eax,0FFFF0000H
jz sf32_z43
mov [ecx+ 4118 ],ax
mov [ecx+ 6166 ],ax
sf32_z43:
mov eax,[ebx+ 4120 ]
test ax,0FFFFH
jz sf32_z44
mov [ecx+ 4120 ],ax
mov [ecx+ 6168 ],ax
sf32_z44:
test eax,0FFFF0000H
jz sf32_z45
mov [ecx+ 4122 ],ax
mov [ecx+ 6170 ],ax
sf32_z45:
mov eax,[ebx+ 4124 ]
test ax,0FFFFH
jz sf32_z46
mov [ecx+ 4124 ],ax
mov [ecx+ 6172 ],ax
sf32_z46:
test eax,0FFFF0000H
jz sf32_z47
mov [ecx+ 4126 ],ax
mov [ecx+ 6174 ],ax
sf32_z47:
mov eax,[ebx+ 4128 ]
test ax,0FFFFH
jz sf32_z48
mov [ecx+ 4128 ],ax
mov [ecx+ 6176 ],ax
sf32_z48:
test eax,0FFFF0000H
jz sf32_z49
mov [ecx+ 4130 ],ax
mov [ecx+ 6178 ],ax
sf32_z49:
mov eax,[ebx+ 4132 ]
test ax,0FFFFH
jz sf32_z50
mov [ecx+ 4132 ],ax
mov [ecx+ 6180 ],ax
sf32_z50:
test eax,0FFFF0000H
jz sf32_z51
mov [ecx+ 4134 ],ax
mov [ecx+ 6182 ],ax
sf32_z51:
mov eax,[ebx+ 4136 ]
test ax,0FFFFH
jz sf32_z52
mov [ecx+ 4136 ],ax
mov [ecx+ 6184 ],ax
sf32_z52:
test eax,0FFFF0000H
jz sf32_z53
mov [ecx+ 4138 ],ax
mov [ecx+ 6186 ],ax
sf32_z53:
mov eax,[ebx+ 4140 ]
test ax,0FFFFH
jz sf32_z54
mov [ecx+ 4140 ],ax
mov [ecx+ 6188 ],ax
sf32_z54:
test eax,0FFFF0000H
jz sf32_z55
mov [ecx+ 4142 ],ax
mov [ecx+ 6190 ],ax
sf32_z55:
mov eax,[ebx+ 4144 ]
test ax,0FFFFH
jz sf32_z56
mov [ecx+ 4144 ],ax
mov [ecx+ 6192 ],ax
sf32_z56:
test eax,0FFFF0000H
jz sf32_z57
mov [ecx+ 4146 ],ax
mov [ecx+ 6194 ],ax
sf32_z57:
mov eax,[ebx+ 4148 ]
test ax,0FFFFH
jz sf32_z58
mov [ecx+ 4148 ],ax
mov [ecx+ 6196 ],ax
sf32_z58:
test eax,0FFFF0000H
jz sf32_z59
mov [ecx+ 4150 ],ax
mov [ecx+ 6198 ],ax
sf32_z59:
mov eax,[ebx+ 4152 ]
test ax,0FFFFH
jz sf32_z60
mov [ecx+ 4152 ],ax
mov [ecx+ 6200 ],ax
sf32_z60:
test eax,0FFFF0000H
jz sf32_z61
mov [ecx+ 4154 ],ax
mov [ecx+ 6202 ],ax
sf32_z61:
mov eax,[ebx+ 4156 ]
test ax,0FFFFH
jz sf32_z62
mov [ecx+ 4156 ],ax
mov [ecx+ 6204 ],ax
sf32_z62:
test eax,0FFFF0000H
jz sf32_z63
mov [ecx+ 4158 ],ax
mov [ecx+ 6206 ],ax
sf32_z63:
mov eax,[ebx+ 8192 ]
test ax,0FFFFH
jz sf32_z64
mov [ecx+ 8192 ],ax
mov [ecx+ 10240 ],ax
sf32_z64:
test eax,0FFFF0000H
jz sf32_z65
mov [ecx+ 8194 ],ax
mov [ecx+ 10242 ],ax
sf32_z65:
mov eax,[ebx+ 8196 ]
test ax,0FFFFH
jz sf32_z66
mov [ecx+ 8196 ],ax
mov [ecx+ 10244 ],ax
sf32_z66:
test eax,0FFFF0000H
jz sf32_z67
mov [ecx+ 8198 ],ax
mov [ecx+ 10246 ],ax
sf32_z67:
mov eax,[ebx+ 8200 ]
test ax,0FFFFH
jz sf32_z68
mov [ecx+ 8200 ],ax
mov [ecx+ 10248 ],ax
sf32_z68:
test eax,0FFFF0000H
jz sf32_z69
mov [ecx+ 8202 ],ax
mov [ecx+ 10250 ],ax
sf32_z69:
mov eax,[ebx+ 8204 ]
test ax,0FFFFH
jz sf32_z70
mov [ecx+ 8204 ],ax
mov [ecx+ 10252 ],ax
sf32_z70:
test eax,0FFFF0000H
jz sf32_z71
mov [ecx+ 8206 ],ax
mov [ecx+ 10254 ],ax
sf32_z71:
mov eax,[ebx+ 8208 ]
test ax,0FFFFH
jz sf32_z72
mov [ecx+ 8208 ],ax
mov [ecx+ 10256 ],ax
sf32_z72:
test eax,0FFFF0000H
jz sf32_z73
mov [ecx+ 8210 ],ax
mov [ecx+ 10258 ],ax
sf32_z73:
mov eax,[ebx+ 8212 ]
test ax,0FFFFH
jz sf32_z74
mov [ecx+ 8212 ],ax
mov [ecx+ 10260 ],ax
sf32_z74:
test eax,0FFFF0000H
jz sf32_z75
mov [ecx+ 8214 ],ax
mov [ecx+ 10262 ],ax
sf32_z75:
mov eax,[ebx+ 8216 ]
test ax,0FFFFH
jz sf32_z76
mov [ecx+ 8216 ],ax
mov [ecx+ 10264 ],ax
sf32_z76:
test eax,0FFFF0000H
jz sf32_z77
mov [ecx+ 8218 ],ax
mov [ecx+ 10266 ],ax
sf32_z77:
mov eax,[ebx+ 8220 ]
test ax,0FFFFH
jz sf32_z78
mov [ecx+ 8220 ],ax
mov [ecx+ 10268 ],ax
sf32_z78:
test eax,0FFFF0000H
jz sf32_z79
mov [ecx+ 8222 ],ax
mov [ecx+ 10270 ],ax
sf32_z79:
mov eax,[ebx+ 8224 ]
test ax,0FFFFH
jz sf32_z80
mov [ecx+ 8224 ],ax
mov [ecx+ 10272 ],ax
sf32_z80:
test eax,0FFFF0000H
jz sf32_z81
mov [ecx+ 8226 ],ax
mov [ecx+ 10274 ],ax
sf32_z81:
mov eax,[ebx+ 8228 ]
test ax,0FFFFH
jz sf32_z82
mov [ecx+ 8228 ],ax
mov [ecx+ 10276 ],ax
sf32_z82:
test eax,0FFFF0000H
jz sf32_z83
mov [ecx+ 8230 ],ax
mov [ecx+ 10278 ],ax
sf32_z83:
mov eax,[ebx+ 8232 ]
test ax,0FFFFH
jz sf32_z84
mov [ecx+ 8232 ],ax
mov [ecx+ 10280 ],ax
sf32_z84:
test eax,0FFFF0000H
jz sf32_z85
mov [ecx+ 8234 ],ax
mov [ecx+ 10282 ],ax
sf32_z85:
mov eax,[ebx+ 8236 ]
test ax,0FFFFH
jz sf32_z86
mov [ecx+ 8236 ],ax
mov [ecx+ 10284 ],ax
sf32_z86:
test eax,0FFFF0000H
jz sf32_z87
mov [ecx+ 8238 ],ax
mov [ecx+ 10286 ],ax
sf32_z87:
mov eax,[ebx+ 8240 ]
test ax,0FFFFH
jz sf32_z88
mov [ecx+ 8240 ],ax
mov [ecx+ 10288 ],ax
sf32_z88:
test eax,0FFFF0000H
jz sf32_z89
mov [ecx+ 8242 ],ax
mov [ecx+ 10290 ],ax
sf32_z89:
mov eax,[ebx+ 8244 ]
test ax,0FFFFH
jz sf32_z90
mov [ecx+ 8244 ],ax
mov [ecx+ 10292 ],ax
sf32_z90:
test eax,0FFFF0000H
jz sf32_z91
mov [ecx+ 8246 ],ax
mov [ecx+ 10294 ],ax
sf32_z91:
mov eax,[ebx+ 8248 ]
test ax,0FFFFH
jz sf32_z92
mov [ecx+ 8248 ],ax
mov [ecx+ 10296 ],ax
sf32_z92:
test eax,0FFFF0000H
jz sf32_z93
mov [ecx+ 8250 ],ax
mov [ecx+ 10298 ],ax
sf32_z93:
mov eax,[ebx+ 8252 ]
test ax,0FFFFH
jz sf32_z94
mov [ecx+ 8252 ],ax
mov [ecx+ 10300 ],ax
sf32_z94:
test eax,0FFFF0000H
jz sf32_z95
mov [ecx+ 8254 ],ax
mov [ecx+ 10302 ],ax
sf32_z95:
mov eax,[ebx+ 12288 ]
test ax,0FFFFH
jz sf32_z96
mov [ecx+ 12288 ],ax
mov [ecx+ 14336 ],ax
sf32_z96:
test eax,0FFFF0000H
jz sf32_z97
mov [ecx+ 12290 ],ax
mov [ecx+ 14338 ],ax
sf32_z97:
mov eax,[ebx+ 12292 ]
test ax,0FFFFH
jz sf32_z98
mov [ecx+ 12292 ],ax
mov [ecx+ 14340 ],ax
sf32_z98:
test eax,0FFFF0000H
jz sf32_z99
mov [ecx+ 12294 ],ax
mov [ecx+ 14342 ],ax
sf32_z99:
mov eax,[ebx+ 12296 ]
test ax,0FFFFH
jz sf32_z100
mov [ecx+ 12296 ],ax
mov [ecx+ 14344 ],ax
sf32_z100:
test eax,0FFFF0000H
jz sf32_z101
mov [ecx+ 12298 ],ax
mov [ecx+ 14346 ],ax
sf32_z101:
mov eax,[ebx+ 12300 ]
test ax,0FFFFH
jz sf32_z102
mov [ecx+ 12300 ],ax
mov [ecx+ 14348 ],ax
sf32_z102:
test eax,0FFFF0000H
jz sf32_z103
mov [ecx+ 12302 ],ax
mov [ecx+ 14350 ],ax
sf32_z103:
mov eax,[ebx+ 12304 ]
test ax,0FFFFH
jz sf32_z104
mov [ecx+ 12304 ],ax
mov [ecx+ 14352 ],ax
sf32_z104:
test eax,0FFFF0000H
jz sf32_z105
mov [ecx+ 12306 ],ax
mov [ecx+ 14354 ],ax
sf32_z105:
mov eax,[ebx+ 12308 ]
test ax,0FFFFH
jz sf32_z106
mov [ecx+ 12308 ],ax
mov [ecx+ 14356 ],ax
sf32_z106:
test eax,0FFFF0000H
jz sf32_z107
mov [ecx+ 12310 ],ax
mov [ecx+ 14358 ],ax
sf32_z107:
mov eax,[ebx+ 12312 ]
test ax,0FFFFH
jz sf32_z108
mov [ecx+ 12312 ],ax
mov [ecx+ 14360 ],ax
sf32_z108:
test eax,0FFFF0000H
jz sf32_z109
mov [ecx+ 12314 ],ax
mov [ecx+ 14362 ],ax
sf32_z109:
mov eax,[ebx+ 12316 ]
test ax,0FFFFH
jz sf32_z110
mov [ecx+ 12316 ],ax
mov [ecx+ 14364 ],ax
sf32_z110:
test eax,0FFFF0000H
jz sf32_z111
mov [ecx+ 12318 ],ax
mov [ecx+ 14366 ],ax
sf32_z111:
mov eax,[ebx+ 12320 ]
test ax,0FFFFH
jz sf32_z112
mov [ecx+ 12320 ],ax
mov [ecx+ 14368 ],ax
sf32_z112:
test eax,0FFFF0000H
jz sf32_z113
mov [ecx+ 12322 ],ax
mov [ecx+ 14370 ],ax
sf32_z113:
mov eax,[ebx+ 12324 ]
test ax,0FFFFH
jz sf32_z114
mov [ecx+ 12324 ],ax
mov [ecx+ 14372 ],ax
sf32_z114:
test eax,0FFFF0000H
jz sf32_z115
mov [ecx+ 12326 ],ax
mov [ecx+ 14374 ],ax
sf32_z115:
mov eax,[ebx+ 12328 ]
test ax,0FFFFH
jz sf32_z116
mov [ecx+ 12328 ],ax
mov [ecx+ 14376 ],ax
sf32_z116:
test eax,0FFFF0000H
jz sf32_z117
mov [ecx+ 12330 ],ax
mov [ecx+ 14378 ],ax
sf32_z117:
mov eax,[ebx+ 12332 ]
test ax,0FFFFH
jz sf32_z118
mov [ecx+ 12332 ],ax
mov [ecx+ 14380 ],ax
sf32_z118:
test eax,0FFFF0000H
jz sf32_z119
mov [ecx+ 12334 ],ax
mov [ecx+ 14382 ],ax
sf32_z119:
mov eax,[ebx+ 12336 ]
test ax,0FFFFH
jz sf32_z120
mov [ecx+ 12336 ],ax
mov [ecx+ 14384 ],ax
sf32_z120:
test eax,0FFFF0000H
jz sf32_z121
mov [ecx+ 12338 ],ax
mov [ecx+ 14386 ],ax
sf32_z121:
mov eax,[ebx+ 12340 ]
test ax,0FFFFH
jz sf32_z122
mov [ecx+ 12340 ],ax
mov [ecx+ 14388 ],ax
sf32_z122:
test eax,0FFFF0000H
jz sf32_z123
mov [ecx+ 12342 ],ax
mov [ecx+ 14390 ],ax
sf32_z123:
mov eax,[ebx+ 12344 ]
test ax,0FFFFH
jz sf32_z124
mov [ecx+ 12344 ],ax
mov [ecx+ 14392 ],ax
sf32_z124:
test eax,0FFFF0000H
jz sf32_z125
mov [ecx+ 12346 ],ax
mov [ecx+ 14394 ],ax
sf32_z125:
mov eax,[ebx+ 12348 ]
test ax,0FFFFH
jz sf32_z126
mov [ecx+ 12348 ],ax
mov [ecx+ 14396 ],ax
sf32_z126:
test eax,0FFFF0000H
jz sf32_z127
mov [ecx+ 12350 ],ax
mov [ecx+ 14398 ],ax
sf32_z127:
mov eax,[ebx+ 16384 ]
test ax,0FFFFH
jz sf32_z128
mov [ecx+ 16384 ],ax
mov [ecx+ 18432 ],ax
sf32_z128:
test eax,0FFFF0000H
jz sf32_z129
mov [ecx+ 16386 ],ax
mov [ecx+ 18434 ],ax
sf32_z129:
mov eax,[ebx+ 16388 ]
test ax,0FFFFH
jz sf32_z130
mov [ecx+ 16388 ],ax
mov [ecx+ 18436 ],ax
sf32_z130:
test eax,0FFFF0000H
jz sf32_z131
mov [ecx+ 16390 ],ax
mov [ecx+ 18438 ],ax
sf32_z131:
mov eax,[ebx+ 16392 ]
test ax,0FFFFH
jz sf32_z132
mov [ecx+ 16392 ],ax
mov [ecx+ 18440 ],ax
sf32_z132:
test eax,0FFFF0000H
jz sf32_z133
mov [ecx+ 16394 ],ax
mov [ecx+ 18442 ],ax
sf32_z133:
mov eax,[ebx+ 16396 ]
test ax,0FFFFH
jz sf32_z134
mov [ecx+ 16396 ],ax
mov [ecx+ 18444 ],ax
sf32_z134:
test eax,0FFFF0000H
jz sf32_z135
mov [ecx+ 16398 ],ax
mov [ecx+ 18446 ],ax
sf32_z135:
mov eax,[ebx+ 16400 ]
test ax,0FFFFH
jz sf32_z136
mov [ecx+ 16400 ],ax
mov [ecx+ 18448 ],ax
sf32_z136:
test eax,0FFFF0000H
jz sf32_z137
mov [ecx+ 16402 ],ax
mov [ecx+ 18450 ],ax
sf32_z137:
mov eax,[ebx+ 16404 ]
test ax,0FFFFH
jz sf32_z138
mov [ecx+ 16404 ],ax
mov [ecx+ 18452 ],ax
sf32_z138:
test eax,0FFFF0000H
jz sf32_z139
mov [ecx+ 16406 ],ax
mov [ecx+ 18454 ],ax
sf32_z139:
mov eax,[ebx+ 16408 ]
test ax,0FFFFH
jz sf32_z140
mov [ecx+ 16408 ],ax
mov [ecx+ 18456 ],ax
sf32_z140:
test eax,0FFFF0000H
jz sf32_z141
mov [ecx+ 16410 ],ax
mov [ecx+ 18458 ],ax
sf32_z141:
mov eax,[ebx+ 16412 ]
test ax,0FFFFH
jz sf32_z142
mov [ecx+ 16412 ],ax
mov [ecx+ 18460 ],ax
sf32_z142:
test eax,0FFFF0000H
jz sf32_z143
mov [ecx+ 16414 ],ax
mov [ecx+ 18462 ],ax
sf32_z143:
mov eax,[ebx+ 16416 ]
test ax,0FFFFH
jz sf32_z144
mov [ecx+ 16416 ],ax
mov [ecx+ 18464 ],ax
sf32_z144:
test eax,0FFFF0000H
jz sf32_z145
mov [ecx+ 16418 ],ax
mov [ecx+ 18466 ],ax
sf32_z145:
mov eax,[ebx+ 16420 ]
test ax,0FFFFH
jz sf32_z146
mov [ecx+ 16420 ],ax
mov [ecx+ 18468 ],ax
sf32_z146:
test eax,0FFFF0000H
jz sf32_z147
mov [ecx+ 16422 ],ax
mov [ecx+ 18470 ],ax
sf32_z147:
mov eax,[ebx+ 16424 ]
test ax,0FFFFH
jz sf32_z148
mov [ecx+ 16424 ],ax
mov [ecx+ 18472 ],ax
sf32_z148:
test eax,0FFFF0000H
jz sf32_z149
mov [ecx+ 16426 ],ax
mov [ecx+ 18474 ],ax
sf32_z149:
mov eax,[ebx+ 16428 ]
test ax,0FFFFH
jz sf32_z150
mov [ecx+ 16428 ],ax
mov [ecx+ 18476 ],ax
sf32_z150:
test eax,0FFFF0000H
jz sf32_z151
mov [ecx+ 16430 ],ax
mov [ecx+ 18478 ],ax
sf32_z151:
mov eax,[ebx+ 16432 ]
test ax,0FFFFH
jz sf32_z152
mov [ecx+ 16432 ],ax
mov [ecx+ 18480 ],ax
sf32_z152:
test eax,0FFFF0000H
jz sf32_z153
mov [ecx+ 16434 ],ax
mov [ecx+ 18482 ],ax
sf32_z153:
mov eax,[ebx+ 16436 ]
test ax,0FFFFH
jz sf32_z154
mov [ecx+ 16436 ],ax
mov [ecx+ 18484 ],ax
sf32_z154:
test eax,0FFFF0000H
jz sf32_z155
mov [ecx+ 16438 ],ax
mov [ecx+ 18486 ],ax
sf32_z155:
mov eax,[ebx+ 16440 ]
test ax,0FFFFH
jz sf32_z156
mov [ecx+ 16440 ],ax
mov [ecx+ 18488 ],ax
sf32_z156:
test eax,0FFFF0000H
jz sf32_z157
mov [ecx+ 16442 ],ax
mov [ecx+ 18490 ],ax
sf32_z157:
mov eax,[ebx+ 16444 ]
test ax,0FFFFH
jz sf32_z158
mov [ecx+ 16444 ],ax
mov [ecx+ 18492 ],ax
sf32_z158:
test eax,0FFFF0000H
jz sf32_z159
mov [ecx+ 16446 ],ax
mov [ecx+ 18494 ],ax
sf32_z159:
mov eax,[ebx+ 20480 ]
test ax,0FFFFH
jz sf32_z160
mov [ecx+ 20480 ],ax
mov [ecx+ 22528 ],ax
sf32_z160:
test eax,0FFFF0000H
jz sf32_z161
mov [ecx+ 20482 ],ax
mov [ecx+ 22530 ],ax
sf32_z161:
mov eax,[ebx+ 20484 ]
test ax,0FFFFH
jz sf32_z162
mov [ecx+ 20484 ],ax
mov [ecx+ 22532 ],ax
sf32_z162:
test eax,0FFFF0000H
jz sf32_z163
mov [ecx+ 20486 ],ax
mov [ecx+ 22534 ],ax
sf32_z163:
mov eax,[ebx+ 20488 ]
test ax,0FFFFH
jz sf32_z164
mov [ecx+ 20488 ],ax
mov [ecx+ 22536 ],ax
sf32_z164:
test eax,0FFFF0000H
jz sf32_z165
mov [ecx+ 20490 ],ax
mov [ecx+ 22538 ],ax
sf32_z165:
mov eax,[ebx+ 20492 ]
test ax,0FFFFH
jz sf32_z166
mov [ecx+ 20492 ],ax
mov [ecx+ 22540 ],ax
sf32_z166:
test eax,0FFFF0000H
jz sf32_z167
mov [ecx+ 20494 ],ax
mov [ecx+ 22542 ],ax
sf32_z167:
mov eax,[ebx+ 20496 ]
test ax,0FFFFH
jz sf32_z168
mov [ecx+ 20496 ],ax
mov [ecx+ 22544 ],ax
sf32_z168:
test eax,0FFFF0000H
jz sf32_z169
mov [ecx+ 20498 ],ax
mov [ecx+ 22546 ],ax
sf32_z169:
mov eax,[ebx+ 20500 ]
test ax,0FFFFH
jz sf32_z170
mov [ecx+ 20500 ],ax
mov [ecx+ 22548 ],ax
sf32_z170:
test eax,0FFFF0000H
jz sf32_z171
mov [ecx+ 20502 ],ax
mov [ecx+ 22550 ],ax
sf32_z171:
mov eax,[ebx+ 20504 ]
test ax,0FFFFH
jz sf32_z172
mov [ecx+ 20504 ],ax
mov [ecx+ 22552 ],ax
sf32_z172:
test eax,0FFFF0000H
jz sf32_z173
mov [ecx+ 20506 ],ax
mov [ecx+ 22554 ],ax
sf32_z173:
mov eax,[ebx+ 20508 ]
test ax,0FFFFH
jz sf32_z174
mov [ecx+ 20508 ],ax
mov [ecx+ 22556 ],ax
sf32_z174:
test eax,0FFFF0000H
jz sf32_z175
mov [ecx+ 20510 ],ax
mov [ecx+ 22558 ],ax
sf32_z175:
mov eax,[ebx+ 20512 ]
test ax,0FFFFH
jz sf32_z176
mov [ecx+ 20512 ],ax
mov [ecx+ 22560 ],ax
sf32_z176:
test eax,0FFFF0000H
jz sf32_z177
mov [ecx+ 20514 ],ax
mov [ecx+ 22562 ],ax
sf32_z177:
mov eax,[ebx+ 20516 ]
test ax,0FFFFH
jz sf32_z178
mov [ecx+ 20516 ],ax
mov [ecx+ 22564 ],ax
sf32_z178:
test eax,0FFFF0000H
jz sf32_z179
mov [ecx+ 20518 ],ax
mov [ecx+ 22566 ],ax
sf32_z179:
mov eax,[ebx+ 20520 ]
test ax,0FFFFH
jz sf32_z180
mov [ecx+ 20520 ],ax
mov [ecx+ 22568 ],ax
sf32_z180:
test eax,0FFFF0000H
jz sf32_z181
mov [ecx+ 20522 ],ax
mov [ecx+ 22570 ],ax
sf32_z181:
mov eax,[ebx+ 20524 ]
test ax,0FFFFH
jz sf32_z182
mov [ecx+ 20524 ],ax
mov [ecx+ 22572 ],ax
sf32_z182:
test eax,0FFFF0000H
jz sf32_z183
mov [ecx+ 20526 ],ax
mov [ecx+ 22574 ],ax
sf32_z183:
mov eax,[ebx+ 20528 ]
test ax,0FFFFH
jz sf32_z184
mov [ecx+ 20528 ],ax
mov [ecx+ 22576 ],ax
sf32_z184:
test eax,0FFFF0000H
jz sf32_z185
mov [ecx+ 20530 ],ax
mov [ecx+ 22578 ],ax
sf32_z185:
mov eax,[ebx+ 20532 ]
test ax,0FFFFH
jz sf32_z186
mov [ecx+ 20532 ],ax
mov [ecx+ 22580 ],ax
sf32_z186:
test eax,0FFFF0000H
jz sf32_z187
mov [ecx+ 20534 ],ax
mov [ecx+ 22582 ],ax
sf32_z187:
mov eax,[ebx+ 20536 ]
test ax,0FFFFH
jz sf32_z188
mov [ecx+ 20536 ],ax
mov [ecx+ 22584 ],ax
sf32_z188:
test eax,0FFFF0000H
jz sf32_z189
mov [ecx+ 20538 ],ax
mov [ecx+ 22586 ],ax
sf32_z189:
mov eax,[ebx+ 20540 ]
test ax,0FFFFH
jz sf32_z190
mov [ecx+ 20540 ],ax
mov [ecx+ 22588 ],ax
sf32_z190:
test eax,0FFFF0000H
jz sf32_z191
mov [ecx+ 20542 ],ax
mov [ecx+ 22590 ],ax
sf32_z191:
mov eax,[ebx+ 24576 ]
test ax,0FFFFH
jz sf32_z192
mov [ecx+ 24576 ],ax
mov [ecx+ 26624 ],ax
sf32_z192:
test eax,0FFFF0000H
jz sf32_z193
mov [ecx+ 24578 ],ax
mov [ecx+ 26626 ],ax
sf32_z193:
mov eax,[ebx+ 24580 ]
test ax,0FFFFH
jz sf32_z194
mov [ecx+ 24580 ],ax
mov [ecx+ 26628 ],ax
sf32_z194:
test eax,0FFFF0000H
jz sf32_z195
mov [ecx+ 24582 ],ax
mov [ecx+ 26630 ],ax
sf32_z195:
mov eax,[ebx+ 24584 ]
test ax,0FFFFH
jz sf32_z196
mov [ecx+ 24584 ],ax
mov [ecx+ 26632 ],ax
sf32_z196:
test eax,0FFFF0000H
jz sf32_z197
mov [ecx+ 24586 ],ax
mov [ecx+ 26634 ],ax
sf32_z197:
mov eax,[ebx+ 24588 ]
test ax,0FFFFH
jz sf32_z198
mov [ecx+ 24588 ],ax
mov [ecx+ 26636 ],ax
sf32_z198:
test eax,0FFFF0000H
jz sf32_z199
mov [ecx+ 24590 ],ax
mov [ecx+ 26638 ],ax
sf32_z199:
mov eax,[ebx+ 24592 ]
test ax,0FFFFH
jz sf32_z200
mov [ecx+ 24592 ],ax
mov [ecx+ 26640 ],ax
sf32_z200:
test eax,0FFFF0000H
jz sf32_z201
mov [ecx+ 24594 ],ax
mov [ecx+ 26642 ],ax
sf32_z201:
mov eax,[ebx+ 24596 ]
test ax,0FFFFH
jz sf32_z202
mov [ecx+ 24596 ],ax
mov [ecx+ 26644 ],ax
sf32_z202:
test eax,0FFFF0000H
jz sf32_z203
mov [ecx+ 24598 ],ax
mov [ecx+ 26646 ],ax
sf32_z203:
mov eax,[ebx+ 24600 ]
test ax,0FFFFH
jz sf32_z204
mov [ecx+ 24600 ],ax
mov [ecx+ 26648 ],ax
sf32_z204:
test eax,0FFFF0000H
jz sf32_z205
mov [ecx+ 24602 ],ax
mov [ecx+ 26650 ],ax
sf32_z205:
mov eax,[ebx+ 24604 ]
test ax,0FFFFH
jz sf32_z206
mov [ecx+ 24604 ],ax
mov [ecx+ 26652 ],ax
sf32_z206:
test eax,0FFFF0000H
jz sf32_z207
mov [ecx+ 24606 ],ax
mov [ecx+ 26654 ],ax
sf32_z207:
mov eax,[ebx+ 24608 ]
test ax,0FFFFH
jz sf32_z208
mov [ecx+ 24608 ],ax
mov [ecx+ 26656 ],ax
sf32_z208:
test eax,0FFFF0000H
jz sf32_z209
mov [ecx+ 24610 ],ax
mov [ecx+ 26658 ],ax
sf32_z209:
mov eax,[ebx+ 24612 ]
test ax,0FFFFH
jz sf32_z210
mov [ecx+ 24612 ],ax
mov [ecx+ 26660 ],ax
sf32_z210:
test eax,0FFFF0000H
jz sf32_z211
mov [ecx+ 24614 ],ax
mov [ecx+ 26662 ],ax
sf32_z211:
mov eax,[ebx+ 24616 ]
test ax,0FFFFH
jz sf32_z212
mov [ecx+ 24616 ],ax
mov [ecx+ 26664 ],ax
sf32_z212:
test eax,0FFFF0000H
jz sf32_z213
mov [ecx+ 24618 ],ax
mov [ecx+ 26666 ],ax
sf32_z213:
mov eax,[ebx+ 24620 ]
test ax,0FFFFH
jz sf32_z214
mov [ecx+ 24620 ],ax
mov [ecx+ 26668 ],ax
sf32_z214:
test eax,0FFFF0000H
jz sf32_z215
mov [ecx+ 24622 ],ax
mov [ecx+ 26670 ],ax
sf32_z215:
mov eax,[ebx+ 24624 ]
test ax,0FFFFH
jz sf32_z216
mov [ecx+ 24624 ],ax
mov [ecx+ 26672 ],ax
sf32_z216:
test eax,0FFFF0000H
jz sf32_z217
mov [ecx+ 24626 ],ax
mov [ecx+ 26674 ],ax
sf32_z217:
mov eax,[ebx+ 24628 ]
test ax,0FFFFH
jz sf32_z218
mov [ecx+ 24628 ],ax
mov [ecx+ 26676 ],ax
sf32_z218:
test eax,0FFFF0000H
jz sf32_z219
mov [ecx+ 24630 ],ax
mov [ecx+ 26678 ],ax
sf32_z219:
mov eax,[ebx+ 24632 ]
test ax,0FFFFH
jz sf32_z220
mov [ecx+ 24632 ],ax
mov [ecx+ 26680 ],ax
sf32_z220:
test eax,0FFFF0000H
jz sf32_z221
mov [ecx+ 24634 ],ax
mov [ecx+ 26682 ],ax
sf32_z221:
mov eax,[ebx+ 24636 ]
test ax,0FFFFH
jz sf32_z222
mov [ecx+ 24636 ],ax
mov [ecx+ 26684 ],ax
sf32_z222:
test eax,0FFFF0000H
jz sf32_z223
mov [ecx+ 24638 ],ax
mov [ecx+ 26686 ],ax
sf32_z223:
mov eax,[ebx+ 28672 ]
test ax,0FFFFH
jz sf32_z224
mov [ecx+ 28672 ],ax
mov [ecx+ 30720 ],ax
sf32_z224:
test eax,0FFFF0000H
jz sf32_z225
mov [ecx+ 28674 ],ax
mov [ecx+ 30722 ],ax
sf32_z225:
mov eax,[ebx+ 28676 ]
test ax,0FFFFH
jz sf32_z226
mov [ecx+ 28676 ],ax
mov [ecx+ 30724 ],ax
sf32_z226:
test eax,0FFFF0000H
jz sf32_z227
mov [ecx+ 28678 ],ax
mov [ecx+ 30726 ],ax
sf32_z227:
mov eax,[ebx+ 28680 ]
test ax,0FFFFH
jz sf32_z228
mov [ecx+ 28680 ],ax
mov [ecx+ 30728 ],ax
sf32_z228:
test eax,0FFFF0000H
jz sf32_z229
mov [ecx+ 28682 ],ax
mov [ecx+ 30730 ],ax
sf32_z229:
mov eax,[ebx+ 28684 ]
test ax,0FFFFH
jz sf32_z230
mov [ecx+ 28684 ],ax
mov [ecx+ 30732 ],ax
sf32_z230:
test eax,0FFFF0000H
jz sf32_z231
mov [ecx+ 28686 ],ax
mov [ecx+ 30734 ],ax
sf32_z231:
mov eax,[ebx+ 28688 ]
test ax,0FFFFH
jz sf32_z232
mov [ecx+ 28688 ],ax
mov [ecx+ 30736 ],ax
sf32_z232:
test eax,0FFFF0000H
jz sf32_z233
mov [ecx+ 28690 ],ax
mov [ecx+ 30738 ],ax
sf32_z233:
mov eax,[ebx+ 28692 ]
test ax,0FFFFH
jz sf32_z234
mov [ecx+ 28692 ],ax
mov [ecx+ 30740 ],ax
sf32_z234:
test eax,0FFFF0000H
jz sf32_z235
mov [ecx+ 28694 ],ax
mov [ecx+ 30742 ],ax
sf32_z235:
mov eax,[ebx+ 28696 ]
test ax,0FFFFH
jz sf32_z236
mov [ecx+ 28696 ],ax
mov [ecx+ 30744 ],ax
sf32_z236:
test eax,0FFFF0000H
jz sf32_z237
mov [ecx+ 28698 ],ax
mov [ecx+ 30746 ],ax
sf32_z237:
mov eax,[ebx+ 28700 ]
test ax,0FFFFH
jz sf32_z238
mov [ecx+ 28700 ],ax
mov [ecx+ 30748 ],ax
sf32_z238:
test eax,0FFFF0000H
jz sf32_z239
mov [ecx+ 28702 ],ax
mov [ecx+ 30750 ],ax
sf32_z239:
mov eax,[ebx+ 28704 ]
test ax,0FFFFH
jz sf32_z240
mov [ecx+ 28704 ],ax
mov [ecx+ 30752 ],ax
sf32_z240:
test eax,0FFFF0000H
jz sf32_z241
mov [ecx+ 28706 ],ax
mov [ecx+ 30754 ],ax
sf32_z241:
mov eax,[ebx+ 28708 ]
test ax,0FFFFH
jz sf32_z242
mov [ecx+ 28708 ],ax
mov [ecx+ 30756 ],ax
sf32_z242:
test eax,0FFFF0000H
jz sf32_z243
mov [ecx+ 28710 ],ax
mov [ecx+ 30758 ],ax
sf32_z243:
mov eax,[ebx+ 28712 ]
test ax,0FFFFH
jz sf32_z244
mov [ecx+ 28712 ],ax
mov [ecx+ 30760 ],ax
sf32_z244:
test eax,0FFFF0000H
jz sf32_z245
mov [ecx+ 28714 ],ax
mov [ecx+ 30762 ],ax
sf32_z245:
mov eax,[ebx+ 28716 ]
test ax,0FFFFH
jz sf32_z246
mov [ecx+ 28716 ],ax
mov [ecx+ 30764 ],ax
sf32_z246:
test eax,0FFFF0000H
jz sf32_z247
mov [ecx+ 28718 ],ax
mov [ecx+ 30766 ],ax
sf32_z247:
mov eax,[ebx+ 28720 ]
test ax,0FFFFH
jz sf32_z248
mov [ecx+ 28720 ],ax
mov [ecx+ 30768 ],ax
sf32_z248:
test eax,0FFFF0000H
jz sf32_z249
mov [ecx+ 28722 ],ax
mov [ecx+ 30770 ],ax
sf32_z249:
mov eax,[ebx+ 28724 ]
test ax,0FFFFH
jz sf32_z250
mov [ecx+ 28724 ],ax
mov [ecx+ 30772 ],ax
sf32_z250:
test eax,0FFFF0000H
jz sf32_z251
mov [ecx+ 28726 ],ax
mov [ecx+ 30774 ],ax
sf32_z251:
mov eax,[ebx+ 28728 ]
test ax,0FFFFH
jz sf32_z252
mov [ecx+ 28728 ],ax
mov [ecx+ 30776 ],ax
sf32_z252:
test eax,0FFFF0000H
jz sf32_z253
mov [ecx+ 28730 ],ax
mov [ecx+ 30778 ],ax
sf32_z253:
mov eax,[ebx+ 28732 ]
test ax,0FFFFH
jz sf32_z254
mov [ecx+ 28732 ],ax
mov [ecx+ 30780 ],ax
sf32_z254:
test eax,0FFFF0000H
jz sf32_z255
mov [ecx+ 28734 ],ax
mov [ecx+ 30782 ],ax
sf32_z255:
mov eax,[ebx+ 32768 ]
test ax,0FFFFH
jz sf32_z256
mov [ecx+ 32768 ],ax
mov [ecx+ 34816 ],ax
sf32_z256:
test eax,0FFFF0000H
jz sf32_z257
mov [ecx+ 32770 ],ax
mov [ecx+ 34818 ],ax
sf32_z257:
mov eax,[ebx+ 32772 ]
test ax,0FFFFH
jz sf32_z258
mov [ecx+ 32772 ],ax
mov [ecx+ 34820 ],ax
sf32_z258:
test eax,0FFFF0000H
jz sf32_z259
mov [ecx+ 32774 ],ax
mov [ecx+ 34822 ],ax
sf32_z259:
mov eax,[ebx+ 32776 ]
test ax,0FFFFH
jz sf32_z260
mov [ecx+ 32776 ],ax
mov [ecx+ 34824 ],ax
sf32_z260:
test eax,0FFFF0000H
jz sf32_z261
mov [ecx+ 32778 ],ax
mov [ecx+ 34826 ],ax
sf32_z261:
mov eax,[ebx+ 32780 ]
test ax,0FFFFH
jz sf32_z262
mov [ecx+ 32780 ],ax
mov [ecx+ 34828 ],ax
sf32_z262:
test eax,0FFFF0000H
jz sf32_z263
mov [ecx+ 32782 ],ax
mov [ecx+ 34830 ],ax
sf32_z263:
mov eax,[ebx+ 32784 ]
test ax,0FFFFH
jz sf32_z264
mov [ecx+ 32784 ],ax
mov [ecx+ 34832 ],ax
sf32_z264:
test eax,0FFFF0000H
jz sf32_z265
mov [ecx+ 32786 ],ax
mov [ecx+ 34834 ],ax
sf32_z265:
mov eax,[ebx+ 32788 ]
test ax,0FFFFH
jz sf32_z266
mov [ecx+ 32788 ],ax
mov [ecx+ 34836 ],ax
sf32_z266:
test eax,0FFFF0000H
jz sf32_z267
mov [ecx+ 32790 ],ax
mov [ecx+ 34838 ],ax
sf32_z267:
mov eax,[ebx+ 32792 ]
test ax,0FFFFH
jz sf32_z268
mov [ecx+ 32792 ],ax
mov [ecx+ 34840 ],ax
sf32_z268:
test eax,0FFFF0000H
jz sf32_z269
mov [ecx+ 32794 ],ax
mov [ecx+ 34842 ],ax
sf32_z269:
mov eax,[ebx+ 32796 ]
test ax,0FFFFH
jz sf32_z270
mov [ecx+ 32796 ],ax
mov [ecx+ 34844 ],ax
sf32_z270:
test eax,0FFFF0000H
jz sf32_z271
mov [ecx+ 32798 ],ax
mov [ecx+ 34846 ],ax
sf32_z271:
mov eax,[ebx+ 32800 ]
test ax,0FFFFH
jz sf32_z272
mov [ecx+ 32800 ],ax
mov [ecx+ 34848 ],ax
sf32_z272:
test eax,0FFFF0000H
jz sf32_z273
mov [ecx+ 32802 ],ax
mov [ecx+ 34850 ],ax
sf32_z273:
mov eax,[ebx+ 32804 ]
test ax,0FFFFH
jz sf32_z274
mov [ecx+ 32804 ],ax
mov [ecx+ 34852 ],ax
sf32_z274:
test eax,0FFFF0000H
jz sf32_z275
mov [ecx+ 32806 ],ax
mov [ecx+ 34854 ],ax
sf32_z275:
mov eax,[ebx+ 32808 ]
test ax,0FFFFH
jz sf32_z276
mov [ecx+ 32808 ],ax
mov [ecx+ 34856 ],ax
sf32_z276:
test eax,0FFFF0000H
jz sf32_z277
mov [ecx+ 32810 ],ax
mov [ecx+ 34858 ],ax
sf32_z277:
mov eax,[ebx+ 32812 ]
test ax,0FFFFH
jz sf32_z278
mov [ecx+ 32812 ],ax
mov [ecx+ 34860 ],ax
sf32_z278:
test eax,0FFFF0000H
jz sf32_z279
mov [ecx+ 32814 ],ax
mov [ecx+ 34862 ],ax
sf32_z279:
mov eax,[ebx+ 32816 ]
test ax,0FFFFH
jz sf32_z280
mov [ecx+ 32816 ],ax
mov [ecx+ 34864 ],ax
sf32_z280:
test eax,0FFFF0000H
jz sf32_z281
mov [ecx+ 32818 ],ax
mov [ecx+ 34866 ],ax
sf32_z281:
mov eax,[ebx+ 32820 ]
test ax,0FFFFH
jz sf32_z282
mov [ecx+ 32820 ],ax
mov [ecx+ 34868 ],ax
sf32_z282:
test eax,0FFFF0000H
jz sf32_z283
mov [ecx+ 32822 ],ax
mov [ecx+ 34870 ],ax
sf32_z283:
mov eax,[ebx+ 32824 ]
test ax,0FFFFH
jz sf32_z284
mov [ecx+ 32824 ],ax
mov [ecx+ 34872 ],ax
sf32_z284:
test eax,0FFFF0000H
jz sf32_z285
mov [ecx+ 32826 ],ax
mov [ecx+ 34874 ],ax
sf32_z285:
mov eax,[ebx+ 32828 ]
test ax,0FFFFH
jz sf32_z286
mov [ecx+ 32828 ],ax
mov [ecx+ 34876 ],ax
sf32_z286:
test eax,0FFFF0000H
jz sf32_z287
mov [ecx+ 32830 ],ax
mov [ecx+ 34878 ],ax
sf32_z287:
mov eax,[ebx+ 36864 ]
test ax,0FFFFH
jz sf32_z288
mov [ecx+ 36864 ],ax
mov [ecx+ 38912 ],ax
sf32_z288:
test eax,0FFFF0000H
jz sf32_z289
mov [ecx+ 36866 ],ax
mov [ecx+ 38914 ],ax
sf32_z289:
mov eax,[ebx+ 36868 ]
test ax,0FFFFH
jz sf32_z290
mov [ecx+ 36868 ],ax
mov [ecx+ 38916 ],ax
sf32_z290:
test eax,0FFFF0000H
jz sf32_z291
mov [ecx+ 36870 ],ax
mov [ecx+ 38918 ],ax
sf32_z291:
mov eax,[ebx+ 36872 ]
test ax,0FFFFH
jz sf32_z292
mov [ecx+ 36872 ],ax
mov [ecx+ 38920 ],ax
sf32_z292:
test eax,0FFFF0000H
jz sf32_z293
mov [ecx+ 36874 ],ax
mov [ecx+ 38922 ],ax
sf32_z293:
mov eax,[ebx+ 36876 ]
test ax,0FFFFH
jz sf32_z294
mov [ecx+ 36876 ],ax
mov [ecx+ 38924 ],ax
sf32_z294:
test eax,0FFFF0000H
jz sf32_z295
mov [ecx+ 36878 ],ax
mov [ecx+ 38926 ],ax
sf32_z295:
mov eax,[ebx+ 36880 ]
test ax,0FFFFH
jz sf32_z296
mov [ecx+ 36880 ],ax
mov [ecx+ 38928 ],ax
sf32_z296:
test eax,0FFFF0000H
jz sf32_z297
mov [ecx+ 36882 ],ax
mov [ecx+ 38930 ],ax
sf32_z297:
mov eax,[ebx+ 36884 ]
test ax,0FFFFH
jz sf32_z298
mov [ecx+ 36884 ],ax
mov [ecx+ 38932 ],ax
sf32_z298:
test eax,0FFFF0000H
jz sf32_z299
mov [ecx+ 36886 ],ax
mov [ecx+ 38934 ],ax
sf32_z299:
mov eax,[ebx+ 36888 ]
test ax,0FFFFH
jz sf32_z300
mov [ecx+ 36888 ],ax
mov [ecx+ 38936 ],ax
sf32_z300:
test eax,0FFFF0000H
jz sf32_z301
mov [ecx+ 36890 ],ax
mov [ecx+ 38938 ],ax
sf32_z301:
mov eax,[ebx+ 36892 ]
test ax,0FFFFH
jz sf32_z302
mov [ecx+ 36892 ],ax
mov [ecx+ 38940 ],ax
sf32_z302:
test eax,0FFFF0000H
jz sf32_z303
mov [ecx+ 36894 ],ax
mov [ecx+ 38942 ],ax
sf32_z303:
mov eax,[ebx+ 36896 ]
test ax,0FFFFH
jz sf32_z304
mov [ecx+ 36896 ],ax
mov [ecx+ 38944 ],ax
sf32_z304:
test eax,0FFFF0000H
jz sf32_z305
mov [ecx+ 36898 ],ax
mov [ecx+ 38946 ],ax
sf32_z305:
mov eax,[ebx+ 36900 ]
test ax,0FFFFH
jz sf32_z306
mov [ecx+ 36900 ],ax
mov [ecx+ 38948 ],ax
sf32_z306:
test eax,0FFFF0000H
jz sf32_z307
mov [ecx+ 36902 ],ax
mov [ecx+ 38950 ],ax
sf32_z307:
mov eax,[ebx+ 36904 ]
test ax,0FFFFH
jz sf32_z308
mov [ecx+ 36904 ],ax
mov [ecx+ 38952 ],ax
sf32_z308:
test eax,0FFFF0000H
jz sf32_z309
mov [ecx+ 36906 ],ax
mov [ecx+ 38954 ],ax
sf32_z309:
mov eax,[ebx+ 36908 ]
test ax,0FFFFH
jz sf32_z310
mov [ecx+ 36908 ],ax
mov [ecx+ 38956 ],ax
sf32_z310:
test eax,0FFFF0000H
jz sf32_z311
mov [ecx+ 36910 ],ax
mov [ecx+ 38958 ],ax
sf32_z311:
mov eax,[ebx+ 36912 ]
test ax,0FFFFH
jz sf32_z312
mov [ecx+ 36912 ],ax
mov [ecx+ 38960 ],ax
sf32_z312:
test eax,0FFFF0000H
jz sf32_z313
mov [ecx+ 36914 ],ax
mov [ecx+ 38962 ],ax
sf32_z313:
mov eax,[ebx+ 36916 ]
test ax,0FFFFH
jz sf32_z314
mov [ecx+ 36916 ],ax
mov [ecx+ 38964 ],ax
sf32_z314:
test eax,0FFFF0000H
jz sf32_z315
mov [ecx+ 36918 ],ax
mov [ecx+ 38966 ],ax
sf32_z315:
mov eax,[ebx+ 36920 ]
test ax,0FFFFH
jz sf32_z316
mov [ecx+ 36920 ],ax
mov [ecx+ 38968 ],ax
sf32_z316:
test eax,0FFFF0000H
jz sf32_z317
mov [ecx+ 36922 ],ax
mov [ecx+ 38970 ],ax
sf32_z317:
mov eax,[ebx+ 36924 ]
test ax,0FFFFH
jz sf32_z318
mov [ecx+ 36924 ],ax
mov [ecx+ 38972 ],ax
sf32_z318:
test eax,0FFFF0000H
jz sf32_z319
mov [ecx+ 36926 ],ax
mov [ecx+ 38974 ],ax
sf32_z319:
mov eax,[ebx+ 40960 ]
test ax,0FFFFH
jz sf32_z320
mov [ecx+ 40960 ],ax
mov [ecx+ 43008 ],ax
sf32_z320:
test eax,0FFFF0000H
jz sf32_z321
mov [ecx+ 40962 ],ax
mov [ecx+ 43010 ],ax
sf32_z321:
mov eax,[ebx+ 40964 ]
test ax,0FFFFH
jz sf32_z322
mov [ecx+ 40964 ],ax
mov [ecx+ 43012 ],ax
sf32_z322:
test eax,0FFFF0000H
jz sf32_z323
mov [ecx+ 40966 ],ax
mov [ecx+ 43014 ],ax
sf32_z323:
mov eax,[ebx+ 40968 ]
test ax,0FFFFH
jz sf32_z324
mov [ecx+ 40968 ],ax
mov [ecx+ 43016 ],ax
sf32_z324:
test eax,0FFFF0000H
jz sf32_z325
mov [ecx+ 40970 ],ax
mov [ecx+ 43018 ],ax
sf32_z325:
mov eax,[ebx+ 40972 ]
test ax,0FFFFH
jz sf32_z326
mov [ecx+ 40972 ],ax
mov [ecx+ 43020 ],ax
sf32_z326:
test eax,0FFFF0000H
jz sf32_z327
mov [ecx+ 40974 ],ax
mov [ecx+ 43022 ],ax
sf32_z327:
mov eax,[ebx+ 40976 ]
test ax,0FFFFH
jz sf32_z328
mov [ecx+ 40976 ],ax
mov [ecx+ 43024 ],ax
sf32_z328:
test eax,0FFFF0000H
jz sf32_z329
mov [ecx+ 40978 ],ax
mov [ecx+ 43026 ],ax
sf32_z329:
mov eax,[ebx+ 40980 ]
test ax,0FFFFH
jz sf32_z330
mov [ecx+ 40980 ],ax
mov [ecx+ 43028 ],ax
sf32_z330:
test eax,0FFFF0000H
jz sf32_z331
mov [ecx+ 40982 ],ax
mov [ecx+ 43030 ],ax
sf32_z331:
mov eax,[ebx+ 40984 ]
test ax,0FFFFH
jz sf32_z332
mov [ecx+ 40984 ],ax
mov [ecx+ 43032 ],ax
sf32_z332:
test eax,0FFFF0000H
jz sf32_z333
mov [ecx+ 40986 ],ax
mov [ecx+ 43034 ],ax
sf32_z333:
mov eax,[ebx+ 40988 ]
test ax,0FFFFH
jz sf32_z334
mov [ecx+ 40988 ],ax
mov [ecx+ 43036 ],ax
sf32_z334:
test eax,0FFFF0000H
jz sf32_z335
mov [ecx+ 40990 ],ax
mov [ecx+ 43038 ],ax
sf32_z335:
mov eax,[ebx+ 40992 ]
test ax,0FFFFH
jz sf32_z336
mov [ecx+ 40992 ],ax
mov [ecx+ 43040 ],ax
sf32_z336:
test eax,0FFFF0000H
jz sf32_z337
mov [ecx+ 40994 ],ax
mov [ecx+ 43042 ],ax
sf32_z337:
mov eax,[ebx+ 40996 ]
test ax,0FFFFH
jz sf32_z338
mov [ecx+ 40996 ],ax
mov [ecx+ 43044 ],ax
sf32_z338:
test eax,0FFFF0000H
jz sf32_z339
mov [ecx+ 40998 ],ax
mov [ecx+ 43046 ],ax
sf32_z339:
mov eax,[ebx+ 41000 ]
test ax,0FFFFH
jz sf32_z340
mov [ecx+ 41000 ],ax
mov [ecx+ 43048 ],ax
sf32_z340:
test eax,0FFFF0000H
jz sf32_z341
mov [ecx+ 41002 ],ax
mov [ecx+ 43050 ],ax
sf32_z341:
mov eax,[ebx+ 41004 ]
test ax,0FFFFH
jz sf32_z342
mov [ecx+ 41004 ],ax
mov [ecx+ 43052 ],ax
sf32_z342:
test eax,0FFFF0000H
jz sf32_z343
mov [ecx+ 41006 ],ax
mov [ecx+ 43054 ],ax
sf32_z343:
mov eax,[ebx+ 41008 ]
test ax,0FFFFH
jz sf32_z344
mov [ecx+ 41008 ],ax
mov [ecx+ 43056 ],ax
sf32_z344:
test eax,0FFFF0000H
jz sf32_z345
mov [ecx+ 41010 ],ax
mov [ecx+ 43058 ],ax
sf32_z345:
mov eax,[ebx+ 41012 ]
test ax,0FFFFH
jz sf32_z346
mov [ecx+ 41012 ],ax
mov [ecx+ 43060 ],ax
sf32_z346:
test eax,0FFFF0000H
jz sf32_z347
mov [ecx+ 41014 ],ax
mov [ecx+ 43062 ],ax
sf32_z347:
mov eax,[ebx+ 41016 ]
test ax,0FFFFH
jz sf32_z348
mov [ecx+ 41016 ],ax
mov [ecx+ 43064 ],ax
sf32_z348:
test eax,0FFFF0000H
jz sf32_z349
mov [ecx+ 41018 ],ax
mov [ecx+ 43066 ],ax
sf32_z349:
mov eax,[ebx+ 41020 ]
test ax,0FFFFH
jz sf32_z350
mov [ecx+ 41020 ],ax
mov [ecx+ 43068 ],ax
sf32_z350:
test eax,0FFFF0000H
jz sf32_z351
mov [ecx+ 41022 ],ax
mov [ecx+ 43070 ],ax
sf32_z351:
mov eax,[ebx+ 45056 ]
test ax,0FFFFH
jz sf32_z352
mov [ecx+ 45056 ],ax
mov [ecx+ 47104 ],ax
sf32_z352:
test eax,0FFFF0000H
jz sf32_z353
mov [ecx+ 45058 ],ax
mov [ecx+ 47106 ],ax
sf32_z353:
mov eax,[ebx+ 45060 ]
test ax,0FFFFH
jz sf32_z354
mov [ecx+ 45060 ],ax
mov [ecx+ 47108 ],ax
sf32_z354:
test eax,0FFFF0000H
jz sf32_z355
mov [ecx+ 45062 ],ax
mov [ecx+ 47110 ],ax
sf32_z355:
mov eax,[ebx+ 45064 ]
test ax,0FFFFH
jz sf32_z356
mov [ecx+ 45064 ],ax
mov [ecx+ 47112 ],ax
sf32_z356:
test eax,0FFFF0000H
jz sf32_z357
mov [ecx+ 45066 ],ax
mov [ecx+ 47114 ],ax
sf32_z357:
mov eax,[ebx+ 45068 ]
test ax,0FFFFH
jz sf32_z358
mov [ecx+ 45068 ],ax
mov [ecx+ 47116 ],ax
sf32_z358:
test eax,0FFFF0000H
jz sf32_z359
mov [ecx+ 45070 ],ax
mov [ecx+ 47118 ],ax
sf32_z359:
mov eax,[ebx+ 45072 ]
test ax,0FFFFH
jz sf32_z360
mov [ecx+ 45072 ],ax
mov [ecx+ 47120 ],ax
sf32_z360:
test eax,0FFFF0000H
jz sf32_z361
mov [ecx+ 45074 ],ax
mov [ecx+ 47122 ],ax
sf32_z361:
mov eax,[ebx+ 45076 ]
test ax,0FFFFH
jz sf32_z362
mov [ecx+ 45076 ],ax
mov [ecx+ 47124 ],ax
sf32_z362:
test eax,0FFFF0000H
jz sf32_z363
mov [ecx+ 45078 ],ax
mov [ecx+ 47126 ],ax
sf32_z363:
mov eax,[ebx+ 45080 ]
test ax,0FFFFH
jz sf32_z364
mov [ecx+ 45080 ],ax
mov [ecx+ 47128 ],ax
sf32_z364:
test eax,0FFFF0000H
jz sf32_z365
mov [ecx+ 45082 ],ax
mov [ecx+ 47130 ],ax
sf32_z365:
mov eax,[ebx+ 45084 ]
test ax,0FFFFH
jz sf32_z366
mov [ecx+ 45084 ],ax
mov [ecx+ 47132 ],ax
sf32_z366:
test eax,0FFFF0000H
jz sf32_z367
mov [ecx+ 45086 ],ax
mov [ecx+ 47134 ],ax
sf32_z367:
mov eax,[ebx+ 45088 ]
test ax,0FFFFH
jz sf32_z368
mov [ecx+ 45088 ],ax
mov [ecx+ 47136 ],ax
sf32_z368:
test eax,0FFFF0000H
jz sf32_z369
mov [ecx+ 45090 ],ax
mov [ecx+ 47138 ],ax
sf32_z369:
mov eax,[ebx+ 45092 ]
test ax,0FFFFH
jz sf32_z370
mov [ecx+ 45092 ],ax
mov [ecx+ 47140 ],ax
sf32_z370:
test eax,0FFFF0000H
jz sf32_z371
mov [ecx+ 45094 ],ax
mov [ecx+ 47142 ],ax
sf32_z371:
mov eax,[ebx+ 45096 ]
test ax,0FFFFH
jz sf32_z372
mov [ecx+ 45096 ],ax
mov [ecx+ 47144 ],ax
sf32_z372:
test eax,0FFFF0000H
jz sf32_z373
mov [ecx+ 45098 ],ax
mov [ecx+ 47146 ],ax
sf32_z373:
mov eax,[ebx+ 45100 ]
test ax,0FFFFH
jz sf32_z374
mov [ecx+ 45100 ],ax
mov [ecx+ 47148 ],ax
sf32_z374:
test eax,0FFFF0000H
jz sf32_z375
mov [ecx+ 45102 ],ax
mov [ecx+ 47150 ],ax
sf32_z375:
mov eax,[ebx+ 45104 ]
test ax,0FFFFH
jz sf32_z376
mov [ecx+ 45104 ],ax
mov [ecx+ 47152 ],ax
sf32_z376:
test eax,0FFFF0000H
jz sf32_z377
mov [ecx+ 45106 ],ax
mov [ecx+ 47154 ],ax
sf32_z377:
mov eax,[ebx+ 45108 ]
test ax,0FFFFH
jz sf32_z378
mov [ecx+ 45108 ],ax
mov [ecx+ 47156 ],ax
sf32_z378:
test eax,0FFFF0000H
jz sf32_z379
mov [ecx+ 45110 ],ax
mov [ecx+ 47158 ],ax
sf32_z379:
mov eax,[ebx+ 45112 ]
test ax,0FFFFH
jz sf32_z380
mov [ecx+ 45112 ],ax
mov [ecx+ 47160 ],ax
sf32_z380:
test eax,0FFFF0000H
jz sf32_z381
mov [ecx+ 45114 ],ax
mov [ecx+ 47162 ],ax
sf32_z381:
mov eax,[ebx+ 45116 ]
test ax,0FFFFH
jz sf32_z382
mov [ecx+ 45116 ],ax
mov [ecx+ 47164 ],ax
sf32_z382:
test eax,0FFFF0000H
jz sf32_z383
mov [ecx+ 45118 ],ax
mov [ecx+ 47166 ],ax
sf32_z383:
mov eax,[ebx+ 49152 ]
test ax,0FFFFH
jz sf32_z384
mov [ecx+ 49152 ],ax
mov [ecx+ 51200 ],ax
sf32_z384:
test eax,0FFFF0000H
jz sf32_z385
mov [ecx+ 49154 ],ax
mov [ecx+ 51202 ],ax
sf32_z385:
mov eax,[ebx+ 49156 ]
test ax,0FFFFH
jz sf32_z386
mov [ecx+ 49156 ],ax
mov [ecx+ 51204 ],ax
sf32_z386:
test eax,0FFFF0000H
jz sf32_z387
mov [ecx+ 49158 ],ax
mov [ecx+ 51206 ],ax
sf32_z387:
mov eax,[ebx+ 49160 ]
test ax,0FFFFH
jz sf32_z388
mov [ecx+ 49160 ],ax
mov [ecx+ 51208 ],ax
sf32_z388:
test eax,0FFFF0000H
jz sf32_z389
mov [ecx+ 49162 ],ax
mov [ecx+ 51210 ],ax
sf32_z389:
mov eax,[ebx+ 49164 ]
test ax,0FFFFH
jz sf32_z390
mov [ecx+ 49164 ],ax
mov [ecx+ 51212 ],ax
sf32_z390:
test eax,0FFFF0000H
jz sf32_z391
mov [ecx+ 49166 ],ax
mov [ecx+ 51214 ],ax
sf32_z391:
mov eax,[ebx+ 49168 ]
test ax,0FFFFH
jz sf32_z392
mov [ecx+ 49168 ],ax
mov [ecx+ 51216 ],ax
sf32_z392:
test eax,0FFFF0000H
jz sf32_z393
mov [ecx+ 49170 ],ax
mov [ecx+ 51218 ],ax
sf32_z393:
mov eax,[ebx+ 49172 ]
test ax,0FFFFH
jz sf32_z394
mov [ecx+ 49172 ],ax
mov [ecx+ 51220 ],ax
sf32_z394:
test eax,0FFFF0000H
jz sf32_z395
mov [ecx+ 49174 ],ax
mov [ecx+ 51222 ],ax
sf32_z395:
mov eax,[ebx+ 49176 ]
test ax,0FFFFH
jz sf32_z396
mov [ecx+ 49176 ],ax
mov [ecx+ 51224 ],ax
sf32_z396:
test eax,0FFFF0000H
jz sf32_z397
mov [ecx+ 49178 ],ax
mov [ecx+ 51226 ],ax
sf32_z397:
mov eax,[ebx+ 49180 ]
test ax,0FFFFH
jz sf32_z398
mov [ecx+ 49180 ],ax
mov [ecx+ 51228 ],ax
sf32_z398:
test eax,0FFFF0000H
jz sf32_z399
mov [ecx+ 49182 ],ax
mov [ecx+ 51230 ],ax
sf32_z399:
mov eax,[ebx+ 49184 ]
test ax,0FFFFH
jz sf32_z400
mov [ecx+ 49184 ],ax
mov [ecx+ 51232 ],ax
sf32_z400:
test eax,0FFFF0000H
jz sf32_z401
mov [ecx+ 49186 ],ax
mov [ecx+ 51234 ],ax
sf32_z401:
mov eax,[ebx+ 49188 ]
test ax,0FFFFH
jz sf32_z402
mov [ecx+ 49188 ],ax
mov [ecx+ 51236 ],ax
sf32_z402:
test eax,0FFFF0000H
jz sf32_z403
mov [ecx+ 49190 ],ax
mov [ecx+ 51238 ],ax
sf32_z403:
mov eax,[ebx+ 49192 ]
test ax,0FFFFH
jz sf32_z404
mov [ecx+ 49192 ],ax
mov [ecx+ 51240 ],ax
sf32_z404:
test eax,0FFFF0000H
jz sf32_z405
mov [ecx+ 49194 ],ax
mov [ecx+ 51242 ],ax
sf32_z405:
mov eax,[ebx+ 49196 ]
test ax,0FFFFH
jz sf32_z406
mov [ecx+ 49196 ],ax
mov [ecx+ 51244 ],ax
sf32_z406:
test eax,0FFFF0000H
jz sf32_z407
mov [ecx+ 49198 ],ax
mov [ecx+ 51246 ],ax
sf32_z407:
mov eax,[ebx+ 49200 ]
test ax,0FFFFH
jz sf32_z408
mov [ecx+ 49200 ],ax
mov [ecx+ 51248 ],ax
sf32_z408:
test eax,0FFFF0000H
jz sf32_z409
mov [ecx+ 49202 ],ax
mov [ecx+ 51250 ],ax
sf32_z409:
mov eax,[ebx+ 49204 ]
test ax,0FFFFH
jz sf32_z410
mov [ecx+ 49204 ],ax
mov [ecx+ 51252 ],ax
sf32_z410:
test eax,0FFFF0000H
jz sf32_z411
mov [ecx+ 49206 ],ax
mov [ecx+ 51254 ],ax
sf32_z411:
mov eax,[ebx+ 49208 ]
test ax,0FFFFH
jz sf32_z412
mov [ecx+ 49208 ],ax
mov [ecx+ 51256 ],ax
sf32_z412:
test eax,0FFFF0000H
jz sf32_z413
mov [ecx+ 49210 ],ax
mov [ecx+ 51258 ],ax
sf32_z413:
mov eax,[ebx+ 49212 ]
test ax,0FFFFH
jz sf32_z414
mov [ecx+ 49212 ],ax
mov [ecx+ 51260 ],ax
sf32_z414:
test eax,0FFFF0000H
jz sf32_z415
mov [ecx+ 49214 ],ax
mov [ecx+ 51262 ],ax
sf32_z415:
mov eax,[ebx+ 53248 ]
test ax,0FFFFH
jz sf32_z416
mov [ecx+ 53248 ],ax
mov [ecx+ 55296 ],ax
sf32_z416:
test eax,0FFFF0000H
jz sf32_z417
mov [ecx+ 53250 ],ax
mov [ecx+ 55298 ],ax
sf32_z417:
mov eax,[ebx+ 53252 ]
test ax,0FFFFH
jz sf32_z418
mov [ecx+ 53252 ],ax
mov [ecx+ 55300 ],ax
sf32_z418:
test eax,0FFFF0000H
jz sf32_z419
mov [ecx+ 53254 ],ax
mov [ecx+ 55302 ],ax
sf32_z419:
mov eax,[ebx+ 53256 ]
test ax,0FFFFH
jz sf32_z420
mov [ecx+ 53256 ],ax
mov [ecx+ 55304 ],ax
sf32_z420:
test eax,0FFFF0000H
jz sf32_z421
mov [ecx+ 53258 ],ax
mov [ecx+ 55306 ],ax
sf32_z421:
mov eax,[ebx+ 53260 ]
test ax,0FFFFH
jz sf32_z422
mov [ecx+ 53260 ],ax
mov [ecx+ 55308 ],ax
sf32_z422:
test eax,0FFFF0000H
jz sf32_z423
mov [ecx+ 53262 ],ax
mov [ecx+ 55310 ],ax
sf32_z423:
mov eax,[ebx+ 53264 ]
test ax,0FFFFH
jz sf32_z424
mov [ecx+ 53264 ],ax
mov [ecx+ 55312 ],ax
sf32_z424:
test eax,0FFFF0000H
jz sf32_z425
mov [ecx+ 53266 ],ax
mov [ecx+ 55314 ],ax
sf32_z425:
mov eax,[ebx+ 53268 ]
test ax,0FFFFH
jz sf32_z426
mov [ecx+ 53268 ],ax
mov [ecx+ 55316 ],ax
sf32_z426:
test eax,0FFFF0000H
jz sf32_z427
mov [ecx+ 53270 ],ax
mov [ecx+ 55318 ],ax
sf32_z427:
mov eax,[ebx+ 53272 ]
test ax,0FFFFH
jz sf32_z428
mov [ecx+ 53272 ],ax
mov [ecx+ 55320 ],ax
sf32_z428:
test eax,0FFFF0000H
jz sf32_z429
mov [ecx+ 53274 ],ax
mov [ecx+ 55322 ],ax
sf32_z429:
mov eax,[ebx+ 53276 ]
test ax,0FFFFH
jz sf32_z430
mov [ecx+ 53276 ],ax
mov [ecx+ 55324 ],ax
sf32_z430:
test eax,0FFFF0000H
jz sf32_z431
mov [ecx+ 53278 ],ax
mov [ecx+ 55326 ],ax
sf32_z431:
mov eax,[ebx+ 53280 ]
test ax,0FFFFH
jz sf32_z432
mov [ecx+ 53280 ],ax
mov [ecx+ 55328 ],ax
sf32_z432:
test eax,0FFFF0000H
jz sf32_z433
mov [ecx+ 53282 ],ax
mov [ecx+ 55330 ],ax
sf32_z433:
mov eax,[ebx+ 53284 ]
test ax,0FFFFH
jz sf32_z434
mov [ecx+ 53284 ],ax
mov [ecx+ 55332 ],ax
sf32_z434:
test eax,0FFFF0000H
jz sf32_z435
mov [ecx+ 53286 ],ax
mov [ecx+ 55334 ],ax
sf32_z435:
mov eax,[ebx+ 53288 ]
test ax,0FFFFH
jz sf32_z436
mov [ecx+ 53288 ],ax
mov [ecx+ 55336 ],ax
sf32_z436:
test eax,0FFFF0000H
jz sf32_z437
mov [ecx+ 53290 ],ax
mov [ecx+ 55338 ],ax
sf32_z437:
mov eax,[ebx+ 53292 ]
test ax,0FFFFH
jz sf32_z438
mov [ecx+ 53292 ],ax
mov [ecx+ 55340 ],ax
sf32_z438:
test eax,0FFFF0000H
jz sf32_z439
mov [ecx+ 53294 ],ax
mov [ecx+ 55342 ],ax
sf32_z439:
mov eax,[ebx+ 53296 ]
test ax,0FFFFH
jz sf32_z440
mov [ecx+ 53296 ],ax
mov [ecx+ 55344 ],ax
sf32_z440:
test eax,0FFFF0000H
jz sf32_z441
mov [ecx+ 53298 ],ax
mov [ecx+ 55346 ],ax
sf32_z441:
mov eax,[ebx+ 53300 ]
test ax,0FFFFH
jz sf32_z442
mov [ecx+ 53300 ],ax
mov [ecx+ 55348 ],ax
sf32_z442:
test eax,0FFFF0000H
jz sf32_z443
mov [ecx+ 53302 ],ax
mov [ecx+ 55350 ],ax
sf32_z443:
mov eax,[ebx+ 53304 ]
test ax,0FFFFH
jz sf32_z444
mov [ecx+ 53304 ],ax
mov [ecx+ 55352 ],ax
sf32_z444:
test eax,0FFFF0000H
jz sf32_z445
mov [ecx+ 53306 ],ax
mov [ecx+ 55354 ],ax
sf32_z445:
mov eax,[ebx+ 53308 ]
test ax,0FFFFH
jz sf32_z446
mov [ecx+ 53308 ],ax
mov [ecx+ 55356 ],ax
sf32_z446:
test eax,0FFFF0000H
jz sf32_z447
mov [ecx+ 53310 ],ax
mov [ecx+ 55358 ],ax
sf32_z447:
mov eax,[ebx+ 57344 ]
test ax,0FFFFH
jz sf32_z448
mov [ecx+ 57344 ],ax
mov [ecx+ 59392 ],ax
sf32_z448:
test eax,0FFFF0000H
jz sf32_z449
mov [ecx+ 57346 ],ax
mov [ecx+ 59394 ],ax
sf32_z449:
mov eax,[ebx+ 57348 ]
test ax,0FFFFH
jz sf32_z450
mov [ecx+ 57348 ],ax
mov [ecx+ 59396 ],ax
sf32_z450:
test eax,0FFFF0000H
jz sf32_z451
mov [ecx+ 57350 ],ax
mov [ecx+ 59398 ],ax
sf32_z451:
mov eax,[ebx+ 57352 ]
test ax,0FFFFH
jz sf32_z452
mov [ecx+ 57352 ],ax
mov [ecx+ 59400 ],ax
sf32_z452:
test eax,0FFFF0000H
jz sf32_z453
mov [ecx+ 57354 ],ax
mov [ecx+ 59402 ],ax
sf32_z453:
mov eax,[ebx+ 57356 ]
test ax,0FFFFH
jz sf32_z454
mov [ecx+ 57356 ],ax
mov [ecx+ 59404 ],ax
sf32_z454:
test eax,0FFFF0000H
jz sf32_z455
mov [ecx+ 57358 ],ax
mov [ecx+ 59406 ],ax
sf32_z455:
mov eax,[ebx+ 57360 ]
test ax,0FFFFH
jz sf32_z456
mov [ecx+ 57360 ],ax
mov [ecx+ 59408 ],ax
sf32_z456:
test eax,0FFFF0000H
jz sf32_z457
mov [ecx+ 57362 ],ax
mov [ecx+ 59410 ],ax
sf32_z457:
mov eax,[ebx+ 57364 ]
test ax,0FFFFH
jz sf32_z458
mov [ecx+ 57364 ],ax
mov [ecx+ 59412 ],ax
sf32_z458:
test eax,0FFFF0000H
jz sf32_z459
mov [ecx+ 57366 ],ax
mov [ecx+ 59414 ],ax
sf32_z459:
mov eax,[ebx+ 57368 ]
test ax,0FFFFH
jz sf32_z460
mov [ecx+ 57368 ],ax
mov [ecx+ 59416 ],ax
sf32_z460:
test eax,0FFFF0000H
jz sf32_z461
mov [ecx+ 57370 ],ax
mov [ecx+ 59418 ],ax
sf32_z461:
mov eax,[ebx+ 57372 ]
test ax,0FFFFH
jz sf32_z462
mov [ecx+ 57372 ],ax
mov [ecx+ 59420 ],ax
sf32_z462:
test eax,0FFFF0000H
jz sf32_z463
mov [ecx+ 57374 ],ax
mov [ecx+ 59422 ],ax
sf32_z463:
mov eax,[ebx+ 57376 ]
test ax,0FFFFH
jz sf32_z464
mov [ecx+ 57376 ],ax
mov [ecx+ 59424 ],ax
sf32_z464:
test eax,0FFFF0000H
jz sf32_z465
mov [ecx+ 57378 ],ax
mov [ecx+ 59426 ],ax
sf32_z465:
mov eax,[ebx+ 57380 ]
test ax,0FFFFH
jz sf32_z466
mov [ecx+ 57380 ],ax
mov [ecx+ 59428 ],ax
sf32_z466:
test eax,0FFFF0000H
jz sf32_z467
mov [ecx+ 57382 ],ax
mov [ecx+ 59430 ],ax
sf32_z467:
mov eax,[ebx+ 57384 ]
test ax,0FFFFH
jz sf32_z468
mov [ecx+ 57384 ],ax
mov [ecx+ 59432 ],ax
sf32_z468:
test eax,0FFFF0000H
jz sf32_z469
mov [ecx+ 57386 ],ax
mov [ecx+ 59434 ],ax
sf32_z469:
mov eax,[ebx+ 57388 ]
test ax,0FFFFH
jz sf32_z470
mov [ecx+ 57388 ],ax
mov [ecx+ 59436 ],ax
sf32_z470:
test eax,0FFFF0000H
jz sf32_z471
mov [ecx+ 57390 ],ax
mov [ecx+ 59438 ],ax
sf32_z471:
mov eax,[ebx+ 57392 ]
test ax,0FFFFH
jz sf32_z472
mov [ecx+ 57392 ],ax
mov [ecx+ 59440 ],ax
sf32_z472:
test eax,0FFFF0000H
jz sf32_z473
mov [ecx+ 57394 ],ax
mov [ecx+ 59442 ],ax
sf32_z473:
mov eax,[ebx+ 57396 ]
test ax,0FFFFH
jz sf32_z474
mov [ecx+ 57396 ],ax
mov [ecx+ 59444 ],ax
sf32_z474:
test eax,0FFFF0000H
jz sf32_z475
mov [ecx+ 57398 ],ax
mov [ecx+ 59446 ],ax
sf32_z475:
mov eax,[ebx+ 57400 ]
test ax,0FFFFH
jz sf32_z476
mov [ecx+ 57400 ],ax
mov [ecx+ 59448 ],ax
sf32_z476:
test eax,0FFFF0000H
jz sf32_z477
mov [ecx+ 57402 ],ax
mov [ecx+ 59450 ],ax
sf32_z477:
mov eax,[ebx+ 57404 ]
test ax,0FFFFH
jz sf32_z478
mov [ecx+ 57404 ],ax
mov [ecx+ 59452 ],ax
sf32_z478:
test eax,0FFFF0000H
jz sf32_z479
mov [ecx+ 57406 ],ax
mov [ecx+ 59454 ],ax
sf32_z479:
mov eax,[ebx+ 61440 ]
test ax,0FFFFH
jz sf32_z480
mov [ecx+ 61440 ],ax
mov [ecx+ 63488 ],ax
sf32_z480:
test eax,0FFFF0000H
jz sf32_z481
mov [ecx+ 61442 ],ax
mov [ecx+ 63490 ],ax
sf32_z481:
mov eax,[ebx+ 61444 ]
test ax,0FFFFH
jz sf32_z482
mov [ecx+ 61444 ],ax
mov [ecx+ 63492 ],ax
sf32_z482:
test eax,0FFFF0000H
jz sf32_z483
mov [ecx+ 61446 ],ax
mov [ecx+ 63494 ],ax
sf32_z483:
mov eax,[ebx+ 61448 ]
test ax,0FFFFH
jz sf32_z484
mov [ecx+ 61448 ],ax
mov [ecx+ 63496 ],ax
sf32_z484:
test eax,0FFFF0000H
jz sf32_z485
mov [ecx+ 61450 ],ax
mov [ecx+ 63498 ],ax
sf32_z485:
mov eax,[ebx+ 61452 ]
test ax,0FFFFH
jz sf32_z486
mov [ecx+ 61452 ],ax
mov [ecx+ 63500 ],ax
sf32_z486:
test eax,0FFFF0000H
jz sf32_z487
mov [ecx+ 61454 ],ax
mov [ecx+ 63502 ],ax
sf32_z487:
mov eax,[ebx+ 61456 ]
test ax,0FFFFH
jz sf32_z488
mov [ecx+ 61456 ],ax
mov [ecx+ 63504 ],ax
sf32_z488:
test eax,0FFFF0000H
jz sf32_z489
mov [ecx+ 61458 ],ax
mov [ecx+ 63506 ],ax
sf32_z489:
mov eax,[ebx+ 61460 ]
test ax,0FFFFH
jz sf32_z490
mov [ecx+ 61460 ],ax
mov [ecx+ 63508 ],ax
sf32_z490:
test eax,0FFFF0000H
jz sf32_z491
mov [ecx+ 61462 ],ax
mov [ecx+ 63510 ],ax
sf32_z491:
mov eax,[ebx+ 61464 ]
test ax,0FFFFH
jz sf32_z492
mov [ecx+ 61464 ],ax
mov [ecx+ 63512 ],ax
sf32_z492:
test eax,0FFFF0000H
jz sf32_z493
mov [ecx+ 61466 ],ax
mov [ecx+ 63514 ],ax
sf32_z493:
mov eax,[ebx+ 61468 ]
test ax,0FFFFH
jz sf32_z494
mov [ecx+ 61468 ],ax
mov [ecx+ 63516 ],ax
sf32_z494:
test eax,0FFFF0000H
jz sf32_z495
mov [ecx+ 61470 ],ax
mov [ecx+ 63518 ],ax
sf32_z495:
mov eax,[ebx+ 61472 ]
test ax,0FFFFH
jz sf32_z496
mov [ecx+ 61472 ],ax
mov [ecx+ 63520 ],ax
sf32_z496:
test eax,0FFFF0000H
jz sf32_z497
mov [ecx+ 61474 ],ax
mov [ecx+ 63522 ],ax
sf32_z497:
mov eax,[ebx+ 61476 ]
test ax,0FFFFH
jz sf32_z498
mov [ecx+ 61476 ],ax
mov [ecx+ 63524 ],ax
sf32_z498:
test eax,0FFFF0000H
jz sf32_z499
mov [ecx+ 61478 ],ax
mov [ecx+ 63526 ],ax
sf32_z499:
mov eax,[ebx+ 61480 ]
test ax,0FFFFH
jz sf32_z500
mov [ecx+ 61480 ],ax
mov [ecx+ 63528 ],ax
sf32_z500:
test eax,0FFFF0000H
jz sf32_z501
mov [ecx+ 61482 ],ax
mov [ecx+ 63530 ],ax
sf32_z501:
mov eax,[ebx+ 61484 ]
test ax,0FFFFH
jz sf32_z502
mov [ecx+ 61484 ],ax
mov [ecx+ 63532 ],ax
sf32_z502:
test eax,0FFFF0000H
jz sf32_z503
mov [ecx+ 61486 ],ax
mov [ecx+ 63534 ],ax
sf32_z503:
mov eax,[ebx+ 61488 ]
test ax,0FFFFH
jz sf32_z504
mov [ecx+ 61488 ],ax
mov [ecx+ 63536 ],ax
sf32_z504:
test eax,0FFFF0000H
jz sf32_z505
mov [ecx+ 61490 ],ax
mov [ecx+ 63538 ],ax
sf32_z505:
mov eax,[ebx+ 61492 ]
test ax,0FFFFH
jz sf32_z506
mov [ecx+ 61492 ],ax
mov [ecx+ 63540 ],ax
sf32_z506:
test eax,0FFFF0000H
jz sf32_z507
mov [ecx+ 61494 ],ax
mov [ecx+ 63542 ],ax
sf32_z507:
mov eax,[ebx+ 61496 ]
test ax,0FFFFH
jz sf32_z508
mov [ecx+ 61496 ],ax
mov [ecx+ 63544 ],ax
sf32_z508:
test eax,0FFFF0000H
jz sf32_z509
mov [ecx+ 61498 ],ax
mov [ecx+ 63546 ],ax
sf32_z509:
mov eax,[ebx+ 61500 ]
test ax,0FFFFH
jz sf32_z510
mov [ecx+ 61500 ],ax
mov [ecx+ 63548 ],ax
sf32_z510:
test eax,0FFFF0000H
jz sf32_z511
mov [ecx+ 61502 ],ax
mov [ecx+ 63550 ],ax
sf32_z511:
