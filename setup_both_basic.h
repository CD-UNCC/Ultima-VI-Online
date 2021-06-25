Randomc(timeGetTime());

ZeroMemory(&spellreagent,sizeof(spellreagent));
ZeroMemory(&spelltarget,256);//0=unknown,1=crt
ZeroMemory(&spellname,sizeof(spellname)); for (i=0;i<=255;i++) spellname[i]=txtnew();

txtset(spellname[(0<<4)+0],"CreateFood"); spellreagent[(0<<4)+0]=GARL|GINS|MAND; //level 1
txtset(spellname[(0<<4)+1],"DetectMagic"); spellreagent[(0<<4)+1]=NIGH|SULF;
txtset(spellname[(0<<4)+2],"DetectTrap"); spellreagent[(0<<4)+2]=NIGH|SULF;
txtset(spellname[(0<<4)+3],"Cure"); spellreagent[(0<<4)+3]=GARL|GINS;
txtset(spellname[(0<<4)+4],"Douse"); spellreagent[(0<<4)+4]=GARL|BLAC;
txtset(spellname[(0<<4)+5],"Harm"); spellreagent[(0<<4)+5]=NIGH|SPID;
spelltarget[(0<<4)+5]=1;
txtset(spellname[(0<<4)+6],"Heal"); spellreagent[(0<<4)+6]=GINS|SPID;
txtset(spellname[(0<<4)+7],"Awaken"); spellreagent[(0<<4)+7]=GINS|GARL;
txtset(spellname[(0<<4)+8],"Ignite"); spellreagent[(0<<4)+8]=SULF|BLAC;
txtset(spellname[(0<<4)+9],"Light"); spellreagent[(0<<4)+9]=SULF;

txtset(spellname[(1<<4)+0],"Infravision"); spellreagent[(1<<4)+0]=NIGH|SULF; //level 2
txtset(spellname[(1<<4)+1],"MagicArrow"); spellreagent[(1<<4)+1]=SULF|BLAC;
spelltarget[(1<<4)+1]=1;
txtset(spellname[(1<<4)+2],"Poison"); spellreagent[(1<<4)+2]=NIGH|BLOO|BLAC;
spelltarget[(1<<4)+2]=1;
txtset(spellname[(1<<4)+3],"Reappear"); spellreagent[(1<<4)+3]=SPID|BLOO|BLAC;
txtset(spellname[(1<<4)+4],"Sleep"); spellreagent[(1<<4)+4]=NIGH|SPID|BLAC;
spelltarget[(1<<4)+4]=1;
txtset(spellname[(1<<4)+5],"Telekinesis"); spellreagent[(1<<4)+5]=BLOO|MAND|BLAC;
txtset(spellname[(1<<4)+6],"Trap"); spellreagent[(1<<4)+6]=SPID|NIGH;
txtset(spellname[(1<<4)+7],"UnlockMagic"); spellreagent[(1<<4)+7]=SULF|BLOO;
txtset(spellname[(1<<4)+8],"Untrap"); spellreagent[(1<<4)+8]=SULF|BLOO;
txtset(spellname[(1<<4)+9],"Vanish"); spellreagent[(1<<4)+9]=GARL|BLOO|BLAC;

txtset(spellname[(2<<4)+0],"Curse"); spellreagent[(2<<4)+0]=SULF|NIGH|GARL; //level 3
spelltarget[(2<<4)+0]=1;
txtset(spellname[(2<<4)+1],"DispelField"); spellreagent[(2<<4)+1]=BLAC|SULF;
txtset(spellname[(2<<4)+2],"Fireball"); spellreagent[(2<<4)+2]=SULF|BLAC;
spelltarget[(2<<4)+2]=1;
txtset(spellname[(2<<4)+3],"GreatLight"); spellreagent[(2<<4)+3]=SULF|MAND;
txtset(spellname[(2<<4)+4],"MagicLock"); spellreagent[(2<<4)+4]=SULF|BLOO|GARL;
txtset(spellname[(2<<4)+5],"MassAwaken"); spellreagent[(2<<4)+5]=GINS|GARL;
txtset(spellname[(2<<4)+6],"MassSleep"); spellreagent[(2<<4)+6]=GINS|NIGH|SPID;
spelltarget[(2<<4)+6]=1;
txtset(spellname[(2<<4)+7],"Peer"); spellreagent[(2<<4)+7]=NIGH|MAND;
txtset(spellname[(2<<4)+8],"Protection"); spellreagent[(2<<4)+8]=SULF|GINS|GARL;
txtset(spellname[(2<<4)+9],"RepelUndead"); spellreagent[(2<<4)+9]=GARL|SULF;
spelltarget[(2<<4)+9]=1;

