//v5.6
//to be sorted in files and cleaned

/*
  complete!
  v 2.1
  00 -> 11
  11 -> 00

  stops mic at button input for t
  2.2



*/
/*4.4 edit
   adding potnyiometer as a timer for lights
*/
//ossible that it gliches because arduino restarts...
//could be tested by changin the booting setup of lamps (ex: 11 -> 10(or any other comb-not gained by clapping-))


//2.65(alt) changed sound detection delta now equalls sv/sv1 instead of sv-sv1
//added that after clap delta must go to zero first before deteting another reading
//added a function for measuring delta

//  #2

#include <SevenSegmentDisplay.h> //screenName.displayCharacter('A'); screenName.displayDecimalPoint(false);

int sensorPin = A0;    // select the input pin for the mic
int sv ;
int sv1 ; // variable to store the value coming from the sensor
double delta ;
double delta2  ;
int deltam();
double deltamax;
double deltamax2;
int state = 0 ;
int state2 = 0 ;
int alt();
int alt2();
int button;
int button2;
int th = 15; //threshhold clap sound
int th2 = th - 1 ;
int ts = 700 ; //max time span in millie seconds between claps
int cd = 5 ; //clap duration
int t = 1000; //time that switch delays circuit (milli sec)
int dynth(); //dynamic threshhold amplitude
int tnoise ; // noise sampl
int tnoise2; // noise sample
int tn; // delta noise samples
int nt = 1000 ;//noise time (millis)
int offswitch = 1; //turns off the mic
int on1() ;
int on2() ;
int off1();
int off2();

int tdelta ; //v.4
int tdelta2;

int timer(); //4.4
int tmax = 11 ;//maximum dial for hours
long tr; //timer time in millies
unsigned long tt; //first time stamp
int ton ;//is timer on
long tr1;
long trh;

//4.5
int claps();
int buttons();
int staten = 0 ;
int staten2 = 0 ;
int buttonsalt();

//led
int screen(int sc);
const int led1 = 3;
const int led2 = 2;
const int led3 = 9;
const int led4 = 10;
const int led5 = 13;
const int led6 = 4;
const int led7 = 6;
const int led8 = A3;
SevenSegmentDisplay screenName(led1 , led2, led3, led4, led5, led6, led7, led8 , true);

int retime();
unsigned long retimeon ;

int secstohrs = 1  * 60 * 60 ; //60*60; //60*60 for hrs 1 for secs

