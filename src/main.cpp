//U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12, /* reset=*/ 8);
#include <main.h>
static CAN_message_t msg;
boolean coueleurChoie=false,premieerEcrant0=true,premieerEcrant1=true,premieerEcrant2=true,premieerEcrant3=true,premieerEcrant4=true,premieerEcrant5=true,premieerEcrant6=true,premieerEcrant7=true;
byte transition,encodeurPos=0,encodeurPosP=1,strategieBicolor,sensRotationEncodeur,couleurStrategie=0;
byte ecrant=0,nb=0;
int timerRotationEncodeur=0,timerBpEncodeur,bpEncodeur=0;
char couleurCanRecu='n';
String st="ez",Strategie;
boolean receptionStrategic=false,strategieRecu=false,pasEnComme=false,modeMatche,irette,tiretteP;
byte nombreDePoint=0;
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 12);
void setup() {
  Can0.begin(1000000);
  u8g2.begin();
  Serial.begin(9600);
  MsTimer2::set(1,IntrerrupTimer);
  MsTimer2::start();
  pinMode(TIRETTE,INPUT_PULLUP);
  pinMode(ENCODEUR_BP,INPUT_PULLUP);
  pinMode(ENCODEUR_A,INPUT_PULLUP);
  pinMode(ENCODEUR_B,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_BP),bpInterupEncodeur,RISING);
  attachInterrupt(digitalPinToInterrupt(ENCODEUR_A),tournInterupEncodeurRisingA,RISING);
  delay(100);
  msg.id=0x006;//message d'initialization
  msg.len=3;
  msg.buf[0]='d';
  msg.buf[1]='s';
  msg.buf[2]=0;
  Can0.write(msg);
}
void loop() {
  while(Can0.read(msg)){
    interpretationCan();
  }
  switch (ecrant) {
    case 0:
      ecrant=ecrant_0();
    break;
    case 1:
      ecrant=ecrant_1();
    break;
    case 2:
      ecrant=ecrant_2();
    break;
    case 3:
      ecrant=ecrant_3();
    break;
    case 4:
      ecrant=ecrant_4();
    break;
    case 5:
      ecrant=ecrant_5();
    break;
    case 6:
      ecrant=ecrant_6();
    break;
    case 7:
      ecrant=ecrant_7();
    break;
  }
}

byte ecrant_0(){
  // partie encodeur
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant0){
    if (encodeurPos>2) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=2;
    encodeurPosP=encodeurPos;
    //partie afficheur
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0, encodeurPos*10, 128, 11);
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9,"Strategiete");
    u8g2.drawStr(0,19,"Test");
    u8g2.drawStr(0,29,"Trasfer strategiete");
    u8g2.sendBuffer();					// transfer internal memory to the display
    if(encodeurPosP==0 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=3;
      msg.buf[0]='G';//pour le matche
      msg.buf[1]='e';
      msg.buf[2]='m';
      Can0.write(msg);
      modeMatche=true;
      transition=1;//Voire si il faut metre des retoure
    }else if(encodeurPosP==1 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=3;
      msg.buf[0]='G';//pour les test
      msg.buf[1]='e';
      msg.buf[2]='t';
      Can0.write(msg);
      modeMatche=false;
      transition=2;//Voire si il faut metre des retoure
    }else if(encodeurPosP==2 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=3;
      msg.buf[0]='G';//pour la communicitation des transfer des fichierù
      msg.buf[1]='e';
      msg.buf[2]='c';
      Can0.write(msg);
      pasEnComme=false;
      transition=3;//Voire si il faut metre des retoure
    }else transition=0;
    bpEncodeur=0;
    premieerEcrant0=false;
  }
  if(transition!=0){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
    strategieRecu="";
  }
  return transition;
}

