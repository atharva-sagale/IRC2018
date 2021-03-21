void fwd()    //moves fwd
{

  //NORMAL FWD FN
  cspeed = 20;

  s.setSpeed(cspeed);
  s1.setSpeed(cspeed);

  for (int i = 0; i < fstep; i++)
  {

    if (i <= 60)
    {
      s.setSpeed(cspeed);
      s1.setSpeed(cspeed);
      s.step(1);
      s1.step(1);
      cspeed += 1;
    }
    else if (i < fstep - 30)
    { //Serial.println(i);
      cspeed = 80;
      s.setSpeed(cspeed);
      s1.setSpeed(cspeed);
      s.step(1);
      s1.step(1);
    }
    else
    { // Serial.println(cspeed);
      s.setSpeed(cspeed);
      s1.setSpeed(cspeed);
      s.step(1);
      s1.step(1);
      cspeed -= 1;
    }
  }

  // s.step(1);
  cspeed = 20;
  fwd_align();

}



