//........................................
#include <Stepper.h>

const int steps = 200;  // change this to fit the number of steps per revolution for your motor

Stepper s(steps, 6, 8, 9, 7); //right stepper
Stepper s1(steps, 2, 4, 5, 3); //left stepper


int cspeed = 20; //constant speed of bot

#define fstep 330 //forward steps


//................................
#include <NewPing.h>
#define sv 10.0
#define le 12 //front echo
#define lt 12  //front trigger
#define fe 11
#define ft 11
#define re 10
#define rt 10
#define mx 450 //max dist for us readings

#define c1 t1>20.0
#define c2 t2>20.0
#define c3 t3>20.0

NewPing a(lt, le, mx);
NewPing b(rt, re, mx);
NewPing c(ft, fe, mx);

NewPing l(lt, le, mx);
NewPing f(rt, re, mx);
NewPing r(ft, fe, mx);

//.............................................................

int  count = 0;
float lti = 0, fti = 0, rti = 0, ld = 0, fd = 0, rd = 0, t1, t2, t3 ;
int sp = 331.4 + 0.606 * 26 + 0.0124 * 50, block = 0;
char ch, p;
bool val, cmd, flg = 0;


//........................................................................



class pos
{
  public:
    short int x, y, i;
    char d[4] = {'n', 'e', 's', 'w'};

    pos()
    {
      x = y = i = 0;
    }
    void disp_pos();
};

pos *dir = new pos();

class Q
{
    char arr[5][36];
  public:
    int top = 0, top1 = 0;
    void push(char a)
    {
      arr[block][top] = a;
      if (flg == 1)
      {
        arr[2][top1] = a;
        top1++;
      }
      top++;
    }

    char pop()
    {
      p = arr[block][--top];
      if (flg == 1)
      {
        char x = arr[2][--top1];
      }
      return p;
    }
    void disp();
    float ldist();
    float rdist();
    float fdist();
    void replace1();
    void check1();
    void checkk1();
    void check2();
};

void pos:: disp_pos()
{
  Serial.print("X :");
  Serial.println(x);
  Serial.print("Y :");
  Serial.println(y);
  Serial.print("direction :");
  Serial.println(d[i]);
}

void fwd_align()
{
  //--------------when going forward-----------
  switch (dir->d[dir->i])
  {
    case'n':
      dir->y++;
      break;
    case 'e':
      dir->x++;
      break;
    case 's':
      dir->y--;
      break;
    case 'w':
      dir->x--;
  }

}
void rev_align()
{
  //  ---------------when going reverse------------------
  switch (dir->d[dir->i])
  {
    case'n':
      dir->y--;
      break;
    case 'e':
      dir->x--;
      break;
    case 's':
      dir->y++;
      break;
    case 'w':
      dir->x++;
  }

}

void right_align()
{
  // -------------in spot turn right------------
  dir->i = (dir->i + 1) % 4;

}
void left_align()
{
  //  -------------in spot turn left--------------
  dir->i = (dir->i + 3) % 4;

}
Q *obj = new Q();

void mapp()
{
  t1 = obj->ldist();
  t2 = obj->fdist();
  t3 = obj->rdist();
  Serial.print(t1);
  Serial.print(" ");
  Serial.print(t2);
  Serial.print(" ");
  Serial.print(t3);
  Serial.println();

  if (c1 && !(c2) && !(c3) )                    // nista left
  {
    ch = '1';
    Serial.println("Left open");
  }
  else if (c2 && !(c1) && !(c3))                     // nista front
  {
    ch = '2';
    Serial.println("Front open");
  }
  else if (c3 && !(c2) && !(c1))                     // nista right
  {
    ch = '3';
    Serial.println("Right open");
  }
  else if (c1 && c2 && !(c3))                    // left ani front
  {
    ch = '4';
    Serial.println("Left and Front open");
  }
  else if (c1 && c3 && !(c2))                    // left ani right
  {
    ch = '5';
    Serial.println("Left and Right open");
  }
  else if (c3 && c2 && !(c1))                    // front ani right
  {
    ch = '6';
    Serial.println("Front and Right open");
  }
  else if (!(c1) && !(c2) && !(c3))                    // dead end
  {
    ch = '7';
    Serial.println("Dead end");
    // deadend();                     //dead end fn calling
  }
  else if (c1 && c2 && c3)                    // + junction
  {
    ch = '8';
    Serial.println("All open");
  }

  obj->push(ch);

  //it tells to which direction the bot must move
  if (ch == '1' || ch == '4' || ch == '5' || ch == '8')
  {
    Serial.println("\nGo left and move fwd");
    left();
    //        delay(1000);
    fwd();
  }
  else if (ch == '2' || ch == '6' )
  {
    Serial.println("\nGo front");
    fwd();
  }
  else if (ch == '7')
  {
    Serial.println("\nDead end");

    obj->replace1();
  }
  else
  {
    Serial.println("\nGo right and move fwd");
    right();
    //       delay(1000);
    fwd();
  }

  if (ch == 'd')
  {
    obj->disp();

  }
  //Serial.println("..........................................");


  //delay(1000);
  //}
  dir->disp_pos();
}



//------------------------------------------------------------------

