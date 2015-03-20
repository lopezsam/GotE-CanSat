Data::Data(int pin)
{
  pinMode(pin, OUTPUT);
  _pin = pin;
}