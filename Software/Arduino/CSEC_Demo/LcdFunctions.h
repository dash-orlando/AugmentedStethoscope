  uint8_t clock[8]    = { 0x0, 0xe, 0x15, 0x17, 0x11, 0xe, 0x0 };
  uint8_t heart[8]    = { 0x0, 0xa, 0x1f, 0x1f, 0xe,  0x4, 0x0 };
  uint8_t check[8]    = { 0x0, 0x1, 0x3,  0x16, 0x1c, 0x8, 0x0 };
  uint8_t retarrow[8] = { 0x1, 0x1, 0x5,  0x9,  0x1f, 0x8, 0x4 };
  
  LiquidCrystal_I2C lcd( 0x20, 20, 4 );   // set the LCD address to 0x20(Cooperate with 3 short circuit caps) for a 16 chars and 2 line display



//*
void boboPrint( int col, int row, String s  )
{
  int len = s.length();

  for ( int i = 0; i < len ; i++ )
  {
      lcd.setCursor( col++, row );
      lcd.print( s[i] );
  }
} // */


void initLCD()
{
  lcd.init();                             // initialize the lcd 
  lcd.backlight();
  lcd.home();
  
  lcd.createChar( 0, clock );
  lcd.createChar( 1, heart );
  lcd.createChar( 2, check );
  lcd.createChar( 3, retarrow );
}

