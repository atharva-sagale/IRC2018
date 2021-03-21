float Q::ldist() {


  lti = a.ping();

  delay(50);

  ld = sp * lti / 2;
  if (ld == 0)
  {
    ldist();
  }
  else
  {
    return (ld / 10000);
  }


}
float Q::fdist()
{
  fti = c.ping();

  delay(50);

  fd = sp * fti / 2;
  if (fd == 0)
  {
    fdist();
  }
  else
  {
    return (fd / 10000);
  }
}
float Q::rdist()
{

  rti = b.ping();

  delay(50);

  rd = sp * rti / 2;
  if (rd == 0)
  {
    rdist();
  }
  else
  {
    return (rd / 10000);
  }
}



