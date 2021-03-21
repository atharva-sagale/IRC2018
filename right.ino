void right()
{
  s.setSpeed(cspeed);
  s1.setSpeed(cspeed);

  for (int i = 0; i < 130; i++)
  {

    s.setSpeed(20);
    s1.setSpeed(20);

    s1.step(-1);
    s.step(1);
  }

  delay(300);
  right_align();
}

