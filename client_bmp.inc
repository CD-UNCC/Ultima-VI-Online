ps640400=newsurf(640,400,SURF_SYSMEM16);//intro surfaces
ps320200=newsurf(320,200,SURF_SYSMEM16);
surf_tremor=newsurf(256,256,SURF_SYSMEM16);
surf_tremor2=newsurf(256,256,SURF_SYSMEM16);
surf_tremorcirclemask=newsurf(256,256,SURF_SYSMEM16);
for(y=-127;y<=128;y++){ for(x=-127;x<=128;x++){
z=x*x+y*y; f=sqrt((float)z);
if (f<=127) surf_tremorcirclemask->o2[(y+127)*256+(x+127)]=0; else surf_tremorcirclemask->o2[(y+127)*256+(x+127)]=1;
}}
intro_startup=loadimage(".\\dr\\startup.bmp",SURF_SYSMEM16);
PORTRAIT_UNAVAILABLE=newsurf(1,1,SURF_SYSMEM16); PORTRAIT_UNAVAILABLE->o2[0]=0;

bt32=newsurf(32,32,SURF_SYSMEM16);
bt16=newsurf(16,16,SURF_SYSMEM16);

// r666 loadimage actionbar.bmp
/*
actionbarsurf[0]=loadimage(".\\dr\\actionbar1.bmp",SURF_SYSMEM16);
actionbarsurf[1]=loadimage(".\\dr\\actionbar2.bmp",SURF_SYSMEM16);
actiontalksurf[0]=loadimage(".\\dr\\actiontalk1.bmp",SURF_SYSMEM16);
actiontalksurf[1]=loadimage(".\\dr\\actiontalk2.bmp",SURF_SYSMEM16);

actionbuttonsurf[ACTIONBUTTON_DROP][0]=loadimage(".\\dr\\actiondropsetbutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_DROP][1]=loadimage(".\\dr\\actiondropupbutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_DROP][2]=loadimage(".\\dr\\actiondropdownbutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_DROP][3]=loadimage(".\\dr\\actiondropleftbutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_DROP][4]=loadimage(".\\dr\\actiondroprightbutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_FOOD][1]=loadimage(".\\dr\\actionfoodhungrybutton.bmp",SURF_SYSMEM16);
actionbuttonsurf[ACTIONBUTTON_FOOD][2]=loadimage(".\\dr\\actionfoodfullbutton.bmp",SURF_SYSMEM16);
*/

// r999 new
uipanelimgsurf[UI_IMGI_ACTIONBAR+1] = loadimage(".\\dr\\n1actionbar1.bmp",SURF_SYSMEM16);
uipanelimgsurf[UI_IMGI_ACTIONBAR+2] = loadimage(".\\dr\\n1actionbar2.bmp",SURF_SYSMEM16);
uipanelimgsurf[UI_IMGI_ACTIONBAR+3] = loadimage(".\\dr\\n1optionbar1.bmp",SURF_SYSMEM16);
uipanelimgsurf[UI_IMGI_ACTIONBAR+4] = loadimage(".\\dr\\n1mapbar1.bmp",SURF_SYSMEM16);

uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+1] = loadimage(".\\dr\\n1actiontalk1.bmp",SURF_SYSMEM16);
uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+2] = loadimage(".\\dr\\n1actiontalk2.bmp",SURF_SYSMEM16);
uipanelimgsurf[UI_IMGI_ACTIONTALKBAR+3] = loadimage(".\\dr\\n1actiontalk3.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+1][1] = loadimage(".\\dr\\n1buttondrops1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+1][2] = loadimage(".\\dr\\n1buttondrops2.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+1][3] = loadimage(".\\dr\\n1buttondrops3.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+1][4] = loadimage(".\\dr\\n1buttondrops4.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+1][5] = loadimage(".\\dr\\n1buttondrops5.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+2][1] = loadimage(".\\dr\\n1buttonfoods1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+2][2] = loadimage(".\\dr\\n1buttonfoods2.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+3][1] = loadimage(".\\dr\\n1buttonworldmaps1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+4][1] = loadimage(".\\dr\\n1buttoncombatlogs1.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+5][1] = loadimage(".\\dr\\n1optionenhances1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+5][2] = loadimage(".\\dr\\n1optionenhances2.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+6][1] = loadimage(".\\dr\\n1optiongamesounds1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+6][2] = loadimage(".\\dr\\n1optiongamesounds2.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+7][1] = loadimage(".\\dr\\n1optioncombatsounds1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+7][2] = loadimage(".\\dr\\n1optioncombatsounds2.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+8][1] = loadimage(".\\dr\\n1optionminimaps1.bmp",SURF_SYSMEM16);