byte ecrant_1(){
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant1||strategieRecu){
    if(premieerEcrant1){
      msg.id=0x006;
      msg.len=5;
      msg.buf[0]='D';//demande de strategie de matche dans le bute de l'afficher
      msg.buf[1]='m';
      msg.buf[2]='s';
      msg.buf[3]='t';
      msg.buf[4]='m';
      Can0.write(msg);
      receptionStrategic=true;
    }
    if (encodeurPos>3) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=3;
    encodeurPosP=encodeurPos;
    if(encodeurPos==0 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=5;
      msg.buf[0]='D';//demande de strategie de matche dans le bute de l'afficher
      msg.buf[1]='m';
      msg.buf[2]='s';
      msg.buf[3]='t';
      msg.buf[4]='m';
      Can0.write(msg);
      receptionStrategic=true;
    }
    if(encodeurPos==1 && bpEncodeur){
      coueleurChoie=!coueleurChoie;
    }
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    if(encodeurPos==0)u8g2.drawBox(0,10, 128, 11);
    if(encodeurPos==1)u8g2.drawBox(0,30, 128, 11);
    if(encodeurPos==2)u8g2.drawBox(0,40, 64, 11);
    if(encodeurPos==3)u8g2.drawBox(64,40, 128, 11);
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Choie strategie couleur Matche");
    u8g2.drawStr(0,19,"Strategie :");
    u8g2.setCursor(0,29);
    u8g2.print(Strategie);//voire en definitife
    u8g2.drawStr(0,39,"Couleur :");
    if(couleurStrategie==0){//pas de couleur couleur uniquement les test
      u8g2.drawStr(40,39,"incolor");
    }else if(couleurStrategie==1){
      u8g2.drawStr(40,39,"jaune");
    }else if(couleurStrategie==2){
      u8g2.drawStr(40,39,"violet");
    }else if (couleurStrategie==3){
      if(coueleurChoie) u8g2.drawStr(40,39,"jaune");
      else u8g2.drawStr(40,39,"violet");
    }
    u8g2.drawStr(0,49,"   valider     retoure");
    u8g2.sendBuffer();					// transfer internal memory to the display
    if(encodeurPos==3&&bpEncodeur)transition=0;
    else if(encodeurPos==2&&bpEncodeur)transition=4;
    else transition=1;
    bpEncodeur=0;
    premieerEcrant1=false;
  }
  if(transition!=1){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
  }
  return transition;
}

byte ecrant_2(){
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant2||strategieRecu){
    if(premieerEcrant2){
      msg.id=0x006;
      msg.len=5;
      msg.buf[0]='D';//demande de strategie de matche dans le bute de l'afficher
      msg.buf[1]='m';
      msg.buf[2]='s';
      msg.buf[3]='t';
      msg.buf[4]='m';
      Can0.write(msg);
      receptionStrategic=true;
    }
    if (encodeurPos>3) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=3;
    encodeurPosP=encodeurPos;
    if(encodeurPos==0 && bpEncodeur==1){
      msg.id=0x006;
      msg.len=5;
      msg.buf[0]='D';//demande de strategie de matche dans le bute de l'afficher
      msg.buf[1]='m';
      msg.buf[2]='s';
      msg.buf[3]='t';
      msg.buf[4]='m';
      Can0.write(msg);
      receptionStrategic=true;
    }
    if(encodeurPos==1 && bpEncodeur){
      coueleurChoie=!coueleurChoie;
    }
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    if(encodeurPos==0)u8g2.drawBox(0,10, 128, 11);
    if(encodeurPos==1)u8g2.drawBox(0,30, 128, 11);
    if(encodeurPos==2)u8g2.drawBox(0,40, 64, 11);
    if(encodeurPos==3)u8g2.drawBox(64,40, 128, 11);
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "Choie strategie couleur Test");
    u8g2.drawStr(0,19,"Strategie :");
    u8g2.setCursor(0,29);
    u8g2.print(Strategie);//voire en definitife
    u8g2.drawStr(0,39,"Couleur :");
    if(couleurStrategie==0){//pas de couleur couleur uniquement les test
      u8g2.drawStr(40,39,"incolor");
    }else if(couleurStrategie==1){
      u8g2.drawStr(40,39,"jaune");
    }else if(couleurStrategie==2){
      u8g2.drawStr(40,39,"violet");
    }else if (couleurStrategie==3){
      if(coueleurChoie) u8g2.drawStr(40,39,"jaune");
      else u8g2.drawStr(40,39,"violet");
    }
    u8g2.drawStr(0,49,"   valider     retoure");
    u8g2.sendBuffer();					// transfer internal memory to the display
    if(encodeurPos==3&&bpEncodeur)transition=0;
    else if(encodeurPos==2&&bpEncodeur)transition=4;
    else transition=0;
    bpEncodeur=0;
    premieerEcrant1=false;
    if(transition!=0){
      msg.id=0x006;
      msg.len=5;
      msg.buf[0]='D';//demande de strategie de matche dans le bute de l'afficher
      msg.buf[1]='m';
      msg.buf[2]='s';
      msg.buf[3]='t';
      msg.buf[4]='m';
      Can0.write(msg);
      premieerEcrant0=true;
      premieerEcrant1=true;
      premieerEcrant2=true;
      premieerEcrant3=true;
      premieerEcrant4=true;
      premieerEcrant5=true;
      premieerEcrant6=true;
      premieerEcrant7=true;
      encodeurPos=0;
      encodeurPosP=0;
    }
  }
  return transition;
}