txtset(spellname[(3<<4)+0],"Animate"); spellreagent[(3<<4)+0]=SULF|MAND|BLOO; //level 4
txtset(spellname[(3<<4)+1],"Conjure"); spellreagent[(3<<4)+1]=SPID|MAND;
txtset(spellname[(3<<4)+2],"Disable"); spellreagent[(3<<4)+2]=NIGH|SPID|MAND;
spelltarget[(3<<4)+2]=1;
txtset(spellname[(3<<4)+3],"FireField"); spellreagent[(3<<4)+3]=BLAC|SULF|SPID;
txtset(spellname[(3<<4)+4],"GreatHeal"); spellreagent[(3<<4)+4]=GINS|SPID|MAND;
txtset(spellname[(3<<4)+5],"Locate"); spellreagent[(3<<4)+5]=NIGH;
txtset(spellname[(3<<4)+6],"MassCure"); spellreagent[(3<<4)+6]=GARL|GINS;
txtset(spellname[(3<<4)+7],"PoisonField"); spellreagent[(3<<4)+7]=NIGH|SPID|BLAC;
txtset(spellname[(3<<4)+8],"SleepField"); spellreagent[(3<<4)+8]=GINS|SPID|BLAC;
txtset(spellname[(3<<4)+9],"WindChange"); spellreagent[(3<<4)+9]=SULF|BLOO;

txtset(spellname[(4<<4)+0],"EnergyField"); spellreagent[(4<<4)+0]=MAND|SPID|BLAC; //level 5
txtset(spellname[(4<<4)+1],"Explosion"); spellreagent[(4<<4)+1]=MAND|SULF|BLAC|BLOO;
spelltarget[(4<<4)+1]=1;
txtset(spellname[(4<<4)+2],"InsectSwarm"); spellreagent[(4<<4)+2]=BLOO|SPID|SULF;
txtset(spellname[(4<<4)+3],"Invisibility"); spellreagent[(4<<4)+3]=NIGH|BLOO;
txtset(spellname[(4<<4)+4],"Lightning"); spellreagent[(4<<4)+4]=BLAC|MAND|SULF;
spelltarget[(4<<4)+4]=1;
txtset(spellname[(4<<4)+5],"Paralyze"); spellreagent[(4<<4)+5]=SPID|SULF|NIGH|BLAC;
spelltarget[(4<<4)+5]=1;
txtset(spellname[(4<<4)+6],"PickPocket"); spellreagent[(4<<4)+6]=BLOO|SPID|NIGH;
spelltarget[(4<<4)+6]=1;
txtset(spellname[(4<<4)+7],"Reveal"); spellreagent[(4<<4)+7]=SPID|NIGH|MAND;
txtset(spellname[(4<<4)+8],"Seance"); spellreagent[(4<<4)+8]=BLOO|MAND|NIGH|SPID|SULF;
txtset(spellname[(4<<4)+9],"X-ray"); spellreagent[(4<<4)+9]=MAND|SULF;