// s444
uiwidgetimgsurf[UI_IMGI_WIDGET+9][1] = loadimage(".\\dr\\n1buttonmap1s1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+10][1] = loadimage(".\\dr\\n1buttonmap2s1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+11][1] = loadimage(".\\dr\\n1buttonmap3s1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+12][1] = loadimage(".\\dr\\n1buttonmap4s1.bmp",SURF_SYSMEM16);

uiwidgetimgsurf[UI_IMGI_WIDGET+13][1] = loadimage(".\\dr\\n1buttonhovers1.bmp",SURF_SYSMEM16);
uiwidgetimgsurf[UI_IMGI_WIDGET+13][2] = loadimage(".\\dr\\n1buttonhovers2.bmp",SURF_SYSMEM16);


// s444
worldmapsurfn1[1] = loadimage(".\\dr\\n1u6map.bmp",SURF_SYSMEM16);
worldmapsurfn1[2] = loadimage(".\\dr\\n1u6pworldmap.bmp",SURF_SYSMEM16);
worldmapsurfn1[3] = loadimage(".\\dr\\n1cmap.bmp",SURF_SYSMEM16);
worldmapsurfn1[4] = loadimage(".\\dr\\n1runicsmall.bmp",SURF_SYSMEM16);




minimap_b=loadimage(".\\dr\\minimap.bmp",SURF_SYSMEM16);
treasuremap=loadimage(".\\dr\\cmap.bmp",SURF_SYSMEM16);
tmap_markers=loadimage(".\\dr\\tmapm.bmp",SURF_SYSMEM16);
status8=loadimage(".\\dr\\status8.bmp",SURF_SYSMEM16);
//load basetiles
bt8[0]=loadimage(".\\dr\\bt80.bmp",SURF_SYSMEM16);
bt8[1]=loadimage(".\\dr\\bt81.bmp",SURF_SYSMEM16);
bt8[2]=loadimage(".\\dr\\bt82.bmp",SURF_SYSMEM16);
bt8[3]=loadimage(".\\dr\\bt83.bmp",SURF_SYSMEM16);
bt8[4]=loadimage(".\\dr\\bt84.bmp",SURF_SYSMEM16);
bt8[5]=loadimage(".\\dr\\bt85.bmp",SURF_SYSMEM16);
bt8[6]=loadimage(".\\dr\\bt86.bmp",SURF_SYSMEM16);
bt8[7]=loadimage(".\\dr\\bt87.bmp",SURF_SYSMEM16);
sfx8=loadimage(".\\dr\\sfx88.bmp",SURF_SYSMEM16);
//load spr getspr compatible
//less data loaded, old:12,582,912 new:1,572,864+548,864=2,121,728 (1/6)
spr84[0]=loadimage(".\\dr\\spr8440.bmp",SURF_SYSMEM16);
spr84[1]=loadimage(".\\dr\\spr8441.bmp",SURF_SYSMEM16);
spr84[2]=loadimage(".\\dr\\spr8442.bmp",SURF_SYSMEM16);
spr84[3]=loadimage(".\\dr\\spr8443.bmp",SURF_SYSMEM16);
spr84[4]=loadimage(".\\dr\\spr8444.bmp",SURF_SYSMEM16);
spr84[5]=loadimage(".\\dr\\spr8445.bmp",SURF_SYSMEM16);
spr84[6]=loadimage(".\\dr\\spr8446.bmp",SURF_SYSMEM16);
spr84[7]=loadimage(".\\dr\\spr8447.bmp",SURF_SYSMEM16);
spr84[8]=loadimage(".\\dr\\spr8448.bmp",SURF_SYSMEM16);

spr8[0]=loadimage(".\\dr\\spr87.bmp",SURF_SYSMEM16); //reverse palette order
spr8[1]=loadimage(".\\dr\\spr86.bmp",SURF_SYSMEM16);
spr8[2]=loadimage(".\\dr\\spr85.bmp",SURF_SYSMEM16);
spr8[3]=loadimage(".\\dr\\spr84.bmp",SURF_SYSMEM16);
spr8[4]=loadimage(".\\dr\\spr83.bmp",SURF_SYSMEM16);
spr8[5]=loadimage(".\\dr\\spr82.bmp",SURF_SYSMEM16);
spr8[6]=loadimage(".\\dr\\spr81.bmp",SURF_SYSMEM16);
spr8[7]=loadimage(".\\dr\\spr80.bmp",SURF_SYSMEM16);
darrow=loadimage(".\\dr\\darrow.bmp",SURF_SYSMEM16);
uarrow=loadimage(".\\dr\\uarrow.bmp",SURF_SYSMEM16);
horizon=loadimage(".\\dr\\horizon.bmp",SURF_SYSMEM16);
horizon2=loadimage(".\\dr\\horizon2.bmp",SURF_SYSMEM16);




cave=loadimage(".\\dr\\cave.bmp",SURF_SYSMEM16);

sun=loadimage(".\\dr\\sunb.bmp",SURF_SYSMEM16);
sun2=loadimage(".\\dr\\sun2b.bmp",SURF_SYSMEM16);
mini_1=loadimage(".\\dr\\mini_1b.bmp",SURF_SYSMEM16);
tmini_1=loadimage(".\\dr\\mini_1b.bmp",SURF_SYSMEM16);
mini_2=loadimage(".\\dr\\mini_2b.bmp",SURF_SYSMEM16);
mini_3=loadimage(".\\dr\\mini_3b.bmp",SURF_SYSMEM16);
u6ob; u6ob=loadimage(".\\dr\\u6ob.bmp",SURF_SYSMEM16);
dhno=loadimage(".\\dr\\no.bmp",SURF_SYSMEM16);
not4sale=loadimage(".\\dr\\not4sale.bmp",SURF_SYSMEM16);
not4salemask=loadimage(".\\dr\\n4smask.bmp",SURF_SYSMEM16);


//surf* null_tr=loadimage(".\\dr\\null_tr.bmp",SURF_SYSMEM16);
//surf* null_bl=loadimage(".\\dr\\null_bl.bmp",SURF_SYSMEM16);
converse_arrows=loadimage(".\\dr\\arrows3.bmp",SURF_SYSMEM16);
spellbook=loadimage(".\\dr\\spelbook.bmp",SURF_SYSMEM16);
spellbookmini=loadimage(".\\dr\\sbmini.bmp",SURF_SYSMEM16);

statusmessage_arrowup=loadimage(".\\dr\\arrowup.bmp",SURF_SYSMEM16);



//ddcc.dwColorSpaceHighValue=31*32*64+61*32+24;
//ddcc.dwColorSpaceLowValue=31*32*64+61*32+24;

spellcircle[0]=loadimage(".\\dr\\circle1.bmp",SURF_SYSMEM16);
spellcircle[1]=loadimage(".\\dr\\circle2.bmp",SURF_SYSMEM16);
spellcircle[2]=loadimage(".\\dr\\circle3.bmp",SURF_SYSMEM16);
spellcircle[3]=loadimage(".\\dr\\circle4.bmp",SURF_SYSMEM16);
spellcircle[4]=loadimage(".\\dr\\circle5.bmp",SURF_SYSMEM16);
spellcircle[5]=loadimage(".\\dr\\circle6.bmp",SURF_SYSMEM16);
spellcircle[6]=loadimage(".\\dr\\circle7.bmp",SURF_SYSMEM16);
spellcircle[7]=loadimage(".\\dr\\circle8.bmp",SURF_SYSMEM16);
img0_0key(spellcircle[0],31*32*64+61*32+24);
img0_0key(spellcircle[1],31*32*64+61*32+24);
img0_0key(spellcircle[2],31*32*64+61*32+24);
img0_0key(spellcircle[3],31*32*64+61*32+24);
img0_0key(spellcircle[4],31*32*64+61*32+24);
img0_0key(spellcircle[5],31*32*64+61*32+24);
img0_0key(spellcircle[6],31*32*64+61*32+24);
img0_0key(spellcircle[7],31*32*64+61*32+24);

/*
spellcircle[0]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[1]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[2]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[3]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[4]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[5]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[6]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
spellcircle[7]->s->SetColorKey(DDCKEY_SRCBLT,&ddcc);
*/

statusbar_r255=loadimage(".\\dr\\r255.bmp",SURF_SYSMEM16);
statusbar_r128=loadimage(".\\dr\\r128.bmp",SURF_SYSMEM16);
statusbar_b255=loadimage(".\\dr\\b255.bmp",SURF_SYSMEM16);
statusbar_b128=loadimage(".\\dr\\b128.bmp",SURF_SYSMEM16);
statusbar_g255=loadimage(".\\dr\\g255.bmp",SURF_SYSMEM16);
statusbar_g128=loadimage(".\\dr\\g128.bmp",SURF_SYSMEM16);

statusbar=newsurf(32,10,SURF_SYSMEM16);

dogearr=loadimage(".\\dr\\dogear.bmp",SURF_SYSMEM16);
dogearl=loadimage(".\\dr\\dogear2.bmp",SURF_SYSMEM16);
spellbookline=loadimage(".\\dr\\line.bmp",SURF_SYSMEM16);

/*
surf *intro10=loadimage(".\\dr\\intro10.bmp",SURF_SYSMEM16);
surf *intro11=loadimage(".\\dr\\intro11.bmp",SURF_SYSMEM16);
surf *intro12=loadimage(".\\dr\\intro12.bmp",SURF_SYSMEM16);
surf *intro13=loadimage(".\\dr\\intro13.bmp",SURF_SYSMEM16);
surf *intro14=loadimage(".\\dr\\intro14.bmp",SURF_SYSMEM16);
surf *intro15=loadimage(".\\dr\\intro15.bmp",SURF_SYSMEM16);
surf *intro16=loadimage(".\\dr\\intro16.bmp",SURF_SYSMEM16);
surf *intro17=loadimage(".\\dr\\intro17.bmp",SURF_SYSMEM16);
surf *intro18=loadimage(".\\dr\\intro18.bmp",SURF_SYSMEM16);
surf *intro19=loadimage(".\\dr\\intro19.bmp",SURF_SYSMEM16);
surf *intro110=loadimage(".\\dr\\intro110.bmp",SURF_SYSMEM16);
surf *intro111=loadimage(".\\dr\\intro111.bmp",SURF_SYSMEM16);
surf *intro112=loadimage(".\\dr\\intro112.bmp",SURF_SYSMEM16);
surf *intro113=loadimage(".\\dr\\intro113.bmp",SURF_SYSMEM16);
surf *intro114=loadimage(".\\dr\\intro114.bmp",SURF_SYSMEM16);
surf *intro115=newsurf(192,400,SURF_SYSMEM16); cls(intro115,0);
surf *intro116=loadimage(".\\dr\\intro115.bmp",SURF_SYSMEM16);

surf *intro20=loadimage(".\\dr\\intro20.bmp",SURF_SYSMEM16);
surf *intro21=loadimage(".\\dr\\intro21.bmp",SURF_SYSMEM16);
surf *intro22=loadimage(".\\dr\\intro22.bmp",SURF_SYSMEM16);
surf *intro23=loadimage(".\\dr\\intro23.bmp",SURF_SYSMEM16);
surf *intro24=loadimage(".\\dr\\intro24.bmp",SURF_SYSMEM16);
surf *intro25=loadimage(".\\dr\\intro25.bmp",SURF_SYSMEM16);
surf *intro26=loadimage(".\\dr\\intro26.bmp",SURF_SYSMEM16);
surf *intro27=loadimage(".\\dr\\intro27.bmp",SURF_SYSMEM16);
*/

intro_gypsy=loadimage(".\\dr\\gypsy.bmp",SURF_SYSMEM16);
intro_gypsy2=loadimage(".\\dr\\gypsy2.bmp",SURF_SYSMEM16);

intro_vial=loadimage(".\\dr\\vial.bmp",SURF_SYSMEM16);
intro_svial=newsurf(intro_vial->d.dwWidth,intro_vial->d.dwHeight,SURF_SYSMEM16);

intro_bigvial=loadimage(".\\dr\\bigvial.bmp",SURF_SYSMEM16);

intro_hpl0=loadimage(".\\dr\\hpl0.bmp",SURF_SYSMEM16);
intro_hpl2=loadimage(".\\dr\\hpl2.bmp",SURF_SYSMEM16);
intro_hpl3=loadimage(".\\dr\\hpl3.bmp",SURF_SYSMEM16);

intro_hps0=newsurf(intro_hpl0->d.dwWidth,intro_hpl0->d.dwHeight,SURF_SYSMEM16);
intro_hps2=newsurf(intro_hpl2->d.dwWidth,intro_hpl2->d.dwHeight,SURF_SYSMEM16);
intro_hps3=newsurf(intro_hpl3->d.dwWidth,intro_hpl3->d.dwHeight,SURF_SYSMEM16);

intro_hpr0=loadimage(".\\dr\\hpr0.bmp",SURF_SYSMEM16);
intro_hpr2=loadimage(".\\dr\\hpr2.bmp",SURF_SYSMEM16);
intro_hpr3=loadimage(".\\dr\\hpr3.bmp",SURF_SYSMEM16);

intro_arml=loadimage(".\\dr\\arml.bmp",SURF_SYSMEM16);
intro_armr=loadimage(".\\dr\\armr.bmp",SURF_SYSMEM16);

intro_s64=newsurf(64,64,SURF_SYSMEM16);
intro_s64b=newsurf(64,64,SURF_SYSMEM16);
intro_s128=newsurf(64,128,SURF_SYSMEM16);

intro_ab=loadimage(".\\dr\\ab.bmp",SURF_SYSMEM16);
intro_aba=loadimage(".\\dr\\aba.bmp",SURF_SYSMEM16);
intro_abb=loadimage(".\\dr\\abb.bmp",SURF_SYSMEM16);


intro_caravan=loadimage(".\\dr\\caravan.bmp",SURF_SYSMEM16);


intro_flask=loadimage(".\\dr\\flask.bmp",SURF_SYSMEM16);




intro_ccsave1=loadimage(".\\dr\\ccsave1.bmp",SURF_SYSMEM16);
intro_ccsave2=loadimage(".\\dr\\ccsave2.bmp",SURF_SYSMEM16);
intro_ccsave3=loadimage(".\\dr\\ccsave3.bmp",SURF_SYSMEM16);

//surf *intro_startup=loadimage(".\\dr\\startup.bmp",SURF_SYSMEM16);


intro_tacinfo=loadimage(".\\dr\\tacinfo.bmp",SURF_SYSMEM16);

intro_newchar=loadimage(".\\dr\\newchar.bmp",SURF_SYSMEM16);
intro_newchar2=loadimage(".\\dr\\newchar2.bmp",SURF_SYSMEM16);

intro_x=loadimage(".\\dr\\x.bmp",SURF_SYSMEM16);

intro_back=loadimage(".\\dr\\back.bmp",SURF_SYSMEM16);
intro_next=loadimage(".\\dr\\next.bmp",SURF_SYSMEM16);

intro_ifield=loadimage(".\\dr\\ifield.bmp",SURF_SYSMEM16);


blr[0]=loadimage(".\\dr\\blr0.bmp",SURF_SYSMEM16);
blr[1]=loadimage(".\\dr\\blr1.bmp",SURF_SYSMEM16);
blr[2]=loadimage(".\\dr\\blr2.bmp",SURF_SYSMEM16);
blr[3]=loadimage(".\\dr\\blr3.bmp",SURF_SYSMEM16);

glr[0]=loadimage(".\\dr\\glr0.bmp",SURF_SYSMEM16);
glr[1]=loadimage(".\\dr\\glr1.bmp",SURF_SYSMEM16);
glr[2]=loadimage(".\\dr\\glr2.bmp",SURF_SYSMEM16);
glr[3]=loadimage(".\\dr\\glr3.bmp",SURF_SYSMEM16);

instantclickx=loadimage(".\\dr\\icx.bmp",SURF_SYSMEM16);
instantclickok=loadimage(".\\dr\\icok.bmp",SURF_SYSMEM16);


endgame_image[0]=loadimage(".\\dr\\endgame0.bmp",SURF_SYSMEM16);
endgame_image[1]=loadimage(".\\dr\\endgame1.bmp",SURF_SYSMEM16);
endgame_image[2]=loadimage(".\\dr\\endgame2.bmp",SURF_SYSMEM16);
endgame_image[3]=loadimage(".\\dr\\endgame3.bmp",SURF_SYSMEM16);
endgame_image[4]=loadimage(".\\dr\\endgame4.bmp",SURF_SYSMEM16);
endgame_image[5]=loadimage(".\\dr\\endgame5.bmp",SURF_SYSMEM16);
endgame_image[6]=loadimage(".\\dr\\endgame6.bmp",SURF_SYSMEM16);
//endgame_image[7]=loadimage(".\\dr\\endgame7.bmp",SURF_SYSMEM16);
endgame_image[8]=loadimage(".\\dr\\endgame8.bmp",SURF_SYSMEM16);
endgame_image[9]=loadimage(".\\dr\\endgame9.bmp",SURF_SYSMEM16);



spellbookmini2=loadimage(".\\dr\\sbmini2.bmp",SURF_SYSMEM16);


voicechat_voicebar=loadimage(".\\dr\\voicebar.bmp",SURF_SYSMEM16);
voicechat_voice1=loadimage(".\\dr\\voice1.bmp",SURF_SYSMEM16);
voicechat_voiceof=loadimage(".\\dr\\voiceof.bmp",SURF_SYSMEM16);

inventoryadd_icon=loadimage(".\\dr\\invadd.bmp",SURF_SYSMEM16);


horsemask=loadimage(".\\dr\\horsemu.bmp",SURF_SYSMEM16);
horsemask2=loadimage(".\\dr\\horseml.bmp",SURF_SYSMEM16);
horsemaskdress=loadimage(".\\dr\\horsemu2.bmp",SURF_SYSMEM16);
horsemask2dress=loadimage(".\\dr\\horseml2.bmp",SURF_SYSMEM16);
horsemaskdressb=loadimage(".\\dr\\horsebu2.bmp",SURF_SYSMEM16);
horsemask2dressb=loadimage(".\\dr\\horsebl2.bmp",SURF_SYSMEM16);

tfh=open(".\\dr\\sc480a2.raw"); get(tfh,&stormcloak[0][0],lof(tfh)); close(tfh);
tfh=open(".\\dr\\sc480b2.raw"); get(tfh,&stormcloak[7][0],lof(tfh)); close(tfh);
for (i=1;i<=6;i++){
for (i2=0;i2<230400;i2++){
stormcloak[i][i2]=((unsigned long)stormcloak[0][i2]*(7-i)+(unsigned long)stormcloak[7][i2]*i)/7;
}
}
for (i=0;i<=7;i++){
for (y2=0;y2<479;y2++){
for (x2=0;x2<479;x2++){
x=stormcloak[i][x2+y2*480];
x3=x2-240;
y3=y2-240;
z=sqrt((float)(x3*x3+y3*y3))/3;
x/=(24+z);
stormcloak[i][x2+y2*480]=x;
}}
}


//portraits 2.0 load file portraits
txtset(t2,".\\dr\\u6port\\");
txtset(t3,".bmp");
for(i=0;i<=207;i++){
txtnumint(t,i);
txtset(t4,t2);
txtadd(t4,t);
txtadd(t4,t3);
port_temp=loadimage(t4,SURF_SYSMEM16);
img0_0key(port_temp,31*32*64+61*32+24);
loadportrait(i,port_temp);
}

vm_volumem=loadimage(".\\dr\\volumem.bmp",SURF_SYSMEM16);
vm_volmmute=loadimage(".\\dr\\volmmute.bmp",SURF_SYSMEM16);
vm_voltab2m=loadimage(".\\dr\\voltab2m.bmp",SURF_SYSMEM16);

volcontrol_background=loadimage(".\\dr\\volume.bmp",SURF_SYSMEM16);
volcontrol_surf=loadimage(".\\dr\\volume.bmp",SURF_SYSMEM16);
volcontrol_tab1=loadimage(".\\dr\\voltab1.bmp",SURF_SYSMEM16);
volcontrol_tab2=loadimage(".\\dr\\voltab2.bmp",SURF_SYSMEM16);
volcontrol_tab3=loadimage(".\\dr\\voltab3.bmp",SURF_SYSMEM16);

viewnpc=loadimage(".\\dr\\viewnpc.bmp",SURF_SYSMEM16);
viewnpc2=loadimage(".\\dr\\viewnpc2.bmp",SURF_SYSMEM16);
viewnpc_temp=newsurf(viewnpc->d.dwWidth,viewnpc->d.dwHeight,SURF_SYSMEM16);
viewnpc2_temp=newsurf(viewnpc2->d.dwWidth,viewnpc2->d.dwHeight,SURF_SYSMEM16);
