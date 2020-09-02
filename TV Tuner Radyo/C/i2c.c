void i2c_bekle(void) {delay_us(20);}

void i2c_basla(void)
   {
      i2c_bekle();
      output_low(SCL);
      i2c_bekle();
      output_high(SDA);
      i2c_bekle();
      output_high(SCL);
      i2c_bekle();
      output_low(SDA);
      i2c_bekle();
      output_low(SCL);
      i2c_bekle();
   }

void i2c_dur(void)
   {
      i2c_bekle();
      output_low(SCL);
      i2c_bekle();
      output_low(SDA);
      i2c_bekle();
      output_high(SCL);
      i2c_bekle();
      output_high(SDA);
      i2c_bekle();
      output_low(SCL);
   }

void i2c_bit_gonder(BOOLEAN bit)
   {
      i2c_bekle();
      output_bit(SDA,bit);
      i2c_bekle();
      output_high(SCL);
      i2c_bekle();
      output_low(SCL);
   }

BOOLEAN i2c_bit_oku (void)
   {
      BOOLEAN a;
      i2c_bekle();
      set_tris_a(0x10);
      output_high(SCL);
      i2c_bekle();
      a=input(SDA);
      output_low(SCL);
      i2c_bekle();
      set_tris_a(0x00);
      return a;
   }

BOOLEAN i2c_byte_gonder(char giden)
   {
      char sayac;
      BOOLEAN onay;
      for(sayac=0;sayac<8;sayac++)i2c_bit_gonder(shift_left(&giden,1,0));
      onay=i2c_bit_oku();
      return onay;
   }

char i2c_byte_oku(BOOLEAN onay)
   {
      char gidecek=0,sayac;
      for(sayac=0;sayac<8;sayac++)shift_left(&gidecek,1,i2c_bit_oku());
      i2c_bit_gonder(onay);
      return gidecek;
   }

BOOLEAN i2c_kontrol(char cih_kodu)
   {
      BOOLEAN onay;
      i2c_basla();
      onay=i2c_byte_gonder(cih_kodu);
      i2c_dur();
      return onay;
   }