txtset(spellname[(5<<4)+0],"Charm"); spellreagent[(5<<4)+0]=BLAC|NIGH|SPID; //level 6
spelltarget[(5<<4)+0]=1;
txtset(spellname[(5<<4)+1],"Clone"); spellreagent[(5<<4)+1]=SULF|SPID|BLOO|GINS|NIGH|MAND;
txtset(spellname[(5<<4)+2],"Confuse"); spellreagent[(5<<4)+2]=MAND|NIGH;
spelltarget[(5<<4)+2]=1;
txtset(spellname[(5<<4)+3],"FlameWind"); spellreagent[(5<<4)+3]=SULF|BLOO|MAND;
spelltarget[(5<<4)+3]=1;
txtset(spellname[(5<<4)+4],"HailStorm"); spellreagent[(5<<4)+4]=BLOO|BLAC|MAND;
//NEWCODE
spelltarget[(5<<4)+4]=1;
//NEWCODEEND
txtset(spellname[(5<<4)+5],"MassProtect"); spellreagent[(5<<4)+5]=SULF|GINS|GARL|MAND;
txtset(spellname[(5<<4)+6],"NegateMagic"); spellreagent[(5<<4)+6]=GARL|MAND|SULF;
txtset(spellname[(5<<4)+7],"PoisonWind"); spellreagent[(5<<4)+7]=NIGH|SULF|BLOO;
txtset(spellname[(5<<4)+8],"Replicate"); spellreagent[(5<<4)+8]=SULF|SPID|BLOO|GINS|NIGH;
txtset(spellname[(5<<4)+9],"Web"); spellreagent[(5<<4)+9]=SPID;
spelltarget[(5<<4)+9]=1;

txtset(spellname[(6<<4)+0],"ChainBolt"); spellreagent[(6<<4)+0]=BLAC|MAND|SULF|BLOO; //level 7
spelltarget[(6<<4)+0]=1;
txtset(spellname[(6<<4)+1],"Enchant"); spellreagent[(6<<4)+1]=SPID|MAND|SULF;
txtset(spellname[(6<<4)+2],"EnergyWind"); spellreagent[(6<<4)+2]=MAND|NIGH|SULF|BLOO;
spelltarget[(6<<4)+2]=1;
txtset(spellname[(6<<4)+3],"Fear"); spellreagent[(6<<4)+3]=NIGH|MAND|GARL;
txtset(spellname[(6<<4)+4],"GateTravel"); spellreagent[(6<<4)+4]=SULF|BLAC|MAND;
txtset(spellname[(6<<4)+5],"Kill"); spellreagent[(6<<4)+5]=BLAC|NIGH|SULF;
spelltarget[(6<<4)+5]=1;
txtset(spellname[(6<<4)+6],"MassCurse"); spellreagent[(6<<4)+6]=SULF|NIGH|GARL|MAND;
spelltarget[(6<<4)+6]=1;
txtset(spellname[(6<<4)+7],"MassInvisibility"); spellreagent[(6<<4)+7]=MAND|NIGH|BLOO|BLAC;
txtset(spellname[(6<<4)+8],"WingStrike"); spellreagent[(6<<4)+8]=BLOO|SPID|MAND|SULF;
txtset(spellname[(6<<4)+9],"WizardEye"); spellreagent[(6<<4)+9]=BLOO|NIGH|MAND|SULF|BLAC|SPID;

txtset(spellname[(7<<4)+0],"DeathWind"); spellreagent[(7<<4)+0]=MAND|NIGH|SULF|BLOO; //level 8
spelltarget[(7<<4)+0]=1;
txtset(spellname[(7<<4)+1],"Eclipse"); spellreagent[(7<<4)+1]=MAND|SULF|NIGH|GARL|BLOO;
txtset(spellname[(7<<4)+2],"MassCharm"); spellreagent[(7<<4)+2]=BLAC|NIGH|SPID|MAND;
spelltarget[(7<<4)+2]=1;
txtset(spellname[(7<<4)+3],"MassKill"); spellreagent[(7<<4)+3]=BLAC|NIGH|MAND|SULF;
spelltarget[(7<<4)+3]=1;
txtset(spellname[(7<<4)+4],"Resurrect"); spellreagent[(7<<4)+4]=GARL|GINS|SPID|SULF|BLOO|MAND;
txtset(spellname[(7<<4)+5],"Slime"); spellreagent[(7<<4)+5]=BLOO|NIGH|MAND;
spelltarget[(7<<4)+5]=1;
txtset(spellname[(7<<4)+6],"Summon"); spellreagent[(7<<4)+6]=MAND|GARL|BLOO;
txtset(spellname[(7<<4)+7],"TimeStop"); spellreagent[(7<<4)+7]=MAND|GARL|BLOO;
txtset(spellname[(7<<4)+8],"Tremor"); spellreagent[(7<<4)+8]=BLOO|SULF|MAND;
txtset(spellname[(7<<4)+9],"Armageddon"); spellreagent[(7<<4)+9]=255; //all!