void setup() {
  //used pins  5, 7, 8, 11, 12, A0,A1,
  //led: 2 3 4 6 9 10 A2 //&A3
  //free pins 0*, 1*, A4, A5


  pinMode(A0, INPUT_PULLUP);//mic 
  pinMode(A1, INPUT_PULLUP);//potntiometer
  pinMode(12, OUTPUT);//relay2 
  pinMode(11, OUTPUT);//relay
  digitalWrite( 12, HIGH);
  digitalWrite( 11, LOW);
  pinMode(5, INPUT_PULLUP);//device off switch
  pinMode(7, INPUT_PULLUP);//switch1
  pinMode(8, INPUT_PULLUP);//switch2
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  button = digitalRead(7);
  button2 = digitalRead(8);
  offswitch = digitalRead(5);




  Serial.begin(9600);

  buttons();
  //tr1 = analogRead(A1);
  delay(200) ;
}
void loop() {


  offswitch = digitalRead(5);
  if (offswitch == 0 )//mic on
  {
    buttonsalt();
    timer();
    screen(-1);
    dynth() ;
    deltam();


    if (delta > th ) {
      tdelta = millis();             //v.4

      delay(30);

      while (delta > 4) {
        deltam();

      }


      Serial.println("clap");                                   //delta diffrence  between 2 samples of noise taken righ after each other to detedct a pulse, th is threshold amplitude
      delay(300);
      if (abs(tdelta2 - tdelta) < ts)
      { //2 claps


        Serial.println("2 claps");
        claps();
        delay(30);
        while (delta > 4) {
          deltam();
        }

        delay(20);

        deltamax2 = 0; //for 3rd pulse
        //   Serial.println("start");
        for ( int i = 0; i < ts - 150  ; i++)
        {
          deltam();
          if (delta > th)
          {
            deltamax2 = delta;
            i = ts + 1 ;

          }

        }
        // Serial.println("finish");




        if (deltamax2 > th) {
          Serial.println("3 claps!");
          //        Serial.print("1=");
          //Serial.println(delta);
          //        Serial.print("2=");
          //        Serial.println(delta2);

          if (staten == 0 && staten2 == 0 ) // 3 pulsees from 00 -> 11
          {
            on1();
            on2();

          }
          else if (staten == 1 && staten2 == 1 )//  11 -> 00
          {

            off1();
            off2();
          }
          else if (staten == 0 && staten2 == 1) // 10 -> 00
          {
            off1();
            off2();
          }


          else if (staten == 1 && staten2 == 0)// 01 -> 11
          {
            on1();
            on2();
          }

        } // seconed relay functions
        /*else { // 2 pulses
          // claps();
          }*/

        delay(100) ;
      }
      tdelta2 = millis();
    }

  }

  else if (offswitch == 1) {
    delay(30) ;
    if (offswitch == 1) {

      buttons();

    }
  }
}
int dynth() {
  //dynamic threshold
  int tnoise3;
  int tn2 ;

  tnoise2 = millis();

  tn = abs(tnoise - tnoise2);
  if (delta > th2)
  {
    tnoise = millis();


    if (tn < nt  ) {


      //if (delta > th2)
      //{
      //  tnoise3 = millis();
      //    tn2 = abs(tnoise3 - tnoise2);

      //if(tn < nt)
      // {

      //  Serial.print("tn=");
      // Serial.println(tn);
      // if ( delta < 25 && delta >= 4  && delta < th )                //4.1 edit

      if ( delta < 25/* && delta >= 4  */ && delta > th )           //4.1 edit   //4.2 2
      {
        th = delta + 2;

      }

      else
      {
        th += 2;
      }
      if (th < 7) {
        th2 = th - 1 ;
      }

      if ( 6 < th < 10)
      {
        th2 = th - 2 ;
      }

      if (9 < th < 13 )
      {
        th2 = th - 3;

      }
      if (th > 12)
      {
        th2 = th - 4;
      }
      /*  Serial.print("th=");
        Serial.println(th);
      */
    }
  }
  // }
  //}

  if (tn > 1000 ) {

    tnoise += 400;
    if (th > 2)
    {
      th--;

    }
    if (th < 7) {
      th2 = th - 1 ;
    }

    if ( 6 < th < 10)
    {
      th2 = th - 2 ;
    }

    if (9 < th < 13 )
    {
      th2 = th - 3;

    }
    if (th > 12)
    {
      th2 = th - 4;
    }


    //Serial.print("th=");Serial.println(th);

  }
  //Serial.print("th=");
  //Serial.println(th);

}
int deltam() {
  double sv[4];
  delta = 0;
  delta2 = 0 ;
  int n = 4;
  for (int i = 0; i < n ; i++ ) {

    sv[i] = analogRead(sensorPin);
    delayMicroseconds(125);
  }
  for (int i = 0; i < n - 1 ; i++ ) {
    sv[i] += sv[i + 1] ;
    //  Serial.println(i);
    // Serial.println(sv[i]);

  }
  delta = (sv[0] / sv[2]);
  if (delta < 1)
    delta = 1 / delta ;
  delta = delta * 1000 - 1000 ;
  delta2 = delta ;
  for (int i = 0; i < n - 2 ; i++ ) {

    delta = (sv[i] / sv[i + 1]);
    if (delta < 1)
      delta = 1 / delta ;
    delta = delta * 1000 - 1000 ;


    if (delta > delta2)
      delta2 = delta ;




  }

  delta = delta2;

  delta2 = 0;

  //  delta2 = sv1 - sv;
  // delta2 = abs(delta2);
  // delta = delta2 ;
  //Serial.println(delta);
}
int alt() {
  if ( state == 0) {
    on1();
    // Serial.println("alt");
  }
  else if (state == 1 ) {
    off1();
    //Serial.println("alt");
  }

  delay(1);

  // delay (50);
}
int alt2() {
  if ( state2 == 1) {
    off2();
    //Serial.println("alt2");
  }
  else if (state2 == 0 ) {
    on2();
    //Serial.println("alt2");
  }

  delay(1);


  // delay (50);
}
int on2() {
  digitalWrite(11, HIGH);
  state2 = 1 ;
  Serial.println("2 on!");
}
int on1() {
  digitalWrite(12, LOW);
  Serial.println("1 on!");

  state = 1 ;
}
int off1() {

  digitalWrite(12, HIGH);
  Serial.println("1 off!");
  state = 0;

}
int off2() {
  digitalWrite(11, LOW);
  Serial.println("2 off!");
  state2 = 0;
}
int timer() { //4.4



  if (abs(analogRead(A1) - tr1) > 5) {
    while (abs(analogRead(A1) - tr1) > 0) {
      while (abs(analogRead(A1) - tr1) > 0) {
        tr1 =  analogRead(A1);
        tr = (tr1 - 0) * (0 - tmax) / (1023 - 0) + tmax;


        if (tr > 0/*.1*/ && ton == 0) {
          Serial.print("time ="); Serial.println(tr);
          //Serial.println("break point1");
          trh = tr;
          tt = millis();//time stamp the time timer starts

          screen(trh);//display timer time
          delay(100);

        }


        else if (ton == 1)
          break;
        delay(100);
      }
      delay(250);
    }
    //Serial.println("break point2");
    ton = 1;//turn alarm on  //5.2
    Serial.println("timer on!1");
    if (0 < tr < /*0.*/1 && ton == 1) { //if time = 0 then switch off alarm
      ton = 0;
      Serial.println("timer off");
      trh = 0;
      screen(0);
    }
    else if (tr > 0/*.1*/ && ton == 1) {
      retime();
    }


  }



  if (millis() >= tt + trh  * /* 60 * 60 */  secstohrs   * 1000 && ton == 1 && trh > 0 ) {
    on1();
    //on2();
    Serial.println("TIME");
    Serial.println("timer off");
    ton = 0;


  }

}
int buttons() {
  //return lights to defualt values of switches...
  if (digitalRead(7) == 0 && state == 0) {
    // go to on1
    delay(20);
    if (digitalRead(7) == 0 && state == 0) {
      on1();
      state = 1;
      button = digitalRead(7);

    }
  }
  else if (digitalRead(7) == 1 && state == 1) {
    delay(20);
    if (digitalRead(7) == 1 && state == 1) {
      off1();
      state = 0;
      button = digitalRead(7);
    }
  }
  if (digitalRead(8) == 0 && state2 == 0) {
    delay(20);
    if (digitalRead(8) == 0 && state2 == 0) {
      on2();
      state2 = 1;
      button2 = digitalRead(8);
    }
  }
  else if (digitalRead(8) == 1 && state2 == 1) {
    delay(20);
    if (digitalRead(8) == 1 && state2 == 1) {
      off2();
      state2 = 0;
      button2 = digitalRead(8);
    }
  }
}
int claps() {
  staten = state;
  staten2 = state2;
  if ( state == 1 && state2 == 1)//all high, if double 11 -> 01 priorty lamp
  {
    off2();

  }
  else if (state == 0 && state2 == 1) // 10 -> 01
  {

    off1();
    off2();
  }
  else {
    alt();
    off2();
  }

}
int buttonsalt() {

  if (button != digitalRead(7) || button2 != digitalRead(8) )
  {
    t = 100;

    for (int i = 0 ; i < t; i++)
    {
      delay(50)  ;

      if (button != digitalRead(7))
      {
        button = digitalRead(7);
        alt();
        Serial.println("switch1");
      }



      if (button2 != digitalRead(8))
      {
        button2 = digitalRead(8);
        alt2();
        Serial.println("switch2");

      }

    }
  }
}
int screen(int sc) {
  //  int screenstate;

  int scdec = sc;

  if (sc > 16)
    sc /= 10;

  if (sc >= 0) {

    char dis = 0;
    //Serial.println(sc);
    Serial.println(scdec);

    if ( sc < 10)
      dis = sc + '0';
    else
      dis = sc - 10 + 'a' ;
    screenName.displayCharacter(dis);
    retimeon = millis();
    //  screenstate = 1 ;

    if (scdec > 16) {
      Serial.println("bp1");
      // sc /= 10 ;
      scdec -= sc * 10;
      screenName.displayDecimalPoint(true);

      delay(700);

      screenName.displayDecimalPoint(false);


      if ( scdec < 10)
        dis = scdec + '0';
      else
        dis = scdec - 10 + 'a' ;
      screenName.displayCharacter(dis);
      retimeon = millis();
      //  screenstate = 1 ;
    }
  }


  else if (sc < 0 ) {
    if (millis() > retimeon + 2000  ) {
      screenName.displayCharacter('#');
      //  screenstate = 0 ;
    }
  }
}
int retime() { // remaining time
  //unsigned long retimevar;
  float retimevar;
  retimevar = ( tt + trh * 1000 * secstohrs /* 60 * 60*/ - millis() ) / (1000);
  retimevar /= secstohrs /*(60 * 60)*/;

  if (ton == 1 && retimevar >= 0 ) { //if timer is on
    // Serial.println("true");

    //  Serial.print("display=");Serial.println(retimevar );//remaining time = expected time (flag) +
    screen( retimevar * 10); //display remaining time//5.5


  }
}