byte ecrant_3(){
  if(encodeurPos!=encodeurPosP||bpEncodeur==1||premieerEcrant3){
    if (encodeurPos>1) encodeurPos=0;
    if (encodeurPos<0) encodeurPos=1;
    encodeurPosP=encodeurPos;
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.drawBox(0, (encodeurPos+1)*10, 128, 11);
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9,"Transmition de donner");
    u8g2.drawStr(0,19,"Retour");
    u8g2.drawStr(0,29,"Retour plus arrée");
    u8g2.sendBuffer();
    premieerEcrant3=false;
  }
  if(bpEncodeur&&pasEnComme){
    if(encodeurPos==1){
      msg.len=7;
      msg.buf[0]='S';//pour un arrée des echange
      msg.buf[1]='t';
      msg.buf[2]='o';
      msg.buf[3]='p';
      msg.buf[4]='c';
      msg.buf[5]='o';
      msg.buf[6]='m';
      Can0.write(msg);
    }
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
    return transition=0;
  }
  return transition=3;
}

byte ecrant_4(){
  if(premieerEcrant4){
    u8g2.setFontMode(1);  /* activate transparent font mode */
    u8g2.setDrawColor(1); /* color 1 for the box */
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,29,"En recalage");
    u8g2.sendBuffer();					// transfer internal memory to the display
    premieerEcrant4=false;
  }
  if(bpEncodeur==1&&modeMatche==0){
    msg.len=7;
    msg.buf[0]='F';//pour un arrée des echange
    msg.buf[1]='i';
    msg.buf[2]='n';
    msg.buf[3]='t';
    msg.buf[4]='e';
    msg.buf[5]='s';
    msg.buf[6]='t';
    Can0.write(msg);
    bpEncodeur=0;
    transition=0;
  }
  //getion du passage dans interueption can
  if(transition!=4){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
  }
  return transition;
}

