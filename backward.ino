//.............................................................
void back()
{
  cspeed = 20;
  s.setSpeed(cspeed);
  s1.setSpeed(cspeed);
  for (int i = 0; i < fstep; i++)
  {

    if (i < 60)
    {
      s.setSpeed(cspeed);
      s1.setSpeed(cspeed);
      s.step(-1);
      s1.step(-1);
      cspeed += 1;
    }

    else
    {
      cspeed = 80;
      s.setSpeed(cspeed);
      s1.setSpeed(cspeed);
      s.step(-1);
      s1.step(-1);
    }
  }
  cspeed = 20;
  //s.step(-10);//step tuning for perfect reverse

  rev_align();
}



//......................................................
void speedset()//to set speed of left and right motors
{
  s.setSpeed(cspeed);
  s1.setSpeed(cspeed);
}

