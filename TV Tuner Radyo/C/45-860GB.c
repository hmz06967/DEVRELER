// Bu program Murat KAYA tarafýndan hazýrlanmýþtýr.

#include "D:\Elektronik\Projelerim\45-860 Geniþ Bant\C\45-860GB.h"
#include "D:\Elektronik\Projelerim\45-860 Geniþ Bant\C\i2c.c"        // I2C veri yolu kontrol fonksiyonlarýný içeren kaynak dosyasý.
#include "D:\Elektronik\Projelerim\45-860 Geniþ Bant\C\led.c"        // 4x7 parçalý gösterge kullanýmýyla ilgili fonksiyonlarý içeren kaynak dosyasý.

#define TUNER_ADR 0xC2  // Tuner I2C adresi

int8 band[3]={0x01,0x02,0x04};    // Tuner band anahtarlarý
int32 frekans=4500;               // Baþlangýç deðeri 45MHz

struct tuner {                    // Tuner kaydedicilerini içeren yapý
      int16 bolen;
      char  kontrol1;
      char  kontrol2;}tuner_kontrol;
      
void i2c_word_gonder(int16 gidecek)    // I2C üzerinden 16 bit gönderen fonksiyon
   {
      int8 parca1,parca2;
      parca1=gidecek&0x00FF;
      parca2=(gidecek>>8)&0x00FF;
      i2c_byte_gonder(parca2);
      i2c_byte_gonder(parca1);
   }
   
void bolen_gonder (void)
   {
      i2c_word_gonder(tuner_kontrol.bolen);
   }
   
void kontrol_gonder(void)
   {
      i2c_byte_gonder(tuner_kontrol.kontrol1);
      i2c_byte_gonder(tuner_kontrol.kontrol2);
   }
   
void i2c_tuner_gonder(int1 anahtar)
   {
      if (frekans>=43000) tuner_kontrol.kontrol2=band[2];            // 430MHz'den yüksekse Yüksek Bant
      else if (frekans>=14000) tuner_kontrol.kontrol2=band[1];       // 140MHz'den yüksekse Orta Bant
      else tuner_kontrol.kontrol2=band[0];                           // hiçbiri deðilse Alçak Bant
      i2c_basla();
      i2c_byte_gonder(TUNER_ADR);
      switch(anahtar)
         {
             case 0:{bolen_gonder();kontrol_gonder();}break;
             case 1:{kontrol_gonder();bolen_gonder();}break;
         }
      i2c_dur();
   }
   
void frekans_arttir(void)
   {
      if (frekans<86000)
         {
            frekans += 5;
            tuner_kontrol.bolen++;
            i2c_tuner_gonder(0x00);
         }
   }
   
void frekans_azalt(void)
   {
      if(frekans>4500)
         {
            frekans -= 5;
            tuner_kontrol.bolen--;
            i2c_tuner_gonder(0x01);
         }
   }
   
void frekans_mhz_arttir(void)
   {
      if (frekans<85900)
         {
            frekans+=100;
            tuner_kontrol.bolen+=0x14;
            i2c_tuner_gonder(0x00);
         }
   }
   
void frekans_mhz_azalt(void)
   {
      if(frekans>4600)
         {
            frekans-=100;
            tuner_kontrol.bolen-=0x14;
            i2c_tuner_gonder(0x01);
         }
   }
   
void main()
{
   BOOLEAN anahtar=TRUE;
   int16 sayac=0;
   int8 durum=0, rotary_anahtar[3], yedek;
   
   setup_adc_ports(NO_ANALOGS);
   setup_adc(ADC_OFF);
   setup_psp(PSP_DISABLED);
   setup_spi(SPI_SS_DISABLED);
   setup_timer_0(RTCC_INTERNAL|RTCC_DIV_2);
   setup_timer_1(T1_DISABLED);
   setup_timer_2(T2_DISABLED,0,1);
   set_tris_a(0x00);
   set_tris_b(0x07);
   set_tris_c(0x00);
   set_tris_d(0x00);
   set_tris_e(0x00);
   disable_interrupts(GLOBAL);
   
   tuner_kontrol.bolen=0x0676;      // DÝKKAT! BU NOKTA ÖNEMLÝ... Kullanýlacak 1. IF frekansýna göre burasý ayarlanacak.
   tuner_kontrol.kontrol1=0xC8;
   i2c_tuner_gonder(0x00);
   rotary_anahtar[0]=0x00;
   rotary_anahtar[1]=0x00;
   rotary_anahtar[2]=0x00;
   donustur(frekans);
   while(TRUE)
      {
         yedek=input_b()& 0x03;
         if(rotary_anahtar[0]!=yedek)
            {
               rotary_anahtar[2]=rotary_anahtar[1];
               rotary_anahtar[1]=rotary_anahtar[0];
               rotary_anahtar[0]=yedek;
               if(((rotary_anahtar[2]==0x00)&&(rotary_anahtar[0]==0x03))||((rotary_anahtar[2]==0x03)&&(rotary_anahtar[0]==0x0))) durum=(rotary_anahtar[2]^rotary_anahtar[1]);
               switch(durum)
                  {
                     case 1:  {if(mhz_artma) frekans_mhz_arttir(); else frekans_arttir();} break;
                     case 2:  {if(mhz_artma) frekans_mhz_azalt(); else frekans_azalt();} break; 
                  } 
               durum=0;
               donustur(frekans);
            }
         
          if(input(PIN_B2)==0 && anahtar)
            {
               while(input(PIN_B2)==0 && sayac<10000) sayac++;
               if(sayac==10000) mhz_artma=!mhz_artma;
               donustur(frekans);
               sayac=0;
               anahtar=FALSE;
            }
          if(input(PIN_B2)==1 && !anahtar) anahtar=TRUE;
          led_goster();
      }
}