byte ecrant_5(){
  transition=5;
  if(premieerEcrant5){
    if(digitalRead(TIRETTE)){
      u8g2.setFontMode(1);  /* activate transparent font mode */
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.setDrawColor(2);
      u8g2.drawStr(0,29,"Tirer la tirete pour lancer le matche");
      u8g2.sendBuffer();					// transfer internal memory to the display
    }else{
      u8g2.setFontMode(1);  /* activate transparent font mode */
      u8g2.setDrawColor(1); /* color 1 for the box */
      u8g2.setDrawColor(2);
      u8g2.drawStr(0,29,"metrre la tirette");
      u8g2.sendBuffer();
    }
    premieerEcrant5=false;
  }else{
    if(tiretteP!=digitalRead(TIRETTE)){
      if(digitalRead(TIRETTE)){
        msg.len=8;
        msg.buf[0]='D';//pour un arrée des echange
        msg.buf[1]='e';
        msg.buf[2]='b';
        msg.buf[3]='m';
        msg.buf[4]='t';
        msg.buf[5]='c';
        msg.buf[6]='h';
        msg.buf[6]='e';
        Can0.write(msg);
        transition=6;
      }else{
        u8g2.setFontMode(1);  /* activate transparent font mode */
        u8g2.setDrawColor(1); /* color 1 for the box */
        u8g2.setDrawColor(2);
        u8g2.drawStr(0,29,"Tirer la tirete pour lancer le matche");
        u8g2.sendBuffer();					// transfer internal memory to the display
      }
    }
  }
  if(bpEncodeur==1&&!modeMatche){
    msg.len=7;
    msg.buf[0]='F';//pour un arrée des echange
    msg.buf[1]='i';
    msg.buf[2]='n';
    msg.buf[3]='t';
    msg.buf[4]='e';
    msg.buf[5]='s';
    msg.buf[6]='t';
    Can0.write(msg);
    transition=0;
    bpEncodeur=0;
  }
  tiretteP=digitalRead(TIRETTE);
  if(transition!=5){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
  }
  return transition;
}

byte ecrant_6(){
  if(premieerEcrant6){
    Can0.write(msg);
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.drawStr(0,9, "en match");
    u8g2.sendBuffer();					// transfer internal memory to the display
  }
  if(bpEncodeur==1&&!modeMatche){
    msg.len=7;
    msg.buf[0]='F';//pour un arrée des echange
    msg.buf[1]='i';
    msg.buf[2]='n';
    msg.buf[3]='t';
    msg.buf[4]='e';
    msg.buf[5]='s';
    msg.buf[6]='t';
    Can0.write(msg);
    transition=0;
    bpEncodeur=0;
  }
  if(transition!=6){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
  }
  return transition;
}

byte ecrant_7(){
  transition=7;
  if(premieerEcrant7){
    Can0.write(msg);
    u8g2.clearBuffer();         // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr); // choose a suitable font
    u8g2.setDrawColor(2);
    u8g2.setCursor(10,29);
    u8g2.drawStr(0,9, "Nb point");
    u8g2.print(nombreDePoint);
    u8g2.sendBuffer();					// transfer internal memory to the display
    premieerEcrant7=0;
  }
  if(bpEncodeur==1&&!modeMatche){
    transition=0;
    bpEncodeur=0;
  }
  if(transition!=7){
    premieerEcrant0=true;
    premieerEcrant1=true;
    premieerEcrant2=true;
    premieerEcrant3=true;
    premieerEcrant4=true;
    premieerEcrant5=true;
    premieerEcrant6=true;
    premieerEcrant7=true;
    encodeurPos=0;
    encodeurPosP=0;
  }
  return transition;
}

void bpInterupEncodeur(){
  if(timerBpEncodeur > 100){
    bpEncodeur=1;
    timerBpEncodeur=0;
  }
}

void tournInterupEncodeurRisingA(){
  if(timerRotationEncodeur > 100){
    encodeurPos++;
    timerRotationEncodeur=0;
  }
}

void interpretationCan(){
  if(receptionStrategic){
    if(comparChenCan("Finstr"))receptionStrategic=false;
    else{
      for(int i=0;i<8;i++){
        strategieRecu+=msg.buf[i];
      }
    }
    couleurStrategie=msg.buf[6];
    coueleurChoie=false;
  }
  if(comparChenCan("Com")){//message de changement etat comme
    pasEnComme=!pasEnComme;
  }
  if(ecrant==4){
    if(comparChenCan("Finreca"))transition=5;
  }
  if(ecrant==6){
    int mult=1;
    for(int i=0;i<8;i++){
      nombreDePoint+=msg.buf[i]*mult;
      mult*=10;
    }
    if(comparChenCan("Finmatch"))transition=7;
  }

}

int comparChenCan(String comp){
  for(int i=0;i<9;i++){
    if(comp[i]=='\0')return 1;
    if(msg.buf[i]!=comp[i]) return 0;
  }
  return 0;
}

void IntrerrupTimer(){
  timerBpEncodeur++;
  timerRotationEncodeur++;
}
