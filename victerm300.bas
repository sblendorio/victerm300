;term300-2.prg ==1001==
   10 fori=828to844:reada:pokei,a:next
   50 close 5,2,3:print chr$(14);
  100 open 5,2,3,chr$(6):q$=chr$(34)
  110 dim f%(255),t%(255)
  200 for j=32to64:t%(j)=j:next
  210 t%(13)=13:t%(20)=8
  220 for j=65to90:k=j+32:t%(j)=k:next
  230 for j=91to95:t%(j)=j:next
  240 for j=193to218:k=j-128:t%(j)=k:next
  250 t%(146)=16:t%(133)=16
  255 t%(137)=03:t%(134)=17:t%(138)=19
  260 for j=0 to 255
  270 k=t%(j)
  280 if k<>0thenf%(k)=j:f%(k+128)=j
  290 next:f%(12)=147:poke204,0
  300 print " "chr$(147)
  305 print "vic terminal 300 baud"
  310 get#5,a$
  320 if a$=""or st<>0 then 360
  330 ifpeek(204)=0thensys828
  335 print chr$(f%(asc(a$)));
  340 if a$=q$ then poke 212,0
  350 goto 310
  360 get a$
  370 if a$<>""then print#5,chr$(t%(asc(a$)));
  375 ifpeek(204)<>0thenpoke204,0
  400 if (peek(37151)and 64)=1 then 400
  410 goto 310
  500 data164,204,208,8,160,1,132,205,164,207,208,252,160,255,132,204,96
  