//4 0xD6C7B8A9 (signature)
//2 255 (checksum)
//2 1 (length)
//1 254 (incorrect version)
incorrectversionmessage[0]=0xA9;
incorrectversionmessage[1]=0xB8;
incorrectversionmessage[2]=0xC7;
incorrectversionmessage[3]=0xD6;
incorrectversionmessage[4]=255;
incorrectversionmessage[5]=0;
incorrectversionmessage[6]=1;
incorrectversionmessage[7]=0;
incorrectversionmessage[8]=254;

ZeroMemory(&mover_offsety,sizeof(mover_offsety));
ZeroMemory(&mover_offsetx,sizeof(mover_offsetx));
for (y=0;y<=6;y++){ for (x=0;x<=6;x++){
mover_offseti[x][y]=255;//offset not available
}}
i=0;
for (y=1;y<=5;y++){ for (x=1;x<=5;x++){
mover_offseti[x][y]=i; mover_offsetx[i]=x-3; mover_offsety[i]=y-3;
i++;
}}
//NOTE: mover_offseti[3][3] is INVALID for referencing but not set to 255 for comparison reasons
i=12; mover_offseti[0][0]=i; mover_offsetx[i]=-3; mover_offsety[i]=-3;
i=25; mover_offseti[3][0]=i; mover_offsetx[i]=0; mover_offsety[i]=-3; i++;
mover_offseti[6][0]=i; mover_offsetx[i]=3; mover_offsety[i]=-3; i++;
mover_offseti[6][3]=i; mover_offsetx[i]=3; mover_offsety[i]=0; i++;
mover_offseti[6][6]=i; mover_offsetx[i]=3; mover_offsety[i]=3; i++;
mover_offseti[3][6]=i; mover_offsetx[i]=0; mover_offsety[i]=3; i++;
mover_offseti[0][6]=i; mover_offsetx[i]=-3; mover_offsety[i]=3; i++;
mover_offseti[0][3]=i; mover_offsetx[i]=-3; mover_offsety[i]=0;

x=0; x2=1;
for (i=0;i<=32;i++){//i=number of bits to keep (0=clear all)
BITSleftmask[i]=x;
x+=x2; x2<<=1;
}

ZeroMemory(&objfloatflags,sizeof(objfloatflags)); tfh=open(".\\dr\\objfloat.flg"); get(tfh,&objfloatflags,lof(tfh)); close(tfh);

ZeroMemory(&sprlnk,sizeof(sprlnk)); tfh=open(".\\dr\\u6spr.lnk"); get(tfh,&sprlnk,lof(tfh)); close(tfh);

//load class info
ZeroMemory(&tclass_object,65536); ZeroMemory(&tclass_mover,65536); ZeroMemory(&tclass_fixed,65536); ZeroMemory(&tclass_build,65536);
tfh=open(".\\dr\\objclass.txt");
i=1;
loadclassinfoloop:
if (sprlnk[i+1]==0) goto loadclassinfodone;
for (i2=0;i2<(sprlnk[i+1]-sprlnk[i]);i2++){
txtfilein(t,tfh);
txtset(t2,"o"); if (txtsame(t,t2)) tclass_object[i+i2*1024]=1;
txtset(t2,"m"); if (txtsame(t,t2)) tclass_mover[i+i2*1024]=1;
txtset(t2,"md"); if (txtsame(t,t2)) tclass_mover[i+i2*1024]=1+2;
txtset(t2,"f"); if (txtsame(t,t2)) tclass_fixed[i+i2*1024]=1;
txtset(t2,"bs"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=1;
txtset(t2,"bh"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=2;
txtset(t2,"bv"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=4;
txtset(t2,"b"); if (txtsame(t,t2)) tclass_build[i+i2*1024]=8;
}//i2
i=i+1; goto loadclassinfoloop;
loadclassinfodone:
tclass_object[339+10*1024]=1; //sherry fix
close(tfh);
i=292;
for (i2=0;i2<=3;i2++){
tclass_fixed[i+i2*1024]=1;
}
i=291;
for (i2=0;i2<=3;i2++){
tclass_fixed[i+i2*1024]=1;
}

ZeroMemory(&obji,sizeof(obji)); tfh=open(".\\dr\\objinfo.u6o"); get(tfh,&obji,lof(tfh)); close(tfh);