/*
https://en.wikipedia.org/wiki/Cosmic_Calendar
https://en.wikipedia.org/wiki/Transistor
https://en.wikipedia.org/wiki/Hans_Christian_Oersted
https://en.wikipedia.org/wiki/Reddit


use kazam to record

      set kazam for 0 seconds.

      start dib, on kazam click "window", then click the window, then watch the countdown and click "capture" a second or so early

   file preferences faster framerate?

bang 13.8
milky way 11

solar system 4.57
life 3.8

complex cells 2
animals 0.67

land plants 0.45
mammals 0.2 = 200 mil

primates 65 mill
humans and stone tools 2.5

modern human 200 thousand
art 35 thou

agriculture 12
bronze, writing, stonehenge 5.5

alphabet wheel 4.5
iron 3

ptolemy zero 2

*/




#include "data_is_beautiful.h"

#include <QtWidgets>

#include <unistd.h>

int state = 0;
   
int yyy,ydelt;

float speed = 10000.0; // msec to draw line across screen

QTime my_time;

float actual_len, compare;

float total_time = 0;   // years. this starts over at zero whenever state changes

#define window_w 1400   // monitor is 1920 x 1080

#define window_h 900

#define num_states 20 // the number of bars representing events 

#define spacing 44


const char * array[] = {
"when this bar is complete, it will represent the 13.8 billion years since the big bang. this and most of the values to follow are approximations.",
"this bar is for the 11 billon years that the milky way galaxy has existed",
"the solar system is 4.57 billion years old",
"life on earth began 3.8 billion years ago",
"it took 1.8 billion more years to produce complex cells",
"animals have existed for 670 million years",
"land plants began growing 450 million years ago",
"mammals have been around for 200 million years",
"primates evolved 65 million years ago",
"63 million years later, humans were using stone tools",
"what we would consider a contemporary human evolved 200 thousand years ago",
"the oldest evidence of painting and sculpture is 35 thousand years old",
"agriculture was developed 12 thousand years ago",
"5500 years ago: stonehenge, early forms of writing, and the use of bronze",
"humans have used an alphabet and have known how to make wheels for 4500 years ",
"iron tools have been used for 3000 years",
"the first recorded use of the number 0 was 2000 years ago",
"Oersted first observed the interaction of electricity and magnetism in 1820",
"the transistor was invented at Bell Labs in 1947",
"2005: diggers began discussing a new site called Reddit"};

int colors[num_states][num_states] = { {155, 155, 255, 255},
                                       {100, 100, 205, 255},
                                       { 50,  50, 150, 255},
                                       
                                       {255, 155, 155, 255},
                                       {205, 100, 100, 255},
                                       {150,  50,  50, 255},

                                       {155, 255, 155, 255},
                                       {100, 205, 100, 255},
                                       { 50, 150,  50, 255},
                              
                                       {255, 255, 155, 255},
                                       {195, 195,  90, 255},
                                       {140, 140,  40, 255},
                              
                                       {155, 255, 255, 255},
                                       { 90, 195, 195, 255},
                                       { 40, 140, 140, 255},
                              
                                       {255, 155, 255, 255},
                                       {195,  90, 195, 255},
                                       {140,  40, 140, 255},

                                       {155, 155, 255, 255},
                                       {255,  69,   0, 255}};

/*
int colors[num_states][num_states] = { { 90, 155,  70, 255},
                                       { 50,  80, 155, 255},
                                       {  0, 220, 105, 255},
                                       
                                       {100, 120, 155, 255},
                                       {170,  90, 125, 255},
                                       { 70,  80, 135, 255},
                                       
                                       { 80, 170, 135, 255},
                                       {110,  70, 135, 255},
                                       {105,   0,  25, 255},
                                      
                                       {200, 205, 110, 255},
                                       { 50,  80, 155, 255},
                                       {  0, 220, 105, 255},
                                       
                                       {  0,  40, 225, 255},
                                       {170,  90, 125, 255},
                                       { 60,  90, 125, 255},
                                       
                                       { 80, 170, 135, 255},
                                       {130,  90, 135, 255},
                                       {105,   0,  25, 255},
                                       
                                       {  0, 220, 105, 255},
                                       {255,  69,   0, 255}};
*/

float age[num_states + 1] = {13800000000.0, 11000000000.0, 4570000000.0, 3800000000.0, 2000000000.0, 670000000.0, 450000000.0, 200000000.0, 
                             65000000.0, 2500000.0, 200000.0, 35000.0, 12000.0, 5500.0, 4500.0, 3000.0, 2000.0, 199.0,  72.0, 14.0, 1.0}; // years. needs a dummy value at the end

float lenra[num_states]; // saves the current x value for segments that are growing.

float xra[num_states][num_states];    // xra saves the old final x (right end of segment) for segments that have finished, meaning states that have completed and we are now drawing a later state. 
                                      // the index in the first column = 1 for the first segment. (index 0 is never used in the first column...). 
                                      // the second column is the state number which begins at 0
                                      // this array is filled when the state changes, and is used to save the final x coordinate for each segment so that the next state knows where to begin drawing.