void Q::disp()
{
  Serial.println("Path is : ");
  for (int i = 0 ; i < top; i++) {
    if (arr[block][i] == '1')
    {
      Serial.println("Left and move fwd");
      left();
      //   delay(500);
      fwd();
    }
    else if (arr[block][i] == '2')
    {
      Serial.println("Front");
      fwd();
    }
    else if (arr[block][i] == '3')
    {
      Serial.println("Right and move fwd");
      right();
      //   delay(500);
      fwd();
    }
    else if (arr[block][i] == '4')
    {
      Serial.println("In Left and Front go Left");
      left();
      //   delay(500);
      fwd();
    }
    else if (arr[block][i] == '5')
    {
      Serial.println("In Left and Right go Left");
      left();
      //     delay(500);
      fwd();
    }
    else if (arr[block][i] == '6')
    {
      Serial.println("In Front and Right go Front");
      fwd();
    }
    else if (arr[block][i] == '7')
    {
      Serial.println("Dead end");

    }
    else if (arr[block][i] == '8')
    {
      Serial.println("All open");
    }

  }

  block++;
}

//------------------------------------------check for qr block----------------------------------

void  Q:: check1()
{
  int ctr = 0 ;
  if (dir->x == 1 && dir->y == 8)
  {
    //    if (dir->d[dir->i] != 'e')
    //    {
    //      while (dir->d[dir->i] != 'e')
    //      {
    //        //spot turn
    //        ctr++;
    //
    //      }
    //    }
    //    //ping beagle to check if qr
    //    //if yes then
    //    //flg++ ;
    //    while(ctr != 0)
    //    {
    //      //opposite spot turn
    //      ctr--;
    //      }
    //
    //  }
    delay(10000);
    obj->disp();

  }
}

//----------------------------------------------------------------------------------------------------------------------------------------------

void Q:: checkk1()
{
  int lx , fx , rx, ly, fy, ry;
  lx = ly = fx = fy = rx = ry = -1;
  switch (dir->d[dir->i])
  {
    case 'n':
      fy = dir->y + 1;
      lx = dir->x - 1;
      rx = dir->x + 1;
      break;

    case 'e':
      fx = dir->x + 1;
      ly = dir->y + 1;
      ry = dir->y - 1;
      break;

    case 's':
      fy = dir->y - 1;
      lx = dir->x + 1;
      rx = dir->x - 1;
      break;

    case 'w':
      fx = dir->x + 1;
      ly = dir->y - 1;
      ry = dir->y + 1;
      break;


    }
//  if(dir->x == 3 && dir->y == 4)
//  flg++;
  
  if ( (dir->x == 0 || dir->x == 3) || (dir->y == 0 || dir->y == 8) || (fx == 0 || fx == 3) || (fy == 0 || fy == 8) )
  {
    //scan function():
    //ping beagle
    //if (block) then
    //block++ and top=0
    //endif
  }
  if ( (lx == 0 || lx == 3) || (ly == 0 || ly == 8))
  {
    //left spot turn
    //scan()
    //right spotturn
  }

  if ( (rx == 0 || rx == 3) || (ry == 0 || ry == 8))
  {
    //spot turn
    //scan()
    //opposite spotturn
  }
}

void Q:: check2()
{
  int ctr = 0;
  if ( (dir->x == 0 && dir->y == 7) || (dir->x == 1 && dir->y == 8) )
  {
    if (dir->d[dir->i] != 's')
    {
      while (dir->d[dir->i] != 's')
      {
        //spot turn
        ctr++;

      }
    }
    delay(10000);
    if (fdist() > 10)
    {
      //next dead end is the end
    }
    else
    {
      //goto other co-ordinate
    }

  }
}
//------------------------------------------------------------------------------------------------------------------------------------------------------


void Q::replace1()
{ count++;
  //char c = obj->pop();
  Serial.print("in replace function c = ");
  char c = pop();
  Serial.println(c);
  while (c == '1' || c == '2' || c == '3' || c == '7')
  {
    if (c == '2') //front
    {
      Serial.println("go reverse");
      back();
    }
    else if (c == '3') //right
    {
      Serial.println("go spot turn left and backward");
      left();
      //  delay(500);
      back();
    }

    else if (c == '1') //left
    {
      Serial.println("go spot turn right and backward");
      right();
      // delay(500);
      back();
    }
    else if (c == '7') //dead end
    {
      Serial.println("go reverse on dead end");
      back();

    }
    else
    { Serial.print("c =");
      Serial.print(c);
      Serial.println("no direction");
    }
    c = obj->pop();

  }

  // c=obj->pop();
  if (c == '4') //left and front
  {
    Serial.println("after backtracking align bot to right and then move front");
    obj->push('2');

    right();
    // delay(500);
    fwd();

  }
  else if (c == '5') //left and right
  {
    Serial.println("after backtracking align bot to right once and then move right and fwd");
    obj->push('3');
    right();
    //  delay(500);
    right();
    //   delay(500);
    fwd();
  }

  else if (c == '6') //front and right
  {
    Serial.println("after backtracking move bot to right");
    obj->push('3');
    right();
    //  delay(500);
    fwd();// change made on 15 november thursday

  }
  else if (c == '8')
  {
    Serial.println("after backtracking align bot to right");
    obj->push('6');
    right();

  }


}
//------------------------------------------------------------------
void setup()
{
  speedset();
  Serial.begin(9600);
}

void loop1()
{ if (fdist1() <= 4)
  {
    cspeed = 20;

    s.setSpeed(cspeed);
    s1.setSpeed(cspeed);
    for (int i = 0; i < 8; i++)
    {
      s.step(-1);
      s1.step(-1);
    }

  }
  else
  { obj->check1();
    mapp();
  }

}
//...................................................................
void loop()
{
  right();
  delay(500);
}