DataIsBeautiful::DataIsBeautiful(QWidget *parent) : QWidget(parent){

   QTimer *timer = new QTimer(this);

   connect(timer, &QTimer::timeout, this, QOverload<>::of(&DataIsBeautiful::update));

   timer->start(10);      // msecs

   setWindowTitle(tr("Data Is Beautiful"));

   QPalette pal = palette();

   pal.setColor(QPalette::Background, Qt::black);

   setPalette(pal);

   setAutoFillBackground(true);

   resize(window_w,window_h);
   
   my_time.start();
}

int loop_count = 0;

int countdown = 7;

void DataIsBeautiful::paintEvent(QPaintEvent *){

   /*

      countdown for a few seconds. timer is 10 msec so 100 loops is one second.

   */

   if (countdown > 0){

      loop_count++;

      if (loop_count > 100){

         loop_count = 0;

         countdown --;
      }

      if (countdown > 0){

         state = 5000;

      } else {

         state = 0;

         my_time.restart();
      }

   }

   QPainter qp(this);
   qp.setRenderHint(QPainter::Antialiasing);

   QPen pen;  // creates a default pen

   pen.setWidth(14);
   pen.setCapStyle(Qt::RoundCap);
   pen.setJoinStyle(Qt::RoundJoin);
   pen.setStyle(Qt::SolidLine);

   if (countdown == 0){

      int msec = my_time.elapsed();

      float len = (float)( msec) * (float)(window_w) / speed;    // speed is the number of milliseconds to draw line across screen

      for (int i=0; i<=state; i++){

         if (i < num_states){

            pen.setBrush(QColor(colors[i][0],colors[i][1],colors[i][2],colors[i][3]));
            qp.setPen(pen);
         }
        
         yyy = spacing / 3  + i * spacing;

         if (i < state){  // draw segments that have finished growing
      
            qp.drawLine(0,  yyy, xra[1][i],  yyy);   // draws first segment after it is done

            if (i > 0){

               actual_len = xra[1][i];

               ydelt = spacing;

               for (int j=1; j<(state);j++){    // this draws segments after the first one. screws up top row blue and yellow. red and green (first and second) look ok on top row.

                  lenra[j+1] = xra[j][i-1] + actual_len *  age[i] / age[i-j]; 
                  qp.drawLine(xra[j][i-1],  yyy-ydelt, lenra[j+1],  yyy-ydelt);

                  ydelt += spacing;
               }
            }
         
         } else {  // draw segments that are still growing, meaning i == current state, the one at the bottom of the rows being drawn

            if (i < num_states){
              
               qp.drawLine(0,  yyy, len,  yyy);   // draws first segment at left end as it grows. this will be the lowest row being drawn.

               if (i > 0) {  // i is the state we are drawing. if i == 0 then there is only the main segment and none above it, so we're done here

                  ydelt = spacing;

                  for (int jj=1; jj<(state+1);jj++){   // start at the next line above the main one for this state

                     lenra[jj+1] = xra[jj][i-1] + len *  age[i] / age[i-jj]; // calculate x2 for this segment. use the length on the main row and ratio it for the smaller segment to go on a higher row, to the right of existing segments
                                                                              // the first instance will be for state 1 which will be on the second row, but will have a segment on the first row beginning where state 0 finished
                                                                              //
                                                                              // so, that x value will be at xra[2][0]
                                                                              
                     qp.drawLine(xra[jj][i-1],  yyy-ydelt, lenra[jj+1],  yyy-ydelt);

                     ydelt += spacing;
                  }
               }
            }
         }
      }

      total_time = (float)(msec) * age[state] / speed;    // total_time and age[] are years. speed is milliseconds to draw one line all the way across the screen, and age[] corresponds to screen width, so this
                                                          // calculation determines where we are as we go across. as soon as we get to where the next event occurs, we need to change states, which is done in the following
                                                          // "if" statement

      if (state == (num_states - 1)){

         compare = age[state];  // the final state will fill the entire bar

      } else {

         compare =  (age[state]  - age[state+1]) ;
      }

      if (total_time > compare){

         xra[1][state] = len;  // this saves the x coord for the right end of the main segment for this state, which will always begin at the left margin

         for (int l=2; l<num_states;l++){

            xra[l][state] = lenra[l];  // this is the x coord for the right end of the segment above the main segment, which will only exist for states > 0
         }

         my_time.restart();

         if (state < num_states){
            state ++;
         }
      }
   }

   qp.setPen(QColor(200,200,222,255));
   QFont my_font("PibotoLt", 30, QFont::Bold);

   my_font.setPointSize(11);

   qp.setFont(my_font);

   if (countdown > 0){

      if (countdown > 1){ // go blank for the final second

         char *he = (char*)malloc(73 * sizeof(char));
         sprintf(he, "%d ", countdown);

         qp.drawText(100,50,he);
      }

   } else {

      for (int m=0;m<=state;m++){

         if (m < num_states){
            qp.drawText(10,(m+1)*spacing - 2 ,array[m]);
         }
      }
   }
}